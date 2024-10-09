#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <limits>
#include <iomanip>
#include <climits>

using namespace std;

const int SENTINEL = numeric_limits<int>::max();

int readMapFromFile(const string& filename, vector<vector<int>>& map) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo " << filename << endl;
        return -1;
    }
    int n, m;
    file >> n >> m;

    map.resize(n, vector<int>(m));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            file >> map[i][j];
        }
    }

    file.close();

    return 0;
}

pair<int, int> mcp_greedy(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();

    // Dynamic programming table for forward traversal
    vector<vector<int>> dp_fwd(n, vector<int>(m, SENTINEL));
    dp_fwd[0][0] = grid[0][0];

    // Fill the dynamic programming table for forward traversal
    for (int i = 1; i < n; ++i) {
        dp_fwd[i][0] = dp_fwd[i - 1][0] + grid[i][0];
    }
    for (int j = 1; j < m; ++j) {
        dp_fwd[0][j] = dp_fwd[0][j - 1] + grid[0][j];
    }
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            dp_fwd[i][j] = min({dp_fwd[i - 1][j], dp_fwd[i][j - 1], dp_fwd[i - 1][j - 1]}) + grid[i][j];
        }
    }

    // Dynamic programming table for backward traversal
    vector<vector<int>> dp_bwd(n, vector<int>(m, SENTINEL));
    dp_bwd[n - 1][m - 1] = grid[n - 1][m - 1];
    for (int i = n - 2; i >= 0; --i) {
        dp_bwd[i][m - 1] = dp_bwd[i + 1][m - 1] + grid[i][m - 1];
    }
    for (int j = m - 2; j >= 0; --j) {
        dp_bwd[n - 1][j] = dp_bwd[n - 1][j + 1] + grid[n - 1][j];
    }
    for (int i = n - 2; i >= 0; --i) {
        for (int j = m - 2; j >= 0; --j) {
            dp_bwd[i][j] = min({dp_bwd[i + 1][j], dp_bwd[i][j + 1], dp_bwd[i + 1][j + 1]}) + grid[i][j];
        }
    }

    // Return the minimum cost obtained from forward and backward traversal
    return {dp_fwd[n - 1][m - 1], dp_bwd[0][0]};
}

void printPath(const vector<vector<int>>& grid, const vector<vector<int>>& memo) {
    vector<vector<char>> path(grid.size(), vector<char>(grid[0].size(), '.'));
    int i = grid.size() - 1;
    int j = grid[0].size() - 1;

    while (i > 0 || j > 0) {
        path[i][j] = 'x';
        int min_neighbor = INT_MAX;

        if (i > 0) {
            min_neighbor = memo[i - 1][j];
            if (j > 0) {
                min_neighbor = min(min_neighbor, memo[i - 1][j - 1]);
            }
        }

        if (j > 0) {
            min_neighbor = min(min_neighbor, memo[i][j - 1]);
        }

        // Consider the diagonal movement
        if (i > 0 && j > 0) {
            min_neighbor = min(min_neighbor, memo[i - 1][j - 1]);
        }

        // Update the coordinates according to the direction of the chosen movement
        if (i > 0 && memo[i - 1][j] == min_neighbor) {
            i--;
        } else if (j > 0 && memo[i][j - 1] == min_neighbor) {
            j--;
        } else {
            i--;
            j--;
        }
    }
    path[0][0] = 'x';

    for (const auto& row : path) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }

    int difficulty = 0;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {
            if (path[i][j] == 'x') {
                difficulty += grid[i][j];
            }
        }
    }

    cout << difficulty << endl;
}

int main(int argc, char* argv[]) {
    bool parse = false;
    string file_name;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--p2D") {
            parse = true;
        } else if (arg == "-f") {
            ++i;
            if (i >= argc) {
                cerr << "ERROR: Can't read file name." << endl;
                exit(EXIT_FAILURE);
            }
            file_name = argv[i];
        } else {
            cerr << "ERROR: Unknown option '" << arg << "'." << endl;
            exit(EXIT_FAILURE);
        }
    }
    if (file_name.empty()) {
        cerr << "ERROR: Missing file name." << endl;
        exit(EXIT_FAILURE);
    }
    ifstream input_file(file_name);
    if (!input_file.is_open()) {
        cerr << "ERROR: Can't open file: " << file_name << endl;
        exit(EXIT_FAILURE);
    }

    vector<vector<int>> map;
    int read_result = readMapFromFile(file_name, map);
    if (read_result != 0) {
        cerr << "ERROR: Failed to read map from file." << endl;
        exit(EXIT_FAILURE);
    }

    pair<int, int> result_greedy = mcp_greedy(map);

    cout << result_greedy.first << " " << result_greedy.second << endl;

    if (parse) {
        if (result_greedy.first <= result_greedy.second) {
            cout << "Forward Path:" << endl;
            printPath(map, map); // Use the same map twice to show the path obtained from the first direction
        } else {
            cout << "Backward Path:" << endl;
            printPath(map, map); // Use the same map twice to show the path obtained from the first direction
        }
    }

    return 0;
}
