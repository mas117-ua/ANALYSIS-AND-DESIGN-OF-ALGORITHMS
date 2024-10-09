#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm> // Para la función min
#include <climits> // Para INT_MAX


using namespace std;

const int SENTINEL = numeric_limits<int>::max();

int mcp_naive(vector<vector<int>>& grid, int i, int j) {
    // Base case: reached the origin
    if (i == 0 && j == 0) {
        return grid[i][j];
    }

    // Base case: out of bounds
    if (i < 0 || j < 0) {
        return SENTINEL;
    }

    // Recursive calls for left, up-left, and up movements
    int left = mcp_naive(grid, i, j - 1);
    int up_left = mcp_naive(grid, i - 1, j - 1);
    int up = mcp_naive(grid, i - 1, j);

    // Find the minimum of the three movements
    int min_cost = min(left, min(up_left, up));

    // Add the current cell's cost to the minimum cost
    return grid[i][j] + min_cost;
}

int mcp_memoization(vector<vector<int>>& grid, int i, int j, vector<vector<int>>& memo) {
    // Base case: reached the origin
    if (i == 0 && j == 0) {
        return grid[i][j];
    }

    // Base case: out of bounds
    if (i < 0 || j < 0) {
        return SENTINEL;
    }

    // Check if the result is already memoized
    if (memo[i][j] != SENTINEL) {
        return memo[i][j];
    }

    // Recursive calls for left, up-left, and up movements
    int left = mcp_memoization(grid, i, j - 1, memo);
    int up_left = mcp_memoization(grid, i - 1, j - 1, memo);
    int up = mcp_memoization(grid, i - 1, j, memo);

    // Find the minimum of the three movements
    int min_cost = min(left, min(up_left, up));

    // Calculate the result and store it in the memo table
    int result = grid[i][j] + min_cost;
    memo[i][j] = result;

    return result;
}

int mcp_iterative(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();

    // Creamos una tabla para almacenar los resultados intermedios
    vector<vector<int>> dp(n, vector<int>(m, SENTINEL));

    // Caso base: el origen tiene costo 0
    dp[0][0] = grid[0][0];

    // Rellenamos el contorno de la tabla usando los casos base
    for (int i = 1; i < n; i++) {
        dp[i][0] = dp[i - 1][0] + grid[i][0];
    }
    for (int j = 1; j < m; j++) {
        dp[0][j] = dp[0][j - 1] + grid[0][j];
    }

    // Rellenamos el resto de la tabla de forma iterativa
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            dp[i][j] = grid[i][j] + min(dp[i - 1][j], min(dp[i][j - 1], dp[i - 1][j - 1]));
        }
    }

    // El resultado estará en la esquina inferior derecha de la tabla
    return dp[n - 1][m - 1];
}


int mcp_iterative_optimized(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();

    // Creamos dos vectores para almacenar los resultados intermedios
    vector<int> prev_row(m, SENTINEL);
    vector<int> curr_row(m, SENTINEL);

    // Caso base: el origen tiene costo 0
    prev_row[0] = grid[0][0];

    // Rellenamos la primera fila usando el caso base
    for (int j = 1; j < m; j++) {
        prev_row[j] = prev_row[j - 1] + grid[0][j];
    }

    // Rellenamos el resto de la tabla de forma iterativa
    for (int i = 1; i < n; i++) {
        // Copiamos la fila anterior en la fila actual
        curr_row = prev_row;

        // Rellenamos la primera columna de la fila actual
        curr_row[0] += grid[i][0];

        // Rellenamos el resto de la fila actual
        for (int j = 1; j < m; j++) {
            curr_row[j] = grid[i][j] + min(curr_row[j - 1], min(prev_row[j], prev_row[j - 1]));
        }

        // Copiamos la fila actual en la fila anterior para la siguiente iteración
        prev_row = curr_row;
    }

    // El resultado estará en la última posición de la fila anterior
    return prev_row[m - 1];
}

int readMapFromFile(const string& filename, vector<vector<int>>& map) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
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

const string USAGE_MESSAGE = "Usage: mcp [-t] [--p2D] [--ignore-naive] -f file\n"
                             "\n"
                             "Options:\n"
                             "  -t              Perform transformation\n"
                             "  --p2D           Convert to 2D\n"
                             "  --ignore-naive  Ignore naive method\n"
                             "  -f file         Specify input file\n";

void show_usage() {
    cerr << USAGE_MESSAGE;
}

void printMap(const vector<vector<int>>& map) {
    for (const auto& row : map) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    bool perform_transformation = false;
    bool convert_to_2D = false;
    bool ignore_naive = false;
    string file_name;

    // Parsing arguments
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-t") {
            perform_transformation = true;
        } else if (arg == "--p2D") {
            convert_to_2D = true;
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
            show_usage();
            exit(EXIT_FAILURE);
        }
    }

    // Check if file_name is provided
    if (file_name.empty()) {
        cerr << "ERROR: Missing file name." << endl;
        exit(EXIT_FAILURE);
    }

    // Attempt to open the file
    ifstream input_file(file_name);
    if (!input_file.is_open()) {
        cerr << "ERROR: Can't open file: " << file_name << endl;
        exit(EXIT_FAILURE);
    }

    // Read the map from the file
    vector<vector<int>> map;
    int read_result = readMapFromFile(file_name, map);
    if (read_result != 0) {
        cerr << "ERROR: Failed to read map from file." << endl;
        exit(EXIT_FAILURE);
    }

    // Print the read map
    cout << "Input file: " << file_name << endl;
    cout << "Map:" << endl;
    printMap(map);

    // If everything is fine, perform the required operations
    // In this case, just printing the selected options
    cout << "Perform transformation: " << boolalpha << perform_transformation << endl;
    cout << "Convert to 2D: " << boolalpha << convert_to_2D << endl;
    cout << "Ignore naive method: " << boolalpha << ignore_naive << endl;

    // Calculate the minimum cost path using the memoization
    vector<vector<int>> memo(map.size(), vector<int>(map[0].size(), SENTINEL));
    int result_memoization = mcp_memoization(map, map.size() - 1, map[0].size() - 1, memo);
    // Print "-" if the naive method is ignored
    if (ignore_naive) {
        cout << "Minimum cost path using naive method: -" << endl;
    } else {
        // Calculate and print the minimum cost path using the naive method
        int result_naive = mcp_naive(map, map.size() - 1, map[0].size() - 1);
        cout << "Minimum cost path using naive method: " << result_naive << endl;
    }

    // Print the result of memoization
    cout << "Minimum cost path using memoization: " << result_memoization << endl;

    // Calcula el costo mínimo del camino utilizando el enfoque iterativo
int result_iterative = mcp_iterative(map);

// Calcula el costo mínimo del camino utilizando la optimización del enfoque iterativo
int result_iterative_optimized = mcp_iterative_optimized(map);

// Imprime los resultados
cout << "Minimum cost path using iterative method: " << result_iterative << endl;
cout << "Minimum cost path using optimized iterative method: " << result_iterative_optimized << endl;


    // If convert_to_2D is true, print the path in 2D format
    if (convert_to_2D) {
// Calcular el camino de dificultad mínima
    // Primero, crear una matriz para almacenar los caracteres
    vector<vector<char>> path(map.size(), vector<char>(map[0].size(), '.'));

    // Luego, seguir el camino óptimo desde la esquina inferior derecha hacia arriba y hacia la izquierda
    int i = map.size() - 1;
    int j = map[0].size() - 1;
    while (i > 0 || j > 0) {
        path[i][j] = 'x';
        int min_neighbour = min({i > 0 ? memo[i - 1][j] : INT_MAX,
                                  j > 0 ? memo[i][j - 1] : INT_MAX,
                                  (i > 0 && j > 0) ? memo[i - 1][j - 1] : INT_MAX});

        if (i > 0 && memo[i - 1][j] == min_neighbour) {
            i--;
        } else if (j > 0 && memo[i][j - 1] == min_neighbour) {
            j--;
        } else {
            i--;
            j--;
        }
    }
    path[0][0] = 'x';

    // Imprimir el camino en formato 2D
    cout << "2D Path:" << endl;
    for (const auto& row : path) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }

    // Calcular la dificultad del camino calculado
    int path_difficulty = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (path[i][j] == 'x') {
                path_difficulty += map[i][j];
            }
        }
    }

    // Imprimir la dificultad del camino calculado
    cout << "2D Path Difficulty: " << path_difficulty << endl;    }

    // If perform_transformation is true, print the table of results for the iterative method
    if (perform_transformation) {
        // Calculate and print the table of results for the iterative method
        cout << "Table of results for the iterative method:" << endl;
        vector<vector<int>> dp(map.size(), vector<int>(map[0].size(), SENTINEL));
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[0].size(); j++) {
                if (i == 0 && j == 0) {
                    dp[i][j] = map[i][j];
                } else if (i == 0) {
                    dp[i][j] = dp[i][j - 1] + map[i][j];
                } else if (j == 0) {
                    dp[i][j] = dp[i - 1][j] + map[i][j];
                } else {
                    dp[i][j] = map[i][j] + min(dp[i - 1][j], min(dp[i][j - 1], dp[i - 1][j - 1]));
                }
                cout << dp[i][j] << ' ';
            }
            cout << endl;
        }
    }

    return 0;
}
