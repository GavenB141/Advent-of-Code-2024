#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void blink (vector<uint64_t>& stones, vector<uint64_t>& quantity) {

    // blink
    for (int i = 0; i < stones.size(); i++) {
        if (stones[i] == 0) {
            stones[i] = 1;
            continue;
        }

        string str = to_string(stones[i]);

        if (str.length() % 2 == 1) {
            stones[i] *= 2024;
        } else {
            str.insert(str.length() / 2, " ");
            istringstream iss(str);

            int left, right;
            iss >> left >> right;

            stones[i] = left;
            stones.insert(stones.begin() + i + 1, right);
            quantity.insert(quantity.begin() + i + 1, quantity[i]);
            i++;
        }
    }

    // consolidate matching stones
    for (int i = 0; i < stones.size(); i++) {;
        auto index = 
            find(stones.begin(), stones.begin() + i, stones[i]) - stones.begin();

        if (index != i) {
            quantity[index] += quantity[i];
            stones.erase(stones.begin() + i);
            quantity.erase(quantity.begin() + i);

            i--;
        }
    }

}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    vector<uint64_t> stones;
    vector<uint64_t> quantity;
    uint64_t current;
    while (input >> current) {
        stones.push_back(current);
        quantity.push_back(1);
    }

    for(int i = 0; i < 75; i++) {
        blink(stones, quantity);
    }

    uint64_t total = 0;
    for(int i = 0; i < quantity.size(); i++) {
        total += quantity[i];
    }

    cout << total << endl;

    input.close();
    return 0;
}
