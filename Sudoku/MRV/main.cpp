#include <iostream>
#include <vector>
using namespace std;

//sayiyi yerlestirmek guvenli mi
bool isSafe(vector<vector<int>> &mat, int row, int col, int num) {
    //satir kontrolu
    for (int x = 0; x < 9; x++) {
        if (mat[row][x] == num)
            return false;
    }
    //sutun kontrolu
    for (int x = 0; x < 9; x++) {
        if (mat[x][col] == num)
            return false;
    }
    // 3x3 kutu kontrolu
    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (mat[startRow + i][startCol + j] == num)
                return false;
        }
    }
    return true;
}
//mrv:en az secenegi olan bos hucreyi bul
bool findMRVCell(vector<vector<int>> &mat, int &bestRow, int &bestCol) {
    int minChoices = 10; // En fazla 9 seçenek olabilir
    bool found = false;

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (mat[row][col] == 0) {
                int choices = 0;
                //bu hucreye kac sayi yerlesebilir?
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(mat, row, col, num))
                        choices++;
                }
                //daha az secenegi olan hucre bulundu
                if (choices < minChoices) {
                    minChoices = choices;
                    bestRow = row;
                    bestCol = col;
                    found = true;
                    //daha iyisi olamaz
                    if (minChoices == 0 ||minChoices == 1)
                        return true;
                }
            }
        }
    }
    return found;
}
bool solveSudoku(vector<vector<int>> &mat) {
    int row, col;
    //bos hucre kalmadiysa cozum bulundu
    if (!findMRVCell(mat, row, col))
        return true;
    //secilen hucre icin sayiları dene
    for (int num = 1; num <= 9; num++) {
        if (isSafe(mat, row, col, num)) {
            mat[row][col] = num;
            if (solveSudoku(mat))
                return true;
            //backtracking
            mat[row][col] = 0;
        }
    }
    return false;
}

int main() {
    vector<vector<int>> mat = {
        {3,0,6,5,0,8,4,0,0},
        {5,2,0,0,0,0,0,0,0},
        {0,8,7,0,0,0,0,3,1},
        {0,0,3,0,1,0,0,8,0},
        {9,0,0,8,6,3,0,0,5},
        {0,5,0,0,9,0,6,0,0},
        {1,3,0,0,0,0,2,5,0},
        {0,0,0,0,0,0,0,7,4},
        {0,0,5,2,0,6,3,0,0}
    };
    solveSudoku(mat);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++)
            cout << mat[i][j] << " ";

        cout << endl;
    }
    return 0;
}