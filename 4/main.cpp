#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// for part 1
int count_occurrences_from(int x, int y, const vector<vector<char>>& map, string target) {
    static const int directions[8][2] = {
        {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}
    };

    int sum = 0;

    if (map[y][x] == target[0]) {
        for (int i = 0; i < 8; i++) {
            int tx = x, ty = y;

            for (char c : target) {
                if (
                    (tx < 0) || (ty < 0) || 
                    (ty >= map.size()) || (tx >= map[ty].size()) ||
                    (map[ty][tx] != c)
                ) {
                    goto not_found;
                }
                tx += directions[i][0];
                ty += directions[i][1];
            }

            sum += 1;

            not_found: continue;
        }
    }

    return sum;
}

// for part 2
int count_x_mas (const vector<vector<char>>& map) {
    int sum = 0;
    static const char ends[2] = {'M', 'S'};

    for (int y = 1; y < map.size() - 1; y++) {
        for (int x = 1; x < map[y].size() - 1; x++) {
            if (map[y][x] != 'A') continue;

            char diags[2][2] = {
                {map[y - 1][x - 1], map[y + 1][x + 1]}, 
                {map[y - 1][x + 1], map[y + 1][x - 1]}
            };

            for (int i = 0; i < 2; i++) {
                for (char c : ends) {
                    if (find(begin(diags[i]), end(diags[i]), c) == end(diags[i])) {
                        goto not_found;
                    }
                }
            }

            sum += 1;

            not_found: continue;
        }
    }

    return sum;
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    vector<vector<char>> puzzle;
    string line;
    while (getline(input, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        puzzle.push_back(row);
    }

    /* part 1
    int sum = 0;
    for (int y = 0; y < puzzle.size(); y++) {
        for (int x = 0; x < puzzle[y].size(); x++) {
            sum += count_occurrences_from(x, y, puzzle, "XMAS");
        }
    }
    */

    cout << count_x_mas(puzzle) << endl;

    input.close();
    return 0;
}
