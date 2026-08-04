
#include <iostream>
#define N 4

using namespace std;
//cozumu yazdirir
void printSolution(int board[N][N]) {
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(board[i][j]){
                cout<<" Q ";
            }
            else{
                cout<<" . ";
            }
        }
        cout<<"\n";
    }
}

//[row][col] karesine yeni bir vezir koyulabilir mi kontrol eder(sadece sol tarafa var)
bool isSafe(int board[N][N], int row, int col) {
    int i,j;
    //sol satir kontrolu(ayni satirda baska vezir var mi)
    for(i=0;i<col;i++){
        if(board[row][i]){
            return false;
        }
    }
    //ust capraz kontrolu
    for(i=row,j=col;i>=0 && j>=0;i--,j--){
        if(board[i][j]){
            return false;
        }
    }
    //alt capraz kontrolu
    for(i=row,j=col;j>=0 && i<N;i++,j--){
        if(board[i][j]){
            return false;
        }
    }
    return true;
}

//recursive olarak vezirleri yerlestirir
bool solveNQUtil(int board[N][N], int col) {
    //base case:col sayısı N'ye esitse tum vezirler yerlestirilmis demektir
    if(col>=N){
        return true;
    }
    for(int i=0;i<N;i++){
        //eger vezir bu kareye yerlestirilebilirse
        if(isSafe(board,i,col)){
            board[i][col]=1;
            //recursive olarak sonraki kolon icin dene
            if(solveNQUtil(board,col+1)){
                return true;
            }
            //eger sonraki kolon icin yerlesim mumkun degilse geri al
            board[i][col]=0;
        }
    }
    return false;
}

//N vezir problemi cozumunu bulur
bool solveNQ(){
    int board[N][N] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    if(solveNQUtil(board,0)==false){
        cout<<"cozum yok"<<endl;
        return false;
    }
    printSolution(board);
    return true;
}

int main()
{
    solveNQ();
    return 0;
}