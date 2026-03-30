#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

using namespace std;

// Prüft, ob c ganzzahlig ist
bool is_square(int x) {
    int s = (int) sqrt(x);
    return s * s == x;
}

int generate_clauses(int n, string* clauses) {
    vector<tuple<int,int,int>> triples;

    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            int c2 = a*a + b*b;
            if (is_square(c2)) {
                int c = (int) sqrt(c2);
                if (c <= n && b < c) {
                    triples.emplace_back(a, b, c);
                }
            }
        }
    }

    //cout << triples.size() << "\n";

    int num_vars = n;
    int num_clauses = 2 * triples.size();

    *clauses = "p cnf " + to_string(num_vars) + " " + to_string(num_clauses) + "\n";

    for (auto tris : triples) {
        int a = get<0>(tris);
        int b = get<1>(tris);
        int c = get<2>(tris);

        *clauses += to_string(-a) + " " + to_string(-b) + " " + to_string(-c) + " 0\n";
        *clauses += to_string(a) + " " + to_string(b) + " " + to_string(c) + " 0\n";
    }

    return num_clauses;
}

int main(int argc, char** argv) {

    string clauses;

    vector<double> x = {};
    vector<double> y = {};

    for (int n = 1; n <= 5000; n++) {
        int num_clauses = generate_clauses(n, &clauses);

        x.push_back(n);
        y.push_back(num_clauses);

        cout << "n = " << n << ", num_clauses = " << num_clauses << "\n";
    }

    /**
    cout << "x = np.array([";

    for (int i = 0; i < x.size(); i++) {
        cout << x[i] << ", ";
    }

    cout << "])" << "\n";

    cout << "y = np.array([";

    for (int i = 0; i < x.size(); i++) {
        cout << y[i] << ", ";
    }

    cout << "])" << "\n";
    **/


    return 0;
}