#include <cmath>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

using namespace std;

struct Puter {
    vector<short> program;
    int ic = 0;
    long reg_a;
    long reg_b;
    long reg_c;
    ostream* outstr;

    void execute() {
        while (ic < program.size()) {
            short operand = program[ic + 1];
            switch (program[ic]) {
                case 0: adv(operand); break;
                case 1: bxl(operand); break;
                case 2: bst(operand); break;
                case 3: jnz(operand); break;
                case 4: bxc(operand); break;
                case 5: out(operand); break;
                case 6: bdv(operand); break;
                case 7: cdv(operand); break;
            }
        }
    }

private:
    long combo(long operand) {
        switch (operand) {
            case 4: return reg_a;
            case 5: return reg_b;
            case 6: return reg_c;
            case 7: cout << "uh oh stinky\n";
            default: return operand;
        }
    }

    void adv(long operand) {
        reg_a = reg_a / pow(2, combo(operand));
        ic += 2;
    }

    void bxl(long operand) {
        reg_b ^= operand;
        ic += 2;
    }

    void bst(long operand) {
        reg_b = combo(operand) % 8;
        ic += 2;
    }

    void jnz(long operand) {
        if (reg_a == 0)
            ic += 2;
        else
            ic = operand;
    }

    void bxc(long operand) {
        reg_b ^= reg_c;
        ic += 2;
    }

    void out(long operand) {
        *outstr << combo(operand) % 8 << ',';
        ic += 2;
    }

    void bdv(long operand) {
        reg_b = reg_a / pow(2, combo(operand));
        ic += 2;
    }

    void cdv(long operand) {
        reg_c = reg_a / pow(2, combo(operand));
        ic += 2;
    }
};

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    Puter puter;
    puter.program = {};
    string s;
    input >> s >> s >> puter.reg_a 
        >> s >> s >> puter.reg_b 
        >> s >> s >> puter.reg_c >> s >> s;

    for (char c : s) {
        if (c == ',')
            continue;
        puter.program.push_back(c - '0');
    }

    ostringstream oss;
    puter.outstr = &oss;
    puter.execute();

    string out = oss.str();
    out.pop_back();
    cout << out << endl;

    input.close();
    return 0;
}
