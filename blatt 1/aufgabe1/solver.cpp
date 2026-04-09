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

int n; // Anzahl Knoten
int Kmax; // maximale Farben

// Variable: x(v,c)
int var(int v, int c) {
    return v * Kmax + c + 1;
}

// Aktivierungsvariable für Farbe c
int color_active(int c) {
    return n * Kmax + c + 1;
}

void add_clause(void* solver, const vector<int>& clause) {
    for (int lit : clause)
        ipasir_add(solver, lit);
    ipasir_add(solver, 0);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: ./color graph.col\n";
        return 1;
    }

    ifstream file("/home/flo/Dokumente/Workspace/sat-solving/blatt 1/aufgabe1/examples/small_test.col");
    string line;

    vector<Edge> edges;

    // Parsing
    while (getline(file, line)) {
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

    Kmax = n; // worst case

    void* solver = ipasir_init();

    // =========================
    // CNF einmal komplett bauen
    // =========================


    // Hier theoretisch alle Klauseln hinzufügen, die die Struktur des Problems beschreiben. Diese Klauseln bleiben während des gesamten inkrementellen Lösungsprozesses gleich. TODO

    /* Folgende sind aber falsch

    // 1. Jeder Knoten hat mindestens eine Farbe (nur wenn Farbe aktiv)
    for (int v = 0; v < n; v++) {
        vector<int> clause;
        for (int c = 0; c < Kmax; c++) {
            // x(v,c) AND color_active(c)
            // encoded as: ¬active(c) ∨ x(v,c)
            clause.push_back(var(v, c));
        }
        add_clause(solver, clause);
    }

    // 2. Höchstens eine Farbe pro Knoten
    for (int v = 0; v < n; v++) {
        for (int c1 = 0; c1 < Kmax; c1++) {
            for (int c2 = c1 + 1; c2 < Kmax; c2++) {
                add_clause(solver, {
                    -var(v, c1),
                    -var(v, c2)
                });
                cout << "Added node clause: " << -var(v, c1) << " " << -var(v, c2) << "\n";
            }
        }
    }

    // 3. Kanten: gleiche Farbe verboten

    for (auto& e : edges) {
        for (int c = 0; c < Kmax; c++) {
            add_clause(solver, {
                -var(e.u, c),
                -var(e.v, c)
            });
            cout << "Added edge clause: " << -var(e.u, c) << " " << -var(e.v, c) << "\n";
        }
    }

*/

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