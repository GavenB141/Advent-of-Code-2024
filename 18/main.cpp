#include <fstream>
#include <iostream>
#include <vector>
#include <climits>
#include <set>
#include "../lib/vec2.hpp"

using namespace std;

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

int shortest_path (set<Vec2> const& corrupted, Vec2 start, Vec2 end) {
    set<Vec2> unvisited;
    vector<vector<int>> dist;

    static const Vec2 directions[] = {
        {1, 0}, {0, -1}, {-1, 0}, {0, 1}
    };

    // initialize
    for (int y = 0; y <= end.y; y++) {
        vector<int> row;
        for (int x = 0; x <= end.x; x++) {
            Vec2 current = Vec2{x, y};
            unvisited.insert(current);
            if (current == start)
                row.push_back(0);
            else
                row.push_back(INT_MAX);
        }
        dist.push_back(row);
    }

    // main loop
    while (!unvisited.empty()) {
        Vec2 node = get_min(dist, unvisited);
        if (node == Vec2{-1, -1})
            break;

        int distance = dist[node.y][node.x];

        if (node == end)
            return distance;

        if (distance == INT_MAX)
            break;

        for (Vec2 dir : directions) {
            Vec2 neighbor = node + dir;

            if (unvisited.find(neighbor) != unvisited.end()) {
                if (corrupted.find(neighbor) != corrupted.end()) {
                    unvisited.erase(neighbor);
                    continue;
                }

                int n_distance = dist[neighbor.y][neighbor.x];
                int candidate = distance + 1;
                if (candidate < n_distance) {
                    dist[neighbor.y][neighbor.x] = candidate;
                }
            }
        }

        unvisited.erase(node);
    }
    return -1;
}

int main(int argc, char* argv[]) {
    Vec2 current = {0,0};
    Vec2 end = {70,70};
    int steps = 1024;

    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        steps = 12;
        end = {6,6};
        input.open("sample.txt");
    }

    vector<Vec2> bytes;
    Vec2 line;
    char c;
    while (input >> line.x >> c >> line.y) {
        bytes.push_back(line);
    }
    

    // part 1
    /*set<Vec2> corrupted;*/
    /*for (int i = 0; i < steps; i++) {*/
    /*    corrupted.insert(bytes[i]);*/
    /*}*/
    /**/
    /*cout << shortest_path(corrupted, current, end) << endl;*/

    // part 2;
    // use a binary search
    int slice = 2;
    int i = bytes.size() / slice;
    while (true) {
        set<Vec2> corrupted;
        for (int j = 0; j < i; j++) {
            corrupted.insert(bytes[j]);
        }

        slice *= 2;
        if (shortest_path(corrupted, current, end) != -1) {
            corrupted.insert(bytes[i]);
            if (shortest_path(corrupted, current, end) == -1) {
                cout << bytes[i].x << ',' << bytes[i].y << endl;
                break;
            } else {
                i += bytes.size() / slice;
            }
        } else {
            i -= bytes.size() / slice;
        }
    }

    input.close();
    return 0;
}
