#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "../lib/vec2.hpp"

using namespace std;

enum Quadrant {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    None
};

struct Robot {
    Vec2 pos;
    Vec2 vel;

    // part 1 functionality
    Quadrant quadrant(Vec2 const& room) const {
        Vec2 middle = {(room.x - 1) / 2, (room.y - 1) / 2};
        
        if (pos.x < middle.x && pos.y < middle.y)
            return TopLeft;
        if (pos.x > middle.x && pos.y < middle.y)
            return TopRight;
        if (pos.x < middle.x && pos.y > middle.y)
            return BottomLeft;
        if (pos.x > middle.x && pos.y > middle.y)
            return BottomRight;
        return None;
    }
};

void pass_seconds(vector<Robot>& bots, Vec2 const& room, int seconds) {
    for (Robot& bot : bots) {
        bot.pos = bot.pos + (bot.vel * Vec2{seconds, seconds});

        while (bot.pos.x < 0)
            bot.pos.x += room.x;
        while (bot.pos.x >= room.x)
            bot.pos.x -= room.x;
        while (bot.pos.y < 0)
            bot.pos.y += room.y;
        while (bot.pos.y >= room.y)
            bot.pos.y -= room.y;
    }
}

// part two functionality
// returns the number of rows found that form the top of a christmas tree
int query_xmas_tree(vector<Robot> const& bots, Vec2 const& room) {
    // get occupied squares
    vector<Vec2> occ;
    for (Robot const& bot : bots) {
        occ.push_back(bot.pos);
    }

    // find pattern
    int max = 0;
    for (Vec2 const& bot : occ) {
        Vec2 left = bot;
        Vec2 right = bot;

        int i = 1;
        while (true) {
            left = left + Vec2{-1, 1};
            right = right + Vec2{1, 1};

            if (find(occ.begin(), occ.end(), left) == occ.end()
                || find(occ.begin(), occ.end(), right) == occ.end()) {
                break;
            }

            i++;
        }

        if (i > max) {
            max = i;
        }
    }

    return max;
}

int main(int argc, char* argv[]) {
    Vec2 room = {101, 103};
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        room = {11, 7};
        input.open("sample.txt");
    }

    vector<Robot> robots;
    
    // read file
    string line;
    while (getline(input, line)) {
        istringstream iss(line);

        Robot r;

        char c;
        iss >> c >> c >> r.pos.x
            >> c >> r.pos.y
            >> c >> c >> r.vel.x
            >> c >> r.vel.y;

        robots.push_back(r);
    }

    int elapsed = 0;

    int max_time = -1;
    int max_matches = 0;
    int rows_matched = query_xmas_tree(robots, room);
    while (elapsed < room.x * room.y) {
        pass_seconds(robots, room, 1);
        elapsed++;
        rows_matched = query_xmas_tree(robots, room);

        if (rows_matched > max_matches) {
            max_matches = rows_matched;
            max_time = elapsed;
        }
    }
    cout << max_time << endl;

    // part 1
    /*pass_seconds(robots, room, 100);*/
    /**/
    /*int a = 0, b = 0, c = 0, d = 0;*/
    /*for (Robot const& bot : robots) {*/
    /*    switch(bot.quadrant(room)) {*/
    /*        case TopLeft: a++; break;*/
    /*        case TopRight: b++; break;*/
    /*        case BottomLeft: c++; break;*/
    /*        case BottomRight: d++; break;*/
    /*        case None: break;*/
    /*    }*/
    /*}*/
    /**/
    /*cout << a * b * c * d << endl;*/

    input.close();
    return 0;
}
