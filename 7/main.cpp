#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct equation {
    uint64_t target;
    vector<uint64_t> operands;

    bool can_be_true () {
        int num_ops = operands.size() - 1;

        uint64_t combos = pow(pow(2, num_ops), 2);

        for (uint64_t i = 0; i < combos; i++) {
            uint64_t total = operands[0];
            for (int j = 0; j < num_ops; j++) {
                // top condition for part 2
                if (i & ((1 << num_ops) << j)) {
                    stringstream stream;
                    stream << total << operands[j+1];
                    stream >> total;
                } else if (i & (1 << j)) {
                    total *= operands[j+1];
                } else {
                    total += operands[j+1];
                }
            }

            if (total == target) {
                return true;
            }
        }

        return false;
    }
};

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    // parse file
    vector<equation> eqs;
    string line;
    while (getline(input, line)) {
        istringstream iss(line);
        equation eq;

        iss >> eq.target;
        
        char _;
        iss >> _;

        uint64_t value;
        while (iss >> value) {
            eq.operands.push_back(value);
        }

        eqs.push_back(eq);
    }

    uint64_t total = 0;

    for (equation eq : eqs) {
        if (eq.can_be_true()) {
            total += eq.target;
        }
    }

    cout << total << endl;

    input.close();
    return 0;
}
