#include <fstream>
#include <iostream>

using namespace std;

// reads each character in the stream until it doesn't match the target
// returns true if it does
bool is_next(ifstream& input, string target) {
    char current;
    for(int i = 0; i < target.length(); i++) {
        if (input.peek() != target[i]) 
            return false;
        input >> current;
    }
    return true;
}

int main (int argc, char *argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    int sum = 0;

    char current;
    bool enabled = true; // for part 2

    while (!input.eof()) {
        int a = 0, b = 0;

        input >> current;

        // detect conditionals for part 2
        if (current == 'd') {
            if (input.peek() == 'o')
                input >> current;
            else continue;

            if (is_next(input, "n't()"))
                enabled = false;
            else if (is_next(input, "()"))
                enabled = true;
        }
        if (current != 'm' || !enabled) {
            continue;
        }

        // detect start
        if (!is_next(input, "ul(")) continue;

        // detect a number and a comma
        input >> current;
        for (int i = 0; current != ','; i++, input >> current) {
            if (i >= 3 || current < 48 || current > 57)
                goto end;
            
            a *= 10;
            a += current - 48;
        }

        // detect a number and a close parenthesis
        input >> current;
        for (int i = 0; current != ')'; i++, input >> current) {
            if (i >= 3 || current < 48 || current > 57)
                goto end;
            
            b *= 10;
            b += current - 48;
        }

        sum += a * b;

        end: continue;
    }

    cout << sum << endl;

    return 0;
}
