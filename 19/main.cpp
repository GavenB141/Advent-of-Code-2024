#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

struct PatternMemo {
    vector<string> inputs;
    vector<long> outputs;

    void add (string pattern, long count) {
        inputs.push_back(pattern);
        outputs.push_back(count);
    }

    long search (string pattern) {
        auto idx = find(inputs.begin(), inputs.end(), pattern);

        if (idx != inputs.end()) {
            return outputs[idx - inputs.begin()];
        }
        return -1;
    }
};
static PatternMemo memo;

long patterns_possible(string pattern, set<string>& given) {
    long memoized = memo.search(pattern);
    if (memoized != -1) {
        return memoized;
    }

    long count = 0;
    if (given.find(pattern) != given.end()) {
        count++;
    } else if (pattern.length() <= 1) {
        return 0;
    }

    string left = pattern;
    string right = "";

    while (right.length() < pattern.length() - 1) {
        right.insert(right.begin(), left[left.size() - 1]);
        left.pop_back();

        long sub;
        if (given.find(right) != given.end() && (sub = patterns_possible(left, given))) {
            /*cout << '(' << pattern << ": " << left << ' ' << right << ")\n";*/
            count += sub;
        }
    }

    memo.add(pattern, count);
    return count;
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    set<string> towels;
    string line;

    // get patterns
    {
        getline(input, line);
        string current;
        for (char c : line) {
            if (c == ' ') continue;
            if (c == ',') {
                towels.insert(current);
                current = "";
                continue;
            }

            current.push_back(c);
        }
        towels.insert(current);
    }

    vector<string> patterns;
    while (getline(input, line)) {
        if (line.length() == 0) continue;

        patterns.push_back(line);
    }


    int possible_designs = 0;
    long total_possibilities = 0;
    for (string pattern : patterns) {
        long possible = patterns_possible(pattern, towels);
        if (possible) {
            possible_designs++;
            total_possibilities += possible;
        }
    }

    cout << "Possible designs: " << possible_designs << endl;
    cout << "Total variations: " << total_possibilities << endl;

    input.close();
    return 0;
}
