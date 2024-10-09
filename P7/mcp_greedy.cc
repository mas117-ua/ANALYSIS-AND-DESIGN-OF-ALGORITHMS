#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm> // Para min_element
#include <utility>   // Para std::pair
#include <tuple>     // Para std::tuple
#include <limits>
using namespace std;

const int SENTINEL = numeric_limits<int>::max();

tuple<vector<vector<int>>, vector<vector<int>>> mcp_greedy(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<int>> tabla_forward(n, vector<int>(m, 0));
    vector<vector<int>> tabla_backward(n, vector<int>(m, 0));

    // Inicializar los valores iniciales de las matrices con los valores del mapa
    tabla_forward[0][0] = grid[0][0];
    tabla_backward[n - 1][m - 1] = grid[n - 1][m - 1];

    // Calcular camino hacia adelante
    for (int i = 1; i < n; i++) {
        tabla_forward[i][0] = tabla_forward[i - 1][0] + grid[i][0];
    }
    for (int j = 1; j < m; j++) {
        tabla_forward[0][j] = tabla_forward[0][j - 1] + grid[0][j];
    }
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            tabla_forward[i][j] = min(tabla_forward[i - 1][j], tabla_forward[i][j - 1]) + grid[i][j];
        }
    }

    // Calcular camino hacia atrás
    for (int i = n - 2; i >= 0; i--) {
        tabla_backward[i][m - 1] = tabla_backward[i + 1][m - 1] + grid[i][m - 1];
    }
    for (int j = m - 2; j >= 0; j--) {
        tabla_backward[n - 1][j] = tabla_backward[n - 1][j + 1] + grid[n - 1][j];
    }
    for (int i = n - 2; i >= 0; i--) {
        for (int j = m - 2; j >= 0; j--) {
            tabla_backward[i][j] = min(tabla_backward[i + 1][j], tabla_backward[i][j + 1]) + grid[i][j];
        }
    }

    return make_tuple(tabla_forward, tabla_backward);
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

        // Considerar el movimiento diagonal
        if (i > 0 && j > 0) {
            min_neighbor = min(min_neighbor, memo[i - 1][j - 1]);
        }

        // Actualizar las coordenadas según la dirección del movimiento elegido
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

    

}
int main(int argc, char* argv[]) {
    bool parse = false;
    string file_name;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--p2D") {
            parse = true;
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

    // Calcular los caminos hacia adelante y hacia atrás usando el enfoque voraz
    vector<vector<int>> tabla_forward, tabla_backward;
    tie(tabla_forward, tabla_backward) = mcp_greedy(map);
    
    // Determinar el camino con la menor dificultad y mostrarlo
    int min_cost_forward = tabla_forward.back().back();
    int min_cost_backward = tabla_backward.front().front();
    cout << min_cost_forward << " " << min_cost_backward << endl;

    // Mostrar los costos de los caminos
    if (min_cost_forward <= min_cost_backward) {
        printPath(map, tabla_forward);
    } else {
        printPath(map, tabla_backward);
    }

    // Mostrar el costo mínimo de los dos caminos calculados con el enfoque voraz
    cout << min(min_cost_forward, min_cost_backward) << endl;

    return 0;
}
