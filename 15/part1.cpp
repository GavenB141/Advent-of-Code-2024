#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "../lib/vec2.hpp"

using namespace std;

enum Tile {
    Empty,
    Wall,
    Box,
    Robot
};

typedef vector<vector<Tile>> warehouse_t;

// Attempt to move the given tile based on `dir`.
// return whether or not the move occurs
// write position back to `from`
bool attempt_move(warehouse_t& room, Vec2& from, char dir) {
    Vec2 next_pos;

    switch (room[from.y][from.x]) {
        case Wall:
            return false;
        case Empty:
            // don't actually move the empty space
            // just signal that others can move here
            return true;
        default:
            break;
    }

    switch (dir) {
        case '^':
            next_pos = from + Vec2{0, -1};
            break;
        case '>':
            next_pos = from + Vec2{1, 0};
            break;
        case 'v':
            next_pos = from + Vec2{0, 1};
            break;
        case '<':
            next_pos = from + Vec2{-1, 0};
            break;
        default:
            return false;
    }

    // use a copy here
    Vec2 next = next_pos;
    if (attempt_move(room, next, dir)) {
        room[next_pos.y][next_pos.x] = room[from.y][from.x];
        room[from.y][from.x] = Empty;

        // write back new position
        from = next_pos;
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    warehouse_t layout;
    Vec2 robot_pos;

    // Get initial layout
    string line;
    int y = 0;
    while(getline(input, line)) {
        // Leave the sequence for processing
        if (line == "")
            break;

        vector<Tile> row;

        istringstream iss(line);
        char c;

        int x = 0;
        while (iss >> c) {
            switch (c) {
                case '#':
                    row.push_back(Wall);
                    break;
                case 'O':
                    row.push_back(Box);
                    break;
                case '@':
                    robot_pos = Vec2{x, y};
                    row.push_back(Robot);
                    break;
                default:
                    row.push_back(Empty);
                    break;
            }
            x++;
        }

        layout.push_back(row);
        y++;
    }

    char move;
    while (input >> move) {
        attempt_move(layout, robot_pos, move);
    }

    int total = 0;
    for (int y = 0; y < layout.size(); y++) {
        for (int x = 0; x < layout[y].size(); x++) {
            if (layout[y][x] == Box) {
                total += y * 100 + x;
            }
        }
    }

    cout << total << endl;

    input.close();
    return 0;
}
