#include <cstdlib>
#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>
#include "btls.h"

BOARD enemysBoard [10][10];
BOARD playersBoard [10][10];

char abc [10] = {'A','B','C','D','E','F', 'G', 'H', 'I', 'J'};

bool generateSetup;
bool playersTurn = true;
bool gameOver = false;

int playMode; // 0 - Computer-Player 1 - Computer-Computer 2 - Player-Player

int X = 0;
int Y = 0;
int xHitted;
int yHitted;

int count2a, count2b, count2c, count3a, count3b, count4; // zjebane liczniki
int count2ap, count2bp, count2cp, count3ap, count3bp, count4p; // zjebane liczniki dla playera

int playersLife = 20;
int enemysLife = 20;

using namespace std;

void LoadData(const char* file_name){
    string line;
    string tmp;
    string fileName;
    fstream file;

    file.open(file_name,ios::in);

    if(file.good()==false){
        cerr << "There is no file named " << file_name << ". I will generate one...\n\n"; // wyswietla blad na stumien bledow
        system("./generator");
        cout << "OK";
        //return 1; chyba nie ma potrzeby
    }
    else{
        int i = 0;
        int j = -2;//wczytaj zycie z dwoch pierwszych linijek save'a

        while(getline(file,line)){
            if(j==-2){
                enemysLife=atoi(line.c_str());
            }
            else if(j==-1){
                playersLife=atoi(line.c_str());
            }
            else{
                tmp=line[0];
                enemysBoard[i][j].hidden=atoi(tmp.c_str());
                tmp=line[1];
                enemysBoard[i][j].occupied=atoi(tmp.c_str());
                tmp=line[2];
                playersBoard[i][j].hidden=atoi(tmp.c_str());
                tmp=line[3];
                playersBoard[i][j].occupied=atoi(tmp.c_str());
            }
            j++;
            if(j==10){
                i++;
                j=0;
            }
        }
    }
    file.close();
}

void Check(BOARD board [10][10]){

    if (board[X][Y].hidden == true){

        //OTWORZ PLIK DO ZAPISU RUCHU
        fstream file("moves.txt", ios_base::app);

        board[X][Y].hidden = false;
        if(board[X][Y].occupied == true){
            cout << "Computer's ship HITTED! (pick again)\n\n";
            //ZAPISZ RUCH
            
            file << "Player hits: " << abc[X] << Y+1 << "\n";

            enemysLife--;
            if(enemysLife<=0){
                gameOver = true;
                return;
            }
        }
        else{
            cout << "MISS!\n\n";
            //ZAPISZ RUCH
            file << "Player misses: " << abc[X] << Y+1 << "\n";
            playersTurn = false;
        }

        file.close(); //ZAMKNIJ PLIK Z ZAPISEM RUCHOW
    }
    else{
        cout << "\n\nThis field is already uncovered! Pick again.\n";
    }

}

void ComputersMove(BOARD board[10][10]){
    if(playersLife <=0 || enemysLife<=0){
        gameOver = true;
        return;
    }
    
    bool repeat;
    int x;
    int y;
    
    do{
        repeat = false;
    
        x = rand()%10;
        y = rand()%10;
    
        //jesli poprzednio trafiono w cel, zamien wartosci losowe na takie z sasiedztwa statku
        if(xHitted != -1){
            //te if-y sa absolutnie zjebane, ale to jedyny sposob, zeby nie wykroczyc poza zakres tablicy i skutecznie znalezc cel
            if(xHitted-1>=0){
                 if(board[xHitted-1][yHitted].hidden==true){
                x = xHitted-1;
                y = yHitted;
                }
            }
            if(xHitted+1<=9){
                if(board[xHitted+1][yHitted].hidden==true){
                    x = xHitted+1;
                    y = yHitted;
                }
            }
            if(yHitted-1>=0){
                if(board[xHitted][yHitted-1].hidden==true){
                x = xHitted;
                y = yHitted-1;
                }
            }
            if(yHitted+1<=9){
                if(board[xHitted][yHitted+1].hidden==true){
                x = xHitted;
                y = yHitted+1;
                }
            }
        }
        
        //SPRAWDZ CZY TO POLE NIE BYLO WCZESNIEJ UDERZONE
        if(board[x][y].hidden==false){
            repeat = true;
        }
        else{
            //OTWORZ PLIK DO ZAPISU RUCHU
            fstream file("moves.txt", ios_base::app);
            
            if(file.good()==false){
                cerr << "Cannot make file with moves (propably no rights to write in the directory)"; // wyswietla blad na stumien bledow
            }

            board[x][y].hidden=false;
            if(board[x][y].occupied==true){
                cout << "Computer HITS! (" << abc[x] << y+1 << ")\n";
                //ZAPISZ RUCH
                file << "Computer hits: " << abc[x] << y+1 << "\n";
    
                repeat = true;
                xHitted = x;
                yHitted = y;
                playersLife--;
            }
            else{
                cout << "Computer MISSES! (" << abc[x] << y+1 << ")\n";
                //ZAPISZ RUCH
                file << "Computer misses: " << abc[x] << y+1 << "\n";
    
                //jezeli wszystkie pola dookola trafionego celu sa odkryte, zmien cel
                if(board[xHitted-1][yHitted].hidden==false && board[xHitted+1][yHitted].hidden==false&&board[xHitted][yHitted-1].hidden==false&&board[xHitted][yHitted+1].hidden==false){
                    xHitted = -1;
                    yHitted = -1;
                }

                file.close(); //ZAMKNIJ PLIK Z ZAPISEM RUCHOW
            }
        }
    
    }while(repeat);
}

void PlayersMove(){
    if(playersLife <=0 || enemysLife<=0){
        gameOver = true;
        return;
    }

    do{
        char cx;
        int x = -1;
        int y;

        do{
            cout << "\n\nEnter target coordinations: ";
            cin >> cx >> y;

            for(int i=0;i<10;i++){
                 if(abc[i] == cx){
                    x = i;
                }
            }
            if(x == -1 || y < 1 || y > 10){
                cerr << "Error: Coordinates out of range!\n";
            }
        }while(x == -1 || y < 1 || y > 10);

        X=x;
        Y=y-1;
        Check(enemysBoard);

    }while(playersTurn);
}

void PrepareGame(int argc, char * argv[]){
    //WCZYTAJ USTAWIENIA -U
    if(argc == 1){
        cerr << "You didn't specify map file. I'll load data from default file ('settings.txt')\n";
        LoadData("settings.txt");
    }
    else{
        for(int i = 0; i < argc; i++){
            if(string(argv[i]) == "-u" || string(argv[i]) == "--ustawienia"){
                if(! argv[i+1]){
                    cerr << "You didn't specify map file. Please try again!\n";
                }
                else{
                    cout << "Wczytuje " << argv[i+1] << "\n";
                    LoadData(argv[i+1]);
                }
            }
            //SPRAWDZ KOLEJNOSC GRACZY
            else if(string(argv[i]) == "-s" || string(argv[i]) == "--start"){
                playersTurn = false; //glupio to wyglada, ale poczatkowo myslalem, ze gracz to uzytkownik, a w tym kontekscie chyba jest na odwrot (gracz to program)
            }
        }
    }
    //WYZERUJ DOMYSLNY PLIK Z RUCHAMI
    fstream file("moves.txt",ios::out);
    file.close();
}

void Run(){
    do{
        if(playMode==0){
                if(playersTurn == true){
                PlayersMove();
            }
            else{
                ComputersMove(playersBoard);
                playersTurn = true;
            }
        }
        else if(playMode==1){
            if(playersTurn == true){
                ComputersMove(enemysBoard);
                playersTurn = false;
                sleep(1);
            }
            else{
                ComputersMove(playersBoard);
                playersTurn = true;
                sleep(1);
            }
        }
        else{
            if(playersTurn == true){
                PlayersMove();
                playersTurn = false;
            }
            else{
                playersTurn = true; // no to juz debilnie okodowane, ale nie myslalem na poczatku o multiplayerze i to jedyny sposob, zeby nie przebudowac calego programu
                PlayersMove();
                playersTurn = true;
            }
        }
        //ZAPISZ STAN GRY
        fstream file("settings.txt", ios::out);
        file << enemysLife << endl << playersLife << endl;
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                file << enemysBoard[i][j].hidden << enemysBoard[i][j].occupied << playersBoard[i][j].hidden << playersBoard[i][j].occupied << endl;
            }
        }
    }while(gameOver == false);
}

int main(int argc, char * argv[]){
    PrepareGame(argc, argv);
    Run();

//OTWORZ PLIK DO ZAPISU RUCHU
fstream file("moves.txt", ios_base::app);
//ZAPISZ INFORMACJE O KONCU:
file << "END\n";
file.close(); //ZAMKNIJ PLIK Z ZAPISEM RUCHOW
}
