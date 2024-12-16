#include <climits>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "../lib/vec2.hpp"

using namespace std;

enum Tile {
    Empty,
    Wall,
    End
};

typedef vector<vector<Tile>> maze_t;

// get the shortest distance node that is in the given set
Vec2 get_min(vector<vector<int>> const& map, set<Vec2> const& s) {
    Vec2 min = {-1, -1};
    int min_value = INT_MAX;

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (s.find(Vec2{x, y}) == s.end()) {
                continue;
            }

            if (map[y][x] < min_value) {
                min_value = map[y][x];
                min = Vec2{x, y};
            }
        }
    }

    return min;
}

int shortest_path (maze_t const& maze, Vec2 start) {
    set<Vec2> unvisited;
    vector<vector<int>> dist;
    vector<vector<Vec2>> face;

    static const Vec2 directions[] = {
        {1, 0}, {0, -1}, {-1, 0}, {0, 1}
    };

    // initialize
    for (int y = 0; y < maze.size(); y++) {
        vector<int> row;
        vector<Vec2> f_row;
        for (int x = 0; x < maze[y].size(); x++) {
            Vec2 current = Vec2{x, y};
            unvisited.insert(current);
            if (current == start)
                row.push_back(0);
            else
                row.push_back(INT_MAX);
            f_row.push_back(Vec2{1, 0});
        }
        dist.push_back(row);
        face.push_back(f_row);
    }

    // main loop
    while (!unvisited.empty()) {
        Vec2 node = get_min(dist, unvisited);
        int distance = dist[node.y][node.x];

        if (maze[node.y][node.x] == End)
            return distance;

        if (distance == INT_MAX)
            break;

        for (Vec2 dir : directions) {
            Vec2 neighbor = node + dir;

            if (unvisited.find(neighbor) != unvisited.end()) {
                if (maze[neighbor.y][neighbor.x] == Wall) {
                    unvisited.erase(neighbor);
                    continue;
                }

                int n_distance = dist[neighbor.y][neighbor.x];

                int candidate = distance + 1;
                if (!(dir == face[node.y][node.x])) {
                    candidate += 1000;
                }

                if (candidate < n_distance) {
                    dist[neighbor.y][neighbor.x] = candidate;
                    face[neighbor.y][neighbor.x] = dir;
                }
            }
        }

        unvisited.erase(node);
    }
    return -1;
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    maze_t maze;
    Vec2 start;

    string line;
    for (int y = 0; getline(input, line); y++) {
        istringstream iss(line);

        vector<Tile> row;
        char c;
        for (int x = 0; iss >> c; x++) {
            switch (c) {
                case '.':
                    row.push_back(Empty);
                    break;
                case '#':
                    row.push_back(Wall);
                    break;
                case 'S':
                    row.push_back(Empty);
                    start = Vec2{x, y};
                    break;
                case 'E':
                    row.push_back(End);
                    break;
            }
        }
        maze.push_back(row);
    }

    cout << shortest_path(maze, start) << endl;

    input.close();
    return 0;
}
