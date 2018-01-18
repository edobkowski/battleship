#include <cstdlib>
#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>
#include "btls.h"

using namespace std;

BOARD enemysBoard [10][10];
BOARD playersBoard [10][10];

char abc [10] = {'A','B','C','D','E','F', 'G', 'H', 'I', 'J'};

bool generateSetup;

int playMode; // 0 - Computer-Player 1 - Computer-Computer 2 - Player-Player

int X = 0;
int Y = 0;

int playersLife = 20;
int enemysLife = 20;

void PutShips(int masts, BOARD board [10][10]){

    bool repeat;

    do{
        repeat = false;
        int mod = 1;
        int x = rand()%10;
        int y = rand()%10;
        int vert = rand()%2;

        if(vert == 0){ //vertical == false -> put ship horizontal
            if(x+masts>9){
                mod = -1;
            }

            for(int i = 0; i<masts && i>(masts*(-1)); i+=mod){
                    //sprawdz czy statek nie zachodzi na inny lub nie styka sie z innym
                if(board[x+i][y].occupied == true || board[x+i+1][y].occupied == true || board[x-1][y].occupied == true || board[x+i-1][y].occupied == true || board[x][y+1].occupied == true || board[x][y-1].occupied == true || board[x+i+1][y-1].occupied == true || board[x+i-1][y-1].occupied == true || board[x+i+1][y+1].occupied == true || board[x+i-1][y+1].occupied == true){
                        repeat = true;
                }
            }

            if(repeat == false){
                for(int i = 0; i<masts && i>(masts*(-1)); i+=mod){
                    board[x+i][y].occupied = true;
                }
            }
        }
        else{
            if(y+masts>9){
                mod = -1;
            }
            for(int i = 0; i<masts && i>(masts*(-1)); i+=mod){
                if(board[x][y+i].occupied == true || board[x][y+i+1].occupied == true || board[x][y-1].occupied == true || board[x][y+i-1].occupied == true || board[x+1][y].occupied == true || board[x-1][y].occupied == true || board[x+i+1][y-1].occupied == true || board[x+i-1][y-1].occupied == true || board[x+i+1][y+1].occupied == true || board[x+i-1][y+1].occupied == true){
                        repeat = true;
                }
            }
            if(repeat == false){
                for(int i = 0; i<masts && i>(masts*(-1)); i+=mod){
                    board[x][y+i].occupied = true;
                }
            }
        }
    }while(repeat);
}

void GenerateMap(){

    for(int i=0; i <10;i++){
        for(int j=0;j<10;j++){
            enemysBoard[i][j].occupied=0;
            playersBoard[i][j].occupied=0;
        }
    }
	

    PutShips(4, enemysBoard);
    PutShips(3, enemysBoard);
    PutShips(3, enemysBoard);
    PutShips(2, enemysBoard);
    PutShips(2, enemysBoard);
    PutShips(2, enemysBoard);
    PutShips(1, enemysBoard);
    PutShips(1, enemysBoard);
    PutShips(1, enemysBoard);
    PutShips(1, enemysBoard);

    PutShips(4, playersBoard);
    PutShips(3, playersBoard);
    PutShips(3, playersBoard);
    PutShips(2, playersBoard);
    PutShips(2, playersBoard);
    PutShips(2, playersBoard);
    PutShips(1, playersBoard);
    PutShips(1, playersBoard);
    PutShips(1, playersBoard);
    PutShips(1, playersBoard);


    //UKRYJ POLA DLA NOWEJ MAPY
    for(int i=0;i<10;i++){
        for(int j=0; j<10;j++){
            enemysBoard[i][j].hidden = true;
            playersBoard[i][j].hidden = true;
        }
    }


    //stworz zapis mapy
    fstream file("settings.txt", ios::out);
    file << enemysLife << endl << playersLife << endl;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            file << enemysBoard[i][j].hidden << enemysBoard[i][j].occupied << playersBoard[i][j].hidden << playersBoard[i][j].occupied << endl;
        }
    }

    //wyswietl ustawienia mapy na strumieniu wyjscia
    cout << enemysLife << endl << playersLife << endl;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cout << enemysBoard[i][j].hidden << enemysBoard[i][j].occupied << playersBoard[i][j].hidden << playersBoard[i][j].occupied << endl;
        }
    }

}

int main(){
    srand(time(NULL));
    GenerateMap();
}
