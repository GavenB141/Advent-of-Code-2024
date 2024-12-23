#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

// for part 1
set<set<int>> get_three_ways(vector<vector<bool>> const& conns) {
    set<set<int>> out;

    for (int i = 0; i < conns.size(); i++) {
        for (int j = i + 1; j < conns.size(); j++) {
            for (int k = j + 1; k < conns.size(); k++) {
                if (conns[i][j] && conns[j][k] && conns[i][k]) {
                    out.insert({i, j, k});
                }
            }
        }
    }

    return out;
}

bool is_clique (vector<vector<bool>> const& conns, set<int> const& subgraph) {
    for (int i : subgraph) {
        for (int j : subgraph) {
            if (i == j) 
                continue;
            if (!conns[i][j]) 
                return false;
        }
    }
    return true;
}

// part 2
set<int> largest_network(vector<vector<bool>> const& conns, set<int> net = {}, int from = 0) {
    set<int> largest = {};

    for (int i = from; i < conns.size(); i++) {
        net.insert(i);

        if (is_clique(conns, net)) {
            if (net.size() > largest.size()) {
                largest = net;
            }

            set<int> candidate = largest_network(conns, net, i+1);

            if (candidate.size() > largest.size()) {
                largest = candidate;
            }
        }

        net.erase(i);
    }

    return largest;
}

int main(int argc, char* argv[]) {
    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("sample.txt");
    }

    vector<string> id;
    set<int> t_coms; // indices of part 1 targets
    vector<vector<bool>> connections;
    
    string line;
    while (getline(input, line)) {
        string lhs(line.begin(), line.begin()+2);
        string rhs(line.begin()+3, line.end());

        auto it = find(id.begin(), id.end(), lhs);
        int il = distance(id.begin(), it);
        if (it == id.end()) {
            if (lhs[0] == 't') {
                t_coms.insert(id.size());
            }
            id.push_back(lhs);
            connections.push_back(vector(connections.size(), false));
            for (vector<bool>& v : connections) {
                v.push_back(false);
            }
        }

        it = find(id.begin(), id.end(), rhs);
        int ir = distance(id.begin(), it);
        if (it == id.end()) {
            if (rhs[0] == 't') {
                t_coms.insert(id.size());
            }
            id.push_back(rhs);
            connections.push_back(vector(connections.size(), false));
            for (vector<bool>& v : connections) {
                v.push_back(false);
            }
        }
        
        connections[il][ir] = true;
        connections[ir][il] = true;
    }

    // part 1
    int total = 0;
    auto three_ways = get_three_ways(connections);
    for (set<int> const& conn : three_ways) {
        for (int id : conn) {
            if (t_coms.find(id) != t_coms.end()) {
                total += 1;
                break;
            }
        }
    }
    cout << "Part 1: " << total << endl;

    // part 2
    set<int> party = largest_network(connections);
    set<string> computers;
    for (int i : party) {
        computers.insert(id[i]);
    }

    // sets are sorted by default
    ostringstream oss;
    for (string s : computers) {
        oss << s << ',';
    }
    string password = oss.str();
    password.pop_back();
    cout << "Part 2: " << password << endl;

    input.close();
    return 0;
}
