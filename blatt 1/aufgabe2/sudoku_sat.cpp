#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int n;              // z.B. 9
int k;              // sqrt(n)
vector<vector<int>> clauses;
vector<vector<string>> string_clauses;

int var(const int row, const int col, const int val) {
    return (row-1)*n*n + (col-1)*n + val;
}

int main() {
    n = 4;
    k = std::sqrt(n);

    if (k * k != n) {
        std::cerr << "n muss eine perfekte Quadratzahl sein!\n";
        return 1;
    }

    //Rows
    for (int brow = 0; brow < n; brow++) {
        for (int val = 0; val < n; val++) {
            vector<int> clause;
            vector<string> string_clause;

            for (int i = 0; i < n; i++) {
                clause.push_back(var(brow + 1, i + 1, val + 1));
                string_clause.push_back("X" + to_string(brow + 1) + to_string(i + 1) + to_string(val + 1));
            }
            clauses.push_back(clause);
            string_clauses.push_back(string_clause);

            for (int a = 0; a < clause.size(); a++) {
                for (int b = a + 1; b < clause.size(); b++) {
                    vector<int> pair_clause;
                    vector<string> pair_string_clause;

                    pair_clause.push_back(-clause[a]);
                    pair_clause.push_back(-clause[b]);

                    pair_string_clause.push_back("-" + string_clause[a]);
                    pair_string_clause.push_back("-" + string_clause[b]);

                    clauses.push_back(pair_clause);
                    string_clauses.push_back(pair_string_clause);
                }
            }
        }
    }

    //Cols
    for (int col = 0; col < n; col++) {
        for (int val = 0; val < n; val++) {
            vector<int> clause;
            vector<string> string_clause;

            for (int i = 0; i < n; i++) {
                clause.push_back(var(i + 1, col + 1 , val + 1));
                string_clause.push_back("X" + to_string(i + 1) + to_string(col + 1) + to_string(val + 1));
            }
            clauses.push_back(clause);
            string_clauses.push_back(string_clause);

            for (int a = 0; a < clause.size(); a++) {
                for (int b = a + 1; b < clause.size(); b++) {
                    vector<int> pair_clause;
                    vector<string> pair_string_clause;

                    pair_clause.push_back(-clause[a]);
                    pair_clause.push_back(-clause[b]);

                    pair_string_clause.push_back("-" + string_clause[a]);
                    pair_string_clause.push_back("-" + string_clause[b]);

                    clauses.push_back(pair_clause);
                    string_clauses.push_back(pair_string_clause);
                }
            }
        }
    }


    // //Box
    for (int brow = 0; brow < k; brow++) {
        for (int bcol = 0; bcol < k; bcol++) {
            for (int val = 0; val < n; val++) {

                vector<int> clause;
                vector<string> string_clause;

                for (int i = 0; i < k; i++) {
                    for (int j = 0; j < k; j++) {
                        clause.push_back(var(brow*k + i + 1, bcol*k + j + 1, val + 1));
                        string_clause.push_back("X" + to_string(brow*k + i + 1) + to_string(bcol*k + j + 1) + to_string(val + 1));
                    }
                }
                clauses.push_back(clause);
                string_clauses.push_back(string_clause);

                for (int a = 0; a < clause.size(); a++) {
                    for (int b = a + 1; b < clause.size(); b++) {
                        vector<int> pair_clause;
                        vector<string> pair_string_clause;

                        pair_clause.push_back(-clause[a]);
                        pair_clause.push_back(-clause[b]);

                        pair_string_clause.push_back("-" + string_clause[a]);
                        pair_string_clause.push_back("-" + string_clause[b]);

                        clauses.push_back(pair_clause);
                        string_clauses.push_back(pair_string_clause);
                    }
                }
            }
        }
    }

    //Damits funktioniert noch pro Zelle eine Zahl (eigentlich dumm aber macht doch irgendwie sinn)

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {

            vector<int> clause;
            vector<string> string_clause;

            for (int v = 0; v < n; v++) {
                clause.push_back(var(r+1, c+1, v+1));
                string_clause.push_back("X" + to_string(r+1) + to_string(c+1) + to_string(v+1));
            }

            clauses.push_back(clause);
            string_clauses.push_back(string_clause);

            for (int a = 0; a < clause.size(); a++) {
                for (int b = a + 1; b < clause.size(); b++) {

                    vector<int> pair_clause;
                    vector<string> pair_string_clause;

                    pair_clause.push_back(-clause[a]);
                    pair_clause.push_back(-clause[b]);

                    pair_string_clause.push_back("-" + string_clause[a]);
                    pair_string_clause.push_back("-" + string_clause[b]);

                    clauses.push_back(pair_clause);
                    string_clauses.push_back(pair_string_clause);
                }
            }
        }
    }



    cout << "p cnf " << n*n*n << " " << clauses.size() + string_clauses.size() << "\n";
    for (const auto& clause : clauses) {
        for (size_t i = 0; i < clause.size(); i++) {
            cout << clause[i];
            if (i < clause.size() - 1) {
                cout << " ";
            }
        }
        cout << " 0\n";
    }

    // for (const auto& string_clause : string_clauses) {
    //     for (size_t i = 0; i < string_clause.size(); i++) {
    //         cout << string_clause[i];
    //         if (i < string_clause.size() - 1) {
    //             cout << " ";
    //         }
    //     }
    //     cout << " 0\n";
    // }



    return 0;
}