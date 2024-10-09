#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <queue>
#include <tuple>
#include <iomanip>
#include <ctime>

using namespace std;

using Node = tuple<int, unsigned, unsigned, unsigned>; // cota/prioridad, x, y, coste

const int SENTINEL = numeric_limits<int>::max();
const int MOVEMENTS = 8;
int dx[MOVEMENTS] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[MOVEMENTS] = {-1, 0, 1, -1, 1, -1, 0, 1};

int n, m;
int nvisit = 0, nexplored = 0, nleaf = 0, nunfeasible = 0, nnot_promising = 0;
int npromising_but_discarded = 0, nbest_solution_updated_from_leafs = 0, nbest_solution_updated_from_pessimistic_bound = 0;

bool valido(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

int readMapFromFile(const string& filename, vector<vector<int>>& map, vector<vector<int>>& dp) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: Can't open file: " << filename << endl;
        return -1;
    }

    file >> n >> m;
    map.resize(n, vector<int>(m));
    dp.resize(n, vector<int>(m, SENTINEL));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            file >> map[i][j];
        }
    }

    file.close();
    return 0;
}

void printcamino2D(const vector<vector<int>>& map, const vector<pair<int, int>>& camino) {
    vector<vector<char>> recorrido(map.size(), vector<char>(map[0].size(), '.'));

    for (const auto& path : camino) {
        recorrido[path.first][path.second] = 'x';
    }

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            cout << recorrido[i][j];
        }
        cout << endl;
    }

    int dificultad = 0;
    for (const auto& path : camino) {
        dificultad += map[path.first][path.second];
    }
    cout << dificultad << endl;
}

void codificado(const vector<pair<int, int>>& camino) {
    string codificacion = "<";
    for (size_t i = 1; i < camino.size(); ++i) {
        int dx = camino[i].first - camino[i - 1].first;
        int dy = camino[i].second - camino[i - 1].second;
        if (dx == -1 && dy == 0) codificacion += '1';
        else if (dx == -1 && dy == 1) codificacion += '2';
        else if (dx == 0 && dy == 1) codificacion += '3';
        else if (dx == 1 && dy == 1) codificacion += '4';
        else if (dx == 1 && dy == 0) codificacion += '5';
        else if (dx == 1 && dy == -1) codificacion += '6';
        else if (dx == 0 && dy == -1) codificacion += '7';
        else if (dx == -1 && dy == -1) codificacion += '8';
    }
    codificacion += '>';
    cout << codificacion << endl;
}

void initializeDynamicProgramming(const vector<vector<int>>& map, vector<vector<int>>& dp) {
    dp[n-1][m-1] = map[n-1][m-1];
    for (int i = n-1; i >= 0; --i) {
        for (int j = m-1; j >= 0; --j) {
            if (i == n-1 && j == m-1) continue;
            int min_cost = SENTINEL;
            for (int k = 0; k < MOVEMENTS; ++k) {
                int nx = i + dx[k];
                int ny = j + dy[k];
                if (valido(nx, ny)) {
                    min_cost = min(min_cost, dp[nx][ny]);
                }
            }
            if (min_cost != SENTINEL) {
                dp[i][j] = map[i][j] + min_cost;
            }
        }
    }
}

void mcp_bb(const vector<vector<int>>& map, vector<vector<int>>& dp) {
    clock_t start = clock();
    priority_queue<Node> pq;
    pq.emplace(-dp[0][0], 0, 0, map[0][0]);

    vector<pair<int, int>> best_path;
    vector<pair<int, int>> current_path;
    int best_cost = SENTINEL;
    while (!pq.empty()) {
        auto [priority, x, y, currentCost] = pq.top();
        pq.pop();
        nvisit++;

        if (x == n-1 && y == m-1) {
            if (currentCost < best_cost) {
                best_cost = currentCost;
                best_path = current_path;
                nbest_solution_updated_from_leafs++;
            }
            nleaf++;
            continue;
        }

        bool hasValidChild = false;
        for (int i = 0; i < MOVEMENTS; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (valido(nx, ny)) {
                hasValidChild = true;
                int newCost = currentCost + map[nx][ny];
                if (newCost + dp[nx][ny] < best_cost) {
                    pq.emplace(-(newCost + dp[nx][ny]), nx, ny, newCost);
                    nexplored++;
                    current_path.push_back({nx, ny});
                } else {
                    npromising_but_discarded++;
                }
            } else {
                nunfeasible++;
            }
        }

        if (!hasValidChild) {
            nleaf++;
        }
    }

    if (best_cost == SENTINEL) {
        cout << "No path found" << endl;
    } else {
        cout << best_cost << endl;
    }

    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    cout << nvisit << " " << nexplored << " " << nleaf << " " << nunfeasible << " " << nnot_promising << " " 
         << npromising_but_discarded << " " << nbest_solution_updated_from_leafs << " " << nbest_solution_updated_from_pessimistic_bound << endl;
    cout << fixed << setprecision(3) << cpu_time_used << endl;

    if (!best_path.empty()) {
        printcamino2D(map, best_path);
        codificado(best_path);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: mcp_bb [ -p] [ --p2D] -f fichero entrada" << endl;
        exit(EXIT_FAILURE);
    }

    bool mostrarCamino = false, mostrar2D = false;
    string filename;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-f") {
            if (i + 1 < argc) {
                filename = argv[++i];
            } else {
                cerr << "ERROR: Missing file name." << endl;
                exit(EXIT_FAILURE);
            }
        } else if (arg == "-p") {
            mostrarCamino = true;
        } else if (arg == "--p2D") {
            mostrar2D = true;
        } else {
            cerr << "ERROR: Unknown option '" << arg << "'." << endl;
            exit(EXIT_FAILURE);
        }
    }

    if (filename.empty()) {
        cerr << "ERROR: Missing file name." << endl;
        exit(EXIT_FAILURE);
    }

    vector<vector<int>> map, dp;
    if (readMapFromFile(filename, map, dp) != 0) {
        cerr << "ERROR: Failed to read map from file." << endl;
        exit(EXIT_FAILURE);
    }

    initializeDynamicProgramming(map, dp);
    mcp_bb(map, dp);

    return 0;
}
