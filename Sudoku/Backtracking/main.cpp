#include <iostream>
#include <vector>
using namespace std;

//row ve col konumuna num sayisini koymak guvenli olup olmadigini kontrol eder
bool isSafe(vector<vector<int>> &mat, int row, int col, int num){
    //sayinin satirda olup olmadigini kontrol eder
    for (int x = 0; x <= 8; x++){
        if (mat[row][x] == num)
            return false;
    }
    //sayinin sutunda olup olmadigini kontrol eder
    for(int x = 0; x<= 8; x++){
        if (mat[x][col] == num)
            return false;
    }
    //sayinin bulundugu 3x3 luk kutuda olup olmadigini kontrol eder
    int startRow = row - (row %3);
    int startCol = col - (col % 3);
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            if(mat[i + startRow][j + startCol] == num)
                return false;
        }
    }
    return true;
}

bool solveSudokuRec(vector<vector<int>> &mat, int row, int col){
    int n = mat.size();
    //base case: son satirin son sutununa ulasildiysa sudoku cozulmustur
    if (row == n - 1 && col == n)
        return true;
    
    //satirin son sutununa gelindiyse bir sonraki satira gec
    if (col == n) {
        row++;
        col = 0;
    }
    //hucre zaten doluysa bir sonraki hucreye gec
    if(mat[row][col] != 0)
        return solveSudokuRec(mat, row, col + 1);
    //tek tek deniyor
    for (int num = 1; num <= n; num++) {
        //sayiyi mevcut konuma yerlestirmek guvenliyse
        if(isSafe(mat, row, col, num)) {
            mat[row][col] = num;
            //bir sonraki hucreyi cozmeye calis
            if (solveSudokuRec(mat, row, col + 1))
                return true;
            mat[row][col] = 0;    
        }
    }
    return false;
}

void solveSudoku(vector<vector<int>> &mat) {
  	solveSudokuRec(mat, 0, 0);
}
int main() {
    vector<vector<int>> mat = {
        {3, 0, 6, 5, 0, 8, 4, 0, 0}, 
      	{5, 2, 0, 0, 0, 0, 0, 0, 0}, 
      	{0, 8, 7, 0, 0, 0, 0, 3, 1},
        {0, 0, 3, 0, 1, 0, 0, 8, 0}, 
      	{9, 0, 0, 8, 6, 3, 0, 0, 5}, 
      	{0, 5, 0, 0, 9, 0, 6, 0, 0},
        {1, 3, 0, 0, 0, 0, 2, 5, 0}, 
      	{0, 0, 0, 0, 0, 0, 0, 7, 4}, 
      	{0, 0, 5, 2, 0, 6, 3, 0, 0}};

	solveSudoku(mat);
    
  	for (int i = 0; i < mat.size(); i++) {
        for (int j = 0; j < mat.size(); j++)
            cout << mat[i][j] << " ";
        cout << endl;
    }

    return 0;
}