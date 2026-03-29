//
// Created by Florian Kaiser on 25.03.26.
//

#include <iostream>
using namespace std;

int var(int row, int col, int val, int n) {
    return (row-1)*n*n + (col-1)*n + val;
}

int main() {

    for (int i = 1; i <= 9; i++) {

    }


    /**
    *for br in 0 .. k-1:
    for bc in 0 .. k-1:
    for v in 1 .. n:

    cells = []

    for i in 1 .. k:
    for j in 1 .. k:
    r = br*k + i
    c = bc*k + j
    cells.append((r,c))

    # alle Paare verbieten
    for a in 0 .. len(cells)-1:
    for b in a+1 .. len(cells)-1:

    (r1,c1) = cells[a]
    (r2,c2) = cells[b]

    clauses.append([
    -var(r1,c1,v),
    -var(r2,c2,v)
    ])


**/





    // row comparison
    std::string row = "";
    std::string row_n = "";
    int numb = 1;
    for(int i = 1; i <= 9; i++) {
        for(int j = 1; j <= 9; j++) {
            row.append(std::to_string(numb) + " ");
            row_n.append("-" + std::to_string(numb) + " ");
            numb++;
        }
        row.append("\n");
        row_n.append("\n");
    }

    // Column comparison
    std::string column = "";
    std::string column_n = "";
    for(int i = 1; i <= 9; i++){
        numb = i;
        for(int j = 1; j <= 9; j++){
            column.append(std::to_string(numb) + " ");
            column_n.append("-" + std::to_string(numb) + " ");
            numb += 9;
        }
        column.append("\n");
        column_n.append("\n");
    }

    // box comparison
    std::string box = "";
    std::string box_n = "";
    //Box Counter
    for(int i = 1; i <= 3 ;i++) {
        int curr_column = i*3;
        for(int j = 1; j <= 3; j++) {
            int curr_row = j*3;
            numb = curr_row * 9 + curr_column - 9;
            // In box comparison
            for (int x = 1; x <= 3; x++) {
                for (int y = 1; y <= 3; y++) {
                    box.append(std::to_string(numb) + " ");
                    box_n.append(std::to_string(-numb) + " ");
                    numb--;
                }
                numb -= 6;
            }
            box.append("\n");
            box_n.append("\n");
        }
    }

    cout << "p cnf " << numb << " ???";
    cout << "\n";
    cout << box;
    cout << box_n;
    cout << row;
    cout << row_n;
    cout << column;
    cout << column_n;
    cout << "Fin";
    return 0;
}