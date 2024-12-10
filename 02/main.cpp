#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// For part 1
bool validate_report(const string& line) {
    istringstream report(line);

    int a, b;
    report >> a >> b;

    bool increasing = b > a;

    do {
        if (a == b) return false;

        bool increase = b > a;
        if (increase != increasing) return false;

        if (abs(a - b) > 3) return false;

        a = b;
    } while (report >> b);

    return true;
}

// For part 2
bool validate_report_dampened(string& report) {
    if (validate_report(report)) {
        return true;
    }

    // Try removing each number until a safe version is found
    for(int i = 0; i < report.length(); i++) {
        string removed = "";

        if (report[i] != ' ') {
            // Remove the number
            while (i < report.length() && report[i] != ' ') {
                removed.push_back(report[i]);
                report.erase(i, 1);
            }

            bool result = validate_report(report);

            // Restore the number to the list
            while (removed.size() > 0) {
                report.insert(i, 1, removed[0]);
                removed.erase(0, 1);
                i++;
            }

            if (result) return true;
        }
    }

    // No safe version was found
    return false;
}

int main (int argc, char *argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    int safe_count = 0;
    string line;
    while (getline(input, line)) {
        if (validate_report_dampened(line)) {
            safe_count += 1;
        }
    }

    cout << safe_count << endl;

    input.close();
    return 0;
}
