#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    cout << 0 << endl;

    input.close();
    return 0;
}
