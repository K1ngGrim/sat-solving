//
// Created by flo on 02.04.26.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "ipasir.h"

using namespace std;

struct Edge {
    int u, v;
};

int n;
int Kmax;

int var(int v, int c) {
    return v * Kmax + c + 1;
}

int color_active(int c) {
    return n * Kmax + c + 1;
}

void add_clause(void* solver, const vector<int>& clause) {
    for (int lit : clause)
        ipasir_add(solver, lit);
    ipasir_add(solver, 0);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: ./blatt_1_1 <datei>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file) {
        cerr << "Kann Datei nicht oeffnen: " << argv[1] << '\n';
        return 1;
    }


    string line;

    vector<Edge> edges;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        if (line[0] == 'p') {
            string tmp;
            int m;
            stringstream ss(line);
            ss >> tmp >> tmp >> n >> m;
        } else if (line[0] == 'e') {
            char tmp;
            int u, v;
            stringstream ss(line);
            ss >> tmp >> u >> v;
            edges.push_back({u - 1, v - 1});
        }
    }

    Kmax = n;

    void* solver = ipasir_init();

    // =========================
    // CNF einmal komplett bauen
    // =========================

    //0. Farbe muss aktiviert sein

    for (int v = 0; v < n; v++) {
        for (int c = 0; c < Kmax; c++) {
            add_clause(solver, { -var(v,c), color_active(c) });
        }
    }

    //1. Jeder Knoten hat mindestens eine Farbe

    for (int v = 0; v < n; v++) {
        vector<int> clause;
        clause.reserve(Kmax);
        for (int c = 0; c < Kmax; c++) {
            clause.push_back(var(v, c));
        }
        add_clause(solver, clause);
    }

    //2. Jeder Knoten hat maximal eine Farbe

    // 2. Jeder Knoten hat maximal eine Farbe
    for (int v = 0; v < n; v++) {
        for (int c1 = 0; c1 < Kmax; c1++) {
            for (int c2 = c1 + 1; c2 < Kmax; c2++) {
                add_clause(solver, {
                    -var(v, c1),
                    -var(v, c2)
                });
            }
        }
    }

    //3. Benachbarte Knoten haben nicht die selbe Farbe

    for (auto& e : edges) {
        for (int c = 0; c < Kmax; c++) {
            add_clause(solver, {
                -var(e.u, c),
                -var(e.v, c)
            });
        }
    }

    // =========================
    // Inkrementelles Lösen
    // =========================

    for (int k = 1; k <= Kmax; k++) {
        cout << "Trying k = " << k << endl;

        // Assumptions:
        // Farben < k aktiv
        for (int c = 0; c < k; c++) {
            ipasir_assume(solver, color_active(c));
        }

        // Farben >= k deaktivieren
        for (int c = k; c < Kmax; c++) {
            ipasir_assume(solver, -color_active(c));
        }


        int res = ipasir_solve(solver);

        if (res == 10) { // SAT
            cout << "\nMinimum colors = " << k << "\n\n";

            for (int v = 0; v < n; v++) {
                for (int c = 0; c < k; c++) {
                    if (ipasir_val(solver, var(v, c)) > 0) {
                        cout << "Node " << v+1 << " -> Color " << c << endl;
                    }
                }
            }

            break;
        }
    }

    ipasir_release(solver);
    return 0;
}