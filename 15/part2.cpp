#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "../lib/vec2.hpp"

using namespace std;

enum Tile {
    Empty,
    Wall,
    LeftBox,
    RightBox,
    Robot
};

typedef vector<vector<Tile>> warehouse_t;

// Conduct a move is possible, return false if not
// Default params are a recursion detail that the caller should ignore
bool attempt_move(warehouse_t& room, Vec2& from, char dir, 
                  bool test = false, bool box_flag = false) {

    Tile tile = room[from.y][from.x];

    if (tile == Robot && !test) {
        // Run in test mode to ensure move is legal
        if (!attempt_move(room, from, dir, true)) {
            return false;
        }
    }

    switch (tile) {
        case Wall:
            return false;
        case Empty:
            return true;
        default:
            break;
    }

    Vec2 next_pos;
    bool vertical = false;
    switch (dir) {
        case '^':
            vertical = true;
            next_pos = from + Vec2{0, -1};
            break;
        case '>':
            next_pos = from + Vec2{1, 0};
            break;
        case 'v':
            vertical = true;
            next_pos = from + Vec2{0, 1};
            break;
        case '<':
            next_pos = from + Vec2{-1, 0};
            break;
        default:
            cout << endl;
            return false;
    }

    if ((tile != Robot) && vertical && !box_flag) {
        Vec2 other = from + Vec2{tile == LeftBox ? 1 : -1, 0};
        if (!attempt_move(room, other, dir, test, true)) {
            return false;
        }
    }

    // use a copy here
    Vec2 next = next_pos;
    if (attempt_move(room, next, dir, test)) {
        if (!test) {
            room[next_pos.y][next_pos.x] = tile;
            room[from.y][from.x] = Empty;

            // write back new position
            from = next_pos;
        }
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
                    row.push_back(Wall);
                    break;
                case 'O':
                    row.push_back(LeftBox);
                    row.push_back(RightBox);
                    break;
                case '@':
                    robot_pos = Vec2{x, y};
                    row.push_back(Robot);
                    row.push_back(Empty);
                    break;
                default:
                    row.push_back(Empty);
                    row.push_back(Empty);
                    break;
            }
            x += 2;
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
            if (layout[y][x] == LeftBox) {
                total += y * 100 + x;
            }
        }
    }

    cout << total << endl;

    input.close();
    return 0;
}
