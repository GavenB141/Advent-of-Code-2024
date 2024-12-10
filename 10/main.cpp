#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Vec2 { 
    int x; 
    int y;
};

inline Vec2 operator+(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x + rhs.x, lhs.y + rhs.y };
}

inline Vec2 operator-(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x - rhs.x, lhs.y - rhs.y };
}

inline bool operator==(Vec2 const& lhs, Vec2 const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

int calculate_score(
    vector<vector<int>> const& map, 
    Vec2 from/*,
    vector<Vec2>& found*/
) {
    if (
        from.y < 0 || from.x < 0 || 
        from.y >= map.size() || from.x >= map[from.y].size()
    ) return 0;

    int height = map[from.y][from.x];

    for (int i = 0; i < height; i++) {
        cout << " ";
    }
    cout << from.x << " " << from.y << " : " << height << endl;

    if (height == 9) {
        /*if (find(found.begin(), found.end(), from) == found.end()) {*/
        /*    found.push_back(from);*/
        return 1;
        /*}*/
        /*return 0;*/
    }

    int sum = 0;
    static const Vec2 directions[] = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}
    };

    for (auto dir : directions) {
        Vec2 next = from + dir;

        if (
            next.y < 0 || next.x < 0 || 
            next.y >= map.size() || next.x >= map[next.y].size()
        ) continue;

        int facing = map[next.y][next.x];
        if (facing == height + 1) {
            sum += calculate_score(map, next/*, found*/);
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

    vector<vector<int>> map;
    string line;
    while (getline(input, line)) {
        istringstream iss(line);
        vector<int> row;

        char current;
        while (iss >> current)
            row.push_back(current - '0');

        map.push_back(row);
    }

    int sum = 0;
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != 0) continue;

            // only needed for part 1
            // vector<Vec2> found = {};
            sum += calculate_score(map, {x, y}/*, found*/);
        }
    }

    cout << sum << endl;

    input.close();
    return 0;
}
