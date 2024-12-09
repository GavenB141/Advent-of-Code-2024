#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void rearrange_disk(vector<int>& disk) {
    int l = 0, r = disk.size() - 1;

    while (true) {
        while (disk[l] != -1) 
            l++;
        while (disk[r] == -1)
            r--;
        if (r <= l)
            break;

        iter_swap(disk.begin() + l, disk.begin() + r);
    }
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    // parse input
    vector<int> layout;  
    char current;
    bool is_file = true;
    int next_id = 0;
    while (input >> current) {
        int id = -1; // -1 = free space
        if (is_file) {
            id = next_id++;
        }
        for (int i = 0; i < current - 48; i++) {
            layout.push_back(id);
        }
        is_file = !is_file;
    }

    rearrange_disk(layout);

    uint64_t sum = 0;
    for (int i = 0; i < layout.size(); i++) {
        if (layout[i] == -1) {
            continue;
        }
        sum += layout[i] * i;
    }

    cout << sum << endl;
    
    input.close();
    return 0;
}
