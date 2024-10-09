//MIHAI ALEXANDRU STEFANOIU 
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm> 
#include <climits>  
#include <iomanip> 


using namespace std;

const int SENTINEL = numeric_limits<int>::max();

int mcp_naive(vector<vector<int>>& grid, int i, int j) {
    if (i == 0 && j == 0) {
        return grid[i][j];
    }

    if (i < 0 || j < 0) {
        return SENTINEL;
    }
    int izq = mcp_naive(grid, i, j - 1);
    int arribaIzq = mcp_naive(grid, i - 1, j - 1);
    int arriba = mcp_naive(grid, i - 1, j);

    int minimo = min(izq, min(arribaIzq, arriba));

    return grid[i][j] + minimo;
}

int mcp_memo(vector<vector<int>>& grid, int i, int j, vector<vector<int>>& memo) {
    if (i == 0 && j == 0) {
        return grid[i][j];
    }

    if (i < 0 || j < 0) {
        return SENTINEL;
    }

    if (memo[i][j] != SENTINEL) {
        return memo[i][j];
    }

    int izq = mcp_memo(grid, i, j - 1, memo);
    int arribaIzq = mcp_memo(grid, i - 1, j - 1, memo);
    int arriba = mcp_memo(grid, i - 1, j, memo);

    int minimo = min(izq, min(arribaIzq, arriba));

    int result = grid[i][j] + minimo;
    memo[i][j] = result;

    return result;
}

int mcp_it_matrix(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<int>> tabla(n, vector<int>(m, SENTINEL));

    tabla[0][0] = grid[0][0];

    for (int i = 1; i < n; i++) {
        tabla[i][0] = tabla[i - 1][0] + grid[i][0];
    }
    for (int j = 1; j < m; j++) {
        tabla[0][j] = tabla[0][j - 1] + grid[0][j];
    }

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            tabla[i][j] = grid[i][j] + min(tabla[i - 1][j], min(tabla[i][j - 1], tabla[i - 1][j - 1]));
        }
    }

    return tabla[n - 1][m - 1];
}


int mcp_it_vector(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    vector<int> fila_actual(m, SENTINEL);
    vector<int> fila_anterior(m, SENTINEL);

    fila_anterior[0] = grid[0][0];

    for (int j = 1; j < m; j++) {
        fila_anterior[j] = fila_anterior[j - 1] + grid[0][j];
    }

    for (int i = 1; i < n; i++) {
        fila_actual = fila_anterior;

        fila_actual[0] += grid[i][0];

        for (int j = 1; j < m; j++) {
            fila_actual[j] = grid[i][j] + min(fila_actual[j - 1], min(fila_anterior[j], fila_anterior[j - 1]));
        }

        fila_anterior = fila_actual;
    }

    return fila_anterior[m - 1];
}

int readMapFromFile(const string& filename, vector<vector<int>>& map) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo " << filename << endl;
        return -1;
    }
    int n, m;
    file >> n >> m;

    map.resize(n, vector<int>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            file >> map[i][j];
        }
    }

    file.close();

    return 0;
}


void mcp_parser(const vector<vector<int>>& map, const vector<vector<int>>& memo) {
    
    vector<vector<char>> camino(map.size(), vector<char>(map[0].size(), '.'));

    int i = map.size() - 1;
    int j = map[0].size() - 1;
    while (i > 0 || j > 0) {
        camino[i][j] = 'x';
        int vec_min = INT_MAX;

        if (i > 0) {
            vec_min = memo[i - 1][j];
            if (j > 0) {
                vec_min = min(vec_min, memo[i - 1][j - 1]);
            }
        }

        if (j > 0) {
            vec_min = min(vec_min, memo[i][j - 1]);
        }

        if (i > 0 && memo[i - 1][j] == vec_min) {
            i--;
        } else if (j > 0 && memo[i][j - 1] == vec_min) {
            j--;
        } else {
            i--;
            j--;
        }
    }
    camino[0][0] = 'x';

    for (const auto& row : camino) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }

    int dif = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (camino[i][j] == 'x') {
                dif += map[i][j];
            }
        }
    }

    cout << dif << endl;
}





int main(int argc, char* argv[]) {
    bool perform_transformation = false;
    bool parse = false;
    bool ignore_naive = false;
    string file_name;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-t") {
            perform_transformation = true;
        } else if (arg == "--p2D") {
            parse = true;
        } else if (arg == "--ignore-naive") {
            ignore_naive = true;
        } else if (arg == "-f") {
            i++;
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

    

    

    vector<vector<int>> memo(map.size(), vector<int>(map[0].size(), SENTINEL));
    int result_memoization = mcp_memo(map, map.size() - 1, map[0].size() - 1, memo);
int result_iterative = mcp_it_matrix(map);

int result_iterative_optimized = mcp_it_vector(map);


    if (ignore_naive) {
        cout << "- " <<result_memoization << " " <<  result_iterative << " " << result_iterative_optimized <<endl ;
    } else {
        int result_naive = mcp_naive(map, map.size() - 1, map[0].size() - 1);
        cout << result_naive << " " << result_memoization << " " <<  result_iterative << " " << result_iterative_optimized << endl;
    }

    if (parse) {
        mcp_parser(map, memo);
    }

    if (perform_transformation) {
    vector<vector<int>> tabla(map.size(), vector<int>(map[0].size(), SENTINEL));
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (i == 0 && j == 0) {
                tabla[i][j] = map[i][j];
            } else if (i == 0) {
                tabla[i][j] = tabla[i][j - 1] + map[i][j];
            } else if (j == 0) {
                tabla[i][j] = tabla[i - 1][j] + map[i][j];
            } else {
                tabla[i][j] = map[i][j] + min(tabla[i - 1][j], min(tabla[i][j - 1], tabla[i - 1][j - 1]));
            }
            cout << setw(2) << tabla[i][j] << ' ';
        }
        cout << endl;
    }
}


    return 0;
}
