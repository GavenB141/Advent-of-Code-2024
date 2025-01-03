#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

int evolve(uint64_t secret) {
    static const int prune = 16777216;

    secret ^= secret * 64;
    secret %= prune;

    secret ^= secret / 32;
    secret %= prune;

    secret ^= secret * 2048;
    secret %= prune;

    return secret;
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    vector<vector<int8_t>> prices;
    vector<vector<int8_t>> deltas;

    int secret;
    while (input >> secret) {
        int8_t prev_price = secret % 10;
        vector<int8_t> monkey = {prev_price};
        vector<int8_t> mdeltas = {10};

        for (int i = 0; i < 2000; i++) {
            secret = evolve(secret);
            int8_t price = secret % 10;
            monkey.push_back(price);
            mdeltas.push_back(price - prev_price);
            prev_price = price;
        }

        prices.push_back(monkey);
        deltas.push_back(mdeltas);
    }

    uint64_t max = 0;

    // haha i know
    // It's not a terrible wait for what it is
    for (int w = -9; w <= 9; w++) {
        for (int x = -9; x <= 9; x++) {
            for (int y = -9; y <= 9; y++) {
                for (int z = -9; z <= 9; z++) {
                    vector<int> seq = {w, x, y, z};
                    cout << w << x << y << z << '\n';

                    uint64_t total = 0;
                    for (int i = 0; i < prices.size(); i++) {
                        auto it = search(deltas[i].begin(), deltas[i].end(), seq.begin(), seq.end());

                        if (it != deltas[i].end()) {
                            total += prices[i][distance(deltas[i].begin(), it) + 3];
                        }
                    }

                    if (total > max) {
                        max = total;
                    }
                }
            }
        }
    }

    cout << max << endl;

    input.close();
    return 0;
}
