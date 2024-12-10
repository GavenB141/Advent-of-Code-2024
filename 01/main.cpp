#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main (int argc, char *argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    vector<int> left = {};
    vector<int> right = {};

    int a, b;
    while (input >> a && input >> b) {
        left.push_back(a);
        right.push_back(b);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    int sum = 0;
    for(int i = 0; i < left.size(); i++) {
        // Part one
        // sum += abs(left[i] - right[i]);

        // Part two
        sum += left[i] * count(right.begin(), right.end(), left[i]);
    }

    cout << sum << endl;

    input.close();
    return 0;
}
