#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct Vec2 { 
    int x; 
    int y;
};

inline Vec2 operator+(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x + rhs.x, lhs.y + lhs.y };
}

inline Vec2 operator-(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x - rhs.x, lhs.y - rhs.y };
}

inline bool operator==(Vec2 const& lhs, Vec2 const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    // indexed by chars representing frequencies
    vector<Vec2> antennas[124];

    // parse map
    string line;
    Vec2 location = {0, 0};
    while(getline(input, line)) {
        istringstream iss(line);

        location.x = 0;
        char current;
        while(iss >> current) {
            if (current != '.') {
                antennas[current].push_back(location);
            }
            location.x++;
        }
        location.y++;
    }

    // find antinodes
    vector<Vec2> antinodes;
    for (vector<Vec2>& freq : antennas) {
        if (freq.size() == 0)
            continue;

        for (int i = 0; i < freq.size(); i++) {
            for (int j = 0; j < freq.size(); j++) {
                if (freq[i] == freq[j]) continue;

                Vec2 distance = freq[j] - freq[i];

                // part 1
                /*Vec2 pos = freq[i] - distance;*/

                // part 2 loops (both parts run this block)
                for (Vec2 pos = freq[i];; pos = pos - distance) {
                    if (
                        pos.y < 0 || pos.y >= location.y || 
                        pos.x < 0 || pos.x >= location.x
                    ) break; // continue for part 1

                    if (
                        find(antinodes.begin(), antinodes.end(), pos)
                        != antinodes.end()
                    ) continue;

                    antinodes.push_back(pos);
                }
            }
        }
    }

    cout << antinodes.size() << endl;

    input.close();
    return 0;
}
