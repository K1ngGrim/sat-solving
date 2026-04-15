#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

int n;              // z.B. 9
int k;              // sqrt(n)
vector<vector<int>> clauses;
vector<vector<string>> string_clauses;

int var(const int row, const int col, const int val) {
    return (row-1)*n*n + (col-1)*n + val;
}

void inv_var(int var, int &row, int &col, int &val, int n) {
    var--; // 0-basiert

    row = var / (n*n);
    var %= (n*n);

    col = var / n;
    val = var % n;

    row++;
    col++;
    val++;
}

vector<tuple<int,int,int>> parse_solution(const string &line, int n) {
    vector<tuple<int,int,int>> result;
    stringstream ss(line);
    string token;

    ss >> token; // "SAT" überspringen

    while (ss >> token) {
        int var = stoi(token);
        cout << var << "\n";

        if (var > 0) { // nur positive Literale
            int row, col, val;
            inv_var(var, row, col, val, n);
            result.emplace_back(row, col, val);
        }
    }

    return result;
}

void read_sudoku(vector<vector<int>> &sudoku) {


    string delimiter = " ";
    ifstream file("/Users/flori/Documents/Workspace/sat-solving/blatt 1/aufgabe2/examples/puzzle02a.sudoku");
    string line;

    getline(file, line);// erste Zeile skippen

    n = pow(stoi(line), 2);

    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        size_t pos = 0;
        string token;
        vector<int> tokens;
        int num;

        while ((pos = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, pos);

            if (!token.empty()) {
                tokens.push_back(stoi(token));
            }

            line.erase(0, pos + delimiter.length());
        }

        if (!line.empty()) {
            tokens.push_back(stoi(line));
        }
        sudoku.push_back(tokens);
    }

}

void print_sat() {
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
}

bool row_contains(const vector<int> &row, const int val) {
    for (int i = 0; i < row.size(); i++) {
        if (row[i] == val) {
            return true;
        }
    }
    return false;
}

bool box_contains(const vector<vector<int>> &sudoku, const int brow, const int bcol, const int val) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (sudoku[brow*k + i][bcol*k + j] == val) {
                return true;
            }
        }
    }
    return false;
}

bool col_contains(const vector<vector<int>> &sudoku, const int col, const int val) {
    for (int i = 0; i < sudoku.size(); i++) {
        if (sudoku[i][col] == val) {
            return true;
        }
    }
    return false;
}

int clauses_main() {
    vector<vector<int>> sudoku;
    read_sudoku(sudoku);

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

            if (row_contains(sudoku[brow], val + 1)) {
                continue;
            }

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

            if (col_contains(sudoku, col, val + 1)) {
                continue;
            }

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

                if (box_contains(sudoku, brow, bcol, val + 1)) {
                    continue;
                }

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

            if (sudoku[r][c] != 0) {
                continue;
            }


            for (int v = 0; v < n; v++) {

                if (sudoku[r][c] == 0) {
                    clause.push_back(var(r+1, c+1, v+1));
                    string_clause.push_back("X" + to_string(r+1) + to_string(c+1) + to_string(v+1));
                }else {
                    if (sudoku[r][c] == v + 1 ) {
                        clause.push_back(var(r+1, c+1, v+1));
                        string_clause.push_back("X" + to_string(r+1) + to_string(c+1) + to_string(v+1));
                    }
                }
            }

            if (clause.empty()) {
                continue;
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

    print_sat();

    return 0;
}

int resolve_sudoku() {

    vector<vector<int>> sudoku;
    read_sudoku(sudoku);

    string line = "SAT -1 -2 -3 -4 -5 6 -7 -8 -9 -10 11 -12 -13 -14 -15 16 -17 -18 -19 20 -21 -22 23 -24 -25 -26 -27 -28 -29 30 -31 -32 -33 34 -35 -36 -37 -38 -39 -40 -41 -42 -43 44 -45 -46 47 -48 -49 -50 51 -52 -53 -54 -55 56 -57 58 -59 -60 -61 -62 -63 -64";
    vector<tuple<int,int,int>> sol = parse_solution(line, n);
    vector<vector<int>> sudoku_solution = vector<vector<int>>(n, vector<int>(n, 0));

    for (const auto& sol_tup : sol) {
        int row, col, val;
        std::tie(row, col, val) = sol_tup;
        sudoku_solution[row-1][col-1] = val;

        cout << "Row: " << row << ", Col: " << col << ", Val: " << val << "\n";
    }

    //print sudoku fin
    for (const auto& row : sudoku_solution) {
        for (const auto& val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }


    return 0;
}


int main() {

    //return clauses_main();
    return resolve_sudoku();


    //return 0;
}