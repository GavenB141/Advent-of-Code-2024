#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

direction next_direction(direction dir) {
    if (dir == LEFT) {
        return UP;
    }
    return (direction)(dir + 1);
}

void apply_direction(direction dir, int *x, int *y) {
    switch (dir) {
        case UP: *y -= 1; break;
        case RIGHT: *x += 1; break;
        case DOWN: *y += 1; break;
        case LEFT: *x -= 1; break;
    }
}

// updated for part 2 to return -1 when a loop is detected
int traverse_map(vector<vector<char>> map, int pos[2]) {
    direction facing = UP;
    int sum = 1;
    int x = pos[0], y = pos[1];

    while (true) {
        int nx = x, ny = y;
        apply_direction(facing, &nx, &ny);

        if (ny < 0 || ny >= map.size() || nx < 0 || nx >= map[ny].size()) {
            break;
        }

        if (map[ny][nx] == '#') {
            facing = next_direction(facing);
            continue;
        } else {
            if (map[ny][nx] == '.') {
                map[ny][nx] = 0b0;
                sum++;
            } else if (map[ny][nx] & (1 << facing)) {
                //deja vu
                return -1;
            }

            // write bit to signify direction traversed
            map[ny][nx] |= (1 << facing);
        }

        x = nx;
        y = ny;
    }

    return sum;
}

// part 2 version
int place_obstacles(vector<vector<char>> map, int pos[2]) {
    direction facing = UP;
    int sum = 0;
    int x = pos[0], y = pos[1];
    
    auto copy = map;

    while (true) {
        int nx = x, ny = y;
        apply_direction(facing, &nx, &ny);

        if (ny < 0 || ny >= map.size() || nx < 0 || nx >= map[ny].size()) {
            break;
        }

        if (map[ny][nx] == '#') {
            facing = next_direction(facing);
            continue;
        } else {
            if (map[ny][nx] == '.') {
                map[ny][nx] = 0b0;

                copy[ny][nx] = '#';
                if (traverse_map(copy, pos) == -1) {
                    sum += 1;
                }
                copy[ny][nx] = '.';
            }

            // write bit to signify direction traversed
            map[ny][nx] |= (1 << facing);
        }

        x = nx;
        y = ny;
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

    // parse map
    vector<vector<char>> map;
    int pos[2];

    string line;
    int y = 0; 
    while (getline(input, line)) {
        istringstream stream(line);

        vector<char> row;
        int x = 0;
        char current;
        while (stream >> current) {
            switch (current) {
                case '.':
                case '#':
                    row.push_back(current);
                    break;
                case '^':
                    //row.push_back('X'); part 1
                    row.push_back(0b1);
                    pos[0] = x;
                    pos[1] = y;
                    break;
            }

            x++;
        }
        map.push_back(row);
        y++;
    }

    // part 1
    // int sum = traverse_map(map, pos);

    // part 2
    int sum = place_obstacles(map, pos);

    cout << sum << endl;

    input.close();
    return 0;
}
