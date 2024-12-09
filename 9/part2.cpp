#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct FileBlocks {
    int id; // -1 = free space
    int size;
};

void rearrange_disk(vector<FileBlocks>& disk) {
    for (int i = disk.size() - 1; i >= 0; i--) {
        FileBlocks* file = &disk[i];

        if (file->id == -1)
            continue;

        for (int j = 0; j < i; j++) {
            FileBlocks* empty = &disk[j];

            if (empty->id != -1) 
                continue;

            // combine empty space if applicable
            while (disk[j+1].id == -1) {
                empty->size += disk[j+1].size; 
                disk.erase(disk.begin() + j + 1);
            }

            int size_diff = empty->size - file->size;

            if (size_diff >= 0) {
                empty->id = file->id;
                file->id = -1;

                if (size_diff) {
                    empty->size -= size_diff;
                    disk.insert(disk.begin() + j + 1, {-1, size_diff});
                }

                break;
            }
        }

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
    vector<FileBlocks> layout;  
    char current;
    bool is_file = true;
    int next_id = 0;
    while (input >> current) {
        int id = -1;
        if (is_file) {
            id = next_id++;
        }

        layout.push_back({id, current - 48});

        is_file = !is_file;
    }

    rearrange_disk(layout);

    int blocks = 0;
    uint64_t sum = 0;
    for (int i = 0; i < layout.size(); i++) {
        if (layout[i].id == -1) {
            blocks += layout[i].size;
            continue;
        }
        
        for (int j = 0; j < layout[i].size; j++) {
            sum += layout[i].id * blocks++;
        }
    }
    cout << sum << endl;
    
    input.close();
    return 0;
}
