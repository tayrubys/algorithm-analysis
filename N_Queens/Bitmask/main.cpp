#include <iostream>
#define N 4

using namespace std;

//cozumu yazdirma
void printSolution(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j]) {
                cout << " Q ";
            } else {
                cout << " . ";
            }
        }
        cout << "\n";
    }
}
//cozum 
bool solveNQBitmaskUtil(int row, int cols, int leftDiag, int rightDiag, int FULL_BOARD, int board[N][N]) {
    //base case:tumm satirlara vezir yerlestirildiyse
    if (row == N) {
        return true;
    }
    //guvenli sutunlarin bitmaskini hesaplar ~ islemi yasakli yerleri tersine cevirir
    int safePositions = ~(cols | leftDiag | rightDiag) & FULL_BOARD;

    //denenecek guvenli kare kaldigi surece dongu calisir
    while (safePositions > 0) {
        //enn sagdaki 1 biti se
        //ornegin:safePositions = 101100
        // p = 000100
        int p = safePositions & -safePositions;

        //secilen kareyi guvenli karelerden cikar
        safePositions -= p;
        //bitin hangi sutuna ait oldugunu bulmak icin __builtin_ctz kullanilir
        int col = __builtin_ctz(p);

        //veziri board'a yerlestir
        board[row][col] = 1;
        //recursive olarak sonraki satir icin dene
        //cols:yeni sutunu dolu olarak isaretle, leftDiag:sol caprazi sola kaydir, rightDiag:sag caprazi saga kaydir
        if (solveNQBitmaskUtil(row + 1, cols | p, (leftDiag | p) << 1, (rightDiag | p) >> 1, FULL_BOARD, board)) {
            return true;
        }
        //backtrack
        board[row][col] = 0;
    }

    //hic guveli kare kalmadiysa
    return false;
}
//cozum baslatma
bool solveNQ() {
    int board[N][N] = {0};//baslangicta tum board bos
    int FULL_BOARD = (1 << N) - 1;//4 bit kullanmamizi saglar

    //tarama 0. satirdan, tum maskeler 0 olarak baslatir
    if (solveNQBitmaskUtil(0, 0, 0, 0, FULL_BOARD, board) == false) {
        cout << "cozum yok" << endl;
        return false;
    }

    printSolution(board);
    return true;
}

int main() {
    solveNQ();
    return 0;
}