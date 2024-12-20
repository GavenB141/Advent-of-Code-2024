#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Get the output of one iteration given an a input
// - Assumes that registers B and C are effectively independent 
//   from prior iterations
// - Assumes that the program is encompassed by one loop
// - Assumes that the program only outputs once per loop
int iter_output_from_a (string program, uint64_t a) {
    
    uint64_t combo[8] = {0, 1, 2, 3, a, 0, 0, 7};

    for (int i = 0; i < program.length(); i += 2) {
        int call = program[i] - '0';
        int op = program[i + 1] - '0';
        switch (call) {
            case 0: 
                combo[4] /= 1 << combo[op];
                break;
            case 1:
                combo[5] ^= op;
                break;
            case 2:
                combo[5] = combo[op] % 8;
                break;
            case 3:
                break; // ignore here
            case 4:
                combo[5] ^= combo[6];
                break;
            case 5:
                return combo[op] % 8;
            case 6:
                combo[5] = combo[4] / (1 << combo[op]);
                break;
            case 7:
                combo[6] = combo[4] / (1 << combo[op]);
                break;
        }
    }

    return -1;
}

// Assumes that A is divided by 8 each loop
uint64_t solve_for(
    const string& program,
    string sub = "",
    uint64_t min = 0,
    uint64_t max = 8
) {
    if (sub.empty()) {
        sub = program;
    }

    for (uint64_t i = min; i < max; i++) {
        int digit = iter_output_from_a(program, i);
        if (digit + '0' == sub[sub.length()-1]) {
            string next_sub = sub;
            next_sub.pop_back();

            if (next_sub.empty()) {
                return i;
            } else {
                uint64_t rec = solve_for(program, next_sub, i * 8, (i + 1) * 8);
                if (rec == 0) {
                    continue;
                }
                return rec;
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    // skip directly to program line,
    // since register input is irrelevant
    string s;
    while (getline(input, s)) {
        if (s.length() == 0) {
            getline(input, s);
            break;
        }
    }

    string program;
    for (char c : s) {
        // only recognize 3-bit values
        if (c < '0' || c > '7')
            continue;

        program += c;
    }

    cout << solve_for(program) << endl;

    input.close();
    return 0;
}
