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

struct Path {
    vector<Vec2> tiles;
};

static const Vec2 directions[] = {
    {1, 0}, {0, -1}, {-1, 0}, {0, 1}
};

struct PathNode {
    Vec2 node;
    int distance;
    Vec2 facing;
};

typedef vector<PathNode> path_t;

path_t shortest_path (maze_t const& maze, Vec2 start, Vec2 facing = directions[0], int target = -1) {
    set<Vec2> unvisited;
    vector<vector<int>> dist;
    vector<vector<Vec2>> face;
    vector<vector<Vec2>> prev;

    // initialize
    for (int y = 0; y < maze.size(); y++) {
        vector<int> row;
        vector<Vec2> f_row;
        vector<Vec2> p_row;
        for (int x = 0; x < maze[y].size(); x++) {
            Vec2 current = Vec2{x, y};
            unvisited.insert(current);
            if (current == start)
                row.push_back(0);
            else
                row.push_back(INT_MAX);
            p_row.push_back(Vec2{-1, -1});
            f_row.push_back(facing);
        }
        dist.push_back(row);
        prev.push_back(p_row);
        face.push_back(f_row);
    }

    // main loop
    Vec2 node;
    while (!unvisited.empty()) {
        node = get_min(dist, unvisited);

        if (node == Vec2{-1, -1}) {
            // no more paths
            return {};
        }

        if (maze[node.y][node.x] == End)
            break;

        int distance = dist[node.y][node.x];

        // failed to find a short path, save time by returning early
        if (target != -1 && distance >= target) {
            return {};
        }

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
                    prev[neighbor.y][neighbor.x] = node;
                }
            }
        }

        unvisited.erase(node);
    }

    // retrace
    path_t path;
    while (true) {
        PathNode pn = PathNode { node, dist[node.y][node.x], face[node.y][node.x] };
        path.insert(path.begin(), pn);

        if (prev[node.y][node.x] == Vec2{-1, -1}) {
            break;
        }
        node = prev[node.y][node.x];
    }

    return path;
}

static vector<vector< vector<path_t> >> paths_memo; 

// recursively find all shortest paths
vector<path_t> all_paths(maze_t& maze, Vec2 start, Vec2 facing = directions[0], int target = -1, int depth = 0) {

    if (!paths_memo[start.y][start.x].empty())
        return paths_memo[start.y][start.x];

    path_t base_path = shortest_path(maze, start, facing, target);
    vector<path_t> output = {base_path};

    if (!base_path.empty() && target == -1) {
        target = base_path[base_path.size()-1].distance;
    } else if (base_path.empty() || base_path[base_path.size()-1].distance > target) {
        return {};
    }

    path_t traversed;
    int distance = 0;

    for (int i = 0; i < base_path.size() - 2; i++) {
        PathNode current = base_path[i];
        Vec2 next = base_path[i+1].node;
        Vec2 prev = i > 0 ? base_path[i-1].node : Vec2{-1, -1};

        cout << depth << ": " << i << " / " << base_path.size() - 2 << '\n';
        
        // count possible
        int count = 0;
        for (Vec2 const& dir : directions) {
            Vec2 candidate = current.node + dir;
            
            if (candidate == next)
                continue;
            if (candidate == prev)
                continue;
            if (maze[candidate.y][candidate.x] == Wall)
                continue;
            count++;
        }

        if (count) {
            // try blocking the next and previous moves
            for (PathNode const& node : traversed) {
                maze[node.node.y][node.node.x] = Wall;
            }

            maze[next.y][next.x] = Wall;
            vector<path_t> alts = all_paths(maze, current.node, current.facing, target - current.distance, depth + 1);
            maze[next.y][next.x] = Empty;

            for (PathNode const& node : traversed) {
                maze[node.node.y][node.node.x] = Empty;
            }

            // prepend existing path
            for (path_t& path : alts) {
                for (PathNode& node : path) {
                    node.distance += distance;
                }
                path.insert(path.begin(), traversed.begin(), traversed.end());
            }

            output.insert(output.end(), alts.begin(), alts.end());
        }

        distance += current.distance;
        traversed.push_back(current);
    }

    paths_memo[start.y][start.x] = output;
    return output;
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
        paths_memo.push_back({});

        istringstream iss(line);

        vector<Tile> row;
        char c;
        for (int x = 0; iss >> c; x++) {
            paths_memo[y].push_back({});
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

    // This is very slow
    // I could write a better algorithm
    // but I've got stuff to do so I'm just gonna let this churn for a while
    vector<path_t> paths = all_paths(maze, start);
    set<Vec2> nodes;

    for (path_t const& path : paths) {
        for (PathNode node : path) {
            nodes.insert(node.node);
        }
    }

    // display
    /*for (int y = 0; y < maze.size(); y++) {*/
    /*    for (int x = 0; x < maze[y].size(); x++) {*/
    /*        Vec2 current = Vec2{x, y};*/
    /**/
    /*        if (nodes.find(current) != nodes.end()) {*/
    /*            cout << 'O';*/
    /*        } else if (maze[y][x] == Wall) {*/
    /*            cout << '#';*/
    /*        } else {*/
    /*            cout << '.';*/
    /*        }*/
    /*    }*/
    /*    cout << '\n';*/
    /*}*/
    
    cout << nodes.size() << endl;

    input.close();
    return 0;
}
