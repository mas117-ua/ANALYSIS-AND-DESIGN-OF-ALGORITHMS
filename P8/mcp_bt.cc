// MIHAI ALEXANDRU STEFANOIU 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

const int SENTINEL = numeric_limits<int>::max();
const int MOVEMENTS = 8;
int nvisit = 0, nexplored = 1, nleaf = 0, nunfeasible = 0, nnot_promising = 0;

int dx[MOVEMENTS] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[MOVEMENTS] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool valido(int x, int y, int n, int m) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

int readMapFromFile(const string& filename, vector<vector<int>>& map) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: Can't open file: " << filename << endl;
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

void backtrack_aux(int x, int y, int costeActual, vector<vector<bool>>& visitado, vector<pair<int, int>>& camino, vector<vector<int>>& mejorCoste, vector<pair<int, int>>& mejorCamino, const vector<vector<int>>& map, int& minCoste, const int& n, const int& m) {
    nvisit++;
    if (costeActual >= minCoste) {
        nnot_promising++;
        return;
    }

    if (x == n - 1 && y == m - 1) {
        nleaf++;
        if (costeActual < minCoste) {
            minCoste = costeActual;
            mejorCamino = camino;
        }
        return;
    }

    vector<pair<int, int>> vecinos;
    for (int i = 0; i < MOVEMENTS; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (valido(nx, ny, n, m) && !visitado[nx][ny]) {
            vecinos.emplace_back(nx, ny);
        }
    }

    sort(vecinos.begin(), vecinos.end(), [&map, &n, &m](const pair<int, int>& a, const pair<int, int>& b) {
        int ha = abs(a.first - (n - 1)) + abs(a.second - (m - 1));
        int hb = abs(b.first - (n - 1)) + abs(b.second - (m - 1));
        return ha < hb;
    });

    for (const auto& v : vecinos) {
        int nx = v.first;
        int ny = v.second;
        nexplored++;
        if (costeActual + map[nx][ny] < mejorCoste[nx][ny]) {
            mejorCoste[nx][ny] = costeActual + map[nx][ny];
            visitado[nx][ny] = true;
            camino.emplace_back(nx, ny);
            backtrack_aux(nx, ny, costeActual + map[nx][ny], visitado, camino, mejorCoste, mejorCamino, map, minCoste, n, m);
            camino.pop_back();
            visitado[nx][ny] = false;
        } else {
            nnot_promising++;
        }
    }
}

void mcp_bt(const vector<vector<int>>& map, bool mostrarCamino, bool mostrar2D) {
    int n = map.size();
    int m = map[0].size();
    vector<vector<bool>> visitado(n, vector<bool>(m, false));
    vector<pair<int, int>> camino = {{0, 0}}, mejorCamino;
    int minCoste = SENTINEL;

    vector<vector<int>> mejorCoste(n, vector<int>(m, SENTINEL));
    mejorCoste[0][0] = map[0][0];

    clock_t start = clock();
    visitado[0][0] = true;
    backtrack_aux(0, 0, map[0][0], visitado, camino, mejorCoste, mejorCamino, map, minCoste, n, m);
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    cout << minCoste << endl;
    cout << nvisit << " " << nexplored << " " << nleaf << " " << nunfeasible << " " << nnot_promising << endl;
    cout << fixed << setprecision(2) << cpu_time_used << endl;

    if (mostrar2D) {
        printcamino2D(map, mejorCamino);
    }

    if (mostrarCamino) {
        codificado(mejorCamino);
    }
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: mcp_bt [ -p] [ --p2D] -f fichero entrada" << endl;
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

    vector<vector<int>> map;
    if (readMapFromFile(filename, map) != 0) {
        cerr << "ERROR: Failed to read map from file." << endl;
        exit(EXIT_FAILURE);
    }

    mcp_bt(map, mostrarCamino, mostrar2D);

    return 0;
}
