#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "../lib/vec2.hpp"

using namespace std;

typedef vector<vector<char>> charmap_t;

inline bool valid_space (charmap_t const& map, Vec2 pos) {
    return !(
        pos.x < 0 ||
        pos.y < 0 ||
        pos.y >= map.size() ||
        pos.x >= map[pos.y].size()
    );
}

// these are for counting vertices in part 2
static const Vec2 grid2edge[] = {
    {-1, -1}, {-1, 0}, {0, 0}, {0, -1}
};
static const Vec2 edge2grid[] = {
    {0, 0}, {1, 0}, {1, 1}, {0, 1}
};

// Used to determine whether the bottom-right corner of `from`
// should be considered a vertex relative to `target`.
// Counts the number of matching squares around it.
// Usually, an odd count means yes and an even count means no.
// `5` is used to indicate the exception of two cornering squares
int count_in_square (charmap_t const& map, Vec2 from, char target) {
    int count = 0;

    vector<char> counted_indices;
    
    for (int i = 0; i < 4; i++) {
        Vec2 pos = from + edge2grid[i];

        if (valid_space(map, pos) && map[pos.y][pos.x] == target) {
            counted_indices.push_back(i);
            count += 1;
        }
    }

    // spoof count if corners were counted
    if (
        counted_indices.size() == 2 
        && counted_indices[1] - counted_indices[0] == 2
    ) return 5;

    return count;
}

static const Vec2 directions[] = {
    {0, 1}, {1, 0}, {0, -1}, {-1, 0}
};

// returns {area, perimeter}
// writes to `vertices`
// ignores spaces in `used`
Vec2 explore_region (
    charmap_t const& map,
    Vec2 from,
    vector<Vec2>& used,
    vector<Vec2>& vertices // part 2
) {
    Vec2 info = {0, 0};

    if (find(used.begin(), used.end(), from) != used.end()) {
        return info;
    }

    used.push_back(from);
    info.x += 1;

    // find vertices for part 2
    for (Vec2 dir : grid2edge) {
        Vec2 cand = from + dir;

        int count = count_in_square(map, cand, map[from.y][from.x]);
        if (count % 2 == 1) {
            if (
                find(vertices.begin(), vertices.end(), cand) == vertices.end()
                || count == 5 // allow catty-corner case
            ) {
                vertices.push_back(cand);
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        Vec2 neighbor = from + directions[i];

        if (
            !valid_space(map, neighbor) ||
            map[neighbor.y][neighbor.x] != map[from.y][from.x]
        ) {
            info.y++;
            continue;
        }

        info = info + explore_region(map, neighbor, used, vertices);
    }

    return info;
}

int fencing_price (charmap_t const& map) {
    vector<Vec2> used;

    int total = 0;
    for (Vec2 pos = {0, 0}; pos.y < map.size(); pos.y++) {
        for (pos.x = 0; pos.x < map[pos.y].size(); pos.x++) {
            if (find(used.begin(), used.end(), pos) != used.end()) {
                continue;
            }
            vector<Vec2> vertices; // part 2
            Vec2 stats = explore_region(map, pos, used, vertices);
            // total += stats.x * stats.y;
            total += stats.x * vertices.size();
        }
    }
    return total;
} 

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    charmap_t farm_map;
    string line;
    while (getline(input, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        farm_map.push_back(row);
    }

    cout << fencing_price(farm_map) << endl;

    input.close();
    return 0;
}
