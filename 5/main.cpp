#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int indexof(vector<int> list, short target) {
    auto it = find(list.begin(), list.end(), target);
    if (it == list.end()) return -1;
    return distance(list.begin(), it);
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    // establish rules
    vector<int> rules[100];
    string line;
    while (getline(input, line)) {
        // read lines until blank separator line
        if (line.length() == 0) break;

        istringstream rule(line);

        int predecessor, follower;
        char _;
        rule >> predecessor >> _ >> follower;

        rules[predecessor].push_back(follower);
    }

    int sum = 0;

    // verify updates
    while (getline(input, line)) {
        istringstream update(line);

        vector<int> used;
        int current;
        bool corrected = false; // part 2 flag

        while (!update.eof()) {
            update >> current;

            if (update.peek() == ',') {
                char _;
                update >> _;
            }

            used.push_back(current);
        }

        for (int c_ind = 0; c_ind < used.size(); c_ind++) {
            int current = used[c_ind];

            for (int i = 0; i < rules[current].size(); i++) {
                int f_ind = indexof(used, rules[current][i]);
                if (f_ind != -1 && f_ind < c_ind) {
                    // Part 1 logic
                    // goto finish;
                    
                    // Part 2 logic
                    corrected = true;

                    int temp = used[c_ind];
                    used[c_ind] = used[f_ind];
                    used[f_ind] = temp;                    

                    // go back to correct new errors
                    c_ind = f_ind;
                    i = 0;
                }
            }
        }

        // remove conditional for part 1
        if (corrected) {
            sum += used[(used.size()-1)/2];
        }

        finish: continue;
    }

    cout << sum << endl;

    input.close();
    return 0;
}
