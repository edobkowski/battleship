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

int playersLife;
int enemysLife;

BOARD enemysBoard [10][10];
BOARD playersBoard [10][10];

char abc [10] = {'A','B','C','D','E','F', 'G', 'H', 'I', 'J'};
string line;

void DrawBoard(BOARD board[10][10]){
    cout << "   ";
    for (int i = 0; i < 10; i++){
        cout << i+1 << " ";
    }

    cout << endl;

    for (int i = 0; i < 10; i++){
        cout << abc[i] << "  ";
        for (int j = 0; j < 10; j++){
            if(board[i][j].hidden){
                cout << "* ";
            }
            else{
                if(board[i][j].occupied){
                    cout << "\033[31;41m"; //31 = red text | 41 = red background
                    cout << "X ";
                    ;cout << "\033[0m";
                }
                else{
                    cout << "\033[1;44m";
                    cout << "  ";
                    cout << "\033[0m";
                }
            }
        } cout << endl;
    }
}

void Load(){
    cin >> playersLife;
    cin >> enemysLife;
    string tmp;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            cin >> line;
            tmp = line[0];
            
            enemysBoard[i][j].hidden = atoi(tmp.c_str());
            tmp = line[1];
            enemysBoard[i][j].occupied = atoi(tmp.c_str());
            tmp = line[2];
            playersBoard[i][j].hidden = 0;//atoi(tmp.c_str());
            tmp = line[3];
            playersBoard[i][j].occupied = atoi(tmp.c_str());
        }
    }
}

void fillBoard(string line){
    int x;
    int y;
    string tmp;
    /*---------------------UWAGA W TYM MOMENCIE JESTESCIE PANSTWO SWIADKAMI ABSOLUTNIE NAJGLUPSZEGO,
    NAJBARDZIEJ NIEDOROZWINIETEGO ROZWIAZANIA PROBLEMU ZWIAZANEGO Z OPERACJAMI NA LANCUCHACH ZNAKOW
    NIEMNIEJ JEDNAK RZYGAC MI SIE CHCE GDY MYSLE O SZUKANIU ELEGANCKICH ROZWIAZAN I PISANIU WLASNYCH
    FUNKCJI DO DZIELENIA STRINGOW NA PODLANCUCHY, A W SPECYFIKACJI NIE BYLO SLOWA O TYM, ZE KOD MA BYC
    LADNY I CZYTELNY, WIEC... PODZIWIAJCIE NAJBARDZIEJ UPOSLEDZONY KAWALEK KODU, JAKI KIEDYKOLWIEK
    MIALEM PRZYJEMNOSC SPLODZIC---------------------------------------------------------------------*/
    tmp = line[0];
    if(tmp == "P"){ //Tzn ze byl to ruch playera
        //cout << "P" << endl;
        tmp = line[7];
        if(tmp == "h"){ //Player hits
            //cout << "h" << endl;
            for(int i=0;i<10;i++){
                if(abc[i] == line[13]){ //sprawdz indeks literki z koordow
                   x = i;
               }
            }

            if(line.length() == 15){ //y: 1-9
                tmp = line[14];
                y = atoi(tmp.c_str())-1;
            }
            else{ //y: 10
                y = 9;
            }
            //cout << endl << endl << x << y << endl;
            enemysBoard[x][y].occupied = true; // trafiony, wiec jest tam statek
            enemysBoard[x][y].hidden = false;
        }
        else{ //Player misses
            for(int i=0;i<10;i++){
                if(abc[i] == line[15]){ //sprawdz indeks literki z koordow
                   x = i;
               }
            }

            if(line.length() == 17){ //y: 1-9
                tmp = line[16];
                y = atoi(tmp.c_str())-1;
            }
            else{ //y: 10
                y = 9;
            }
            enemysBoard[x][y].occupied = false; // pudlo, wiec brak statku
            enemysBoard[x][y].hidden = false;
        }
        
    }
    else{ //Ruch komputera
        tmp = line[9];
        if(tmp == "h"){ //Computer hits
            for(int i=0;i<10;i++){
                if(abc[i] == line[15]){ //sprawdz indeks literki z koordow
                   x = i;
               }
            }

            if(line.length() == 17){ //y: 1-9
                tmp = line[16];
                y = atoi(tmp.c_str())-1;
            }
            else{ //y: 10
                y = 9;
            }
            playersBoard[x][y].occupied = true;
        }
        else{ //Computer misses
            for(int i=0;i<10;i++){
                if(abc[i] == line[17]){ //sprawdz indeks literki z koordow
                   x = i;
               }
            }
            if(line.length() == 19){ //y: 1-9
                tmp = line[18];
                y = atoi(tmp.c_str())-1;
            }
            else{
                y = 9;
            }
            playersBoard[x][y].occupied = false;
        }
        playersBoard[x][y].hidden = false;
    }
}

void LoadFromFile(const char* file_name){
    //UKRYJ POCZATKOWO WSZYSTKIE POLA
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            enemysBoard[i][j].hidden = true;
            playersBoard[i][j].hidden = true;
        }
    }

    fstream file;

    file.open(file_name,ios::in);
    if(file.good()==false){
        cerr << "There is no file named " << file_name << ". Trying to load default file('moves.txt')...\n"; // wyswietla blad na stumien bledow
        
        file.open("moves.txt",ios::in);
        if(file.good()==false){
            cerr << "There is no default moves file!";
        }
        else{
            string line;
            while(getline(file,line)){
                fillBoard(line);
            }
        }
    }
    else{
        string line;
        while(getline(file,line)){
            fillBoard(line);
        }
    }
}

int main(int argc, char* argv[])
{
    //JEZELI NIE WPROWADZONO FLAGI:
    if(argc == 1){ 
        Load();
    }

    // ODTWORZ PLANSZE PO RUCHACH:
    else{
        for(int i = 0; i < argc; i++){
            if(string(argv[i]) == "-r" || string(argv[i]) == "--ruchy"){
                if(! argv[i+1]){
                    cerr << "You didn't specify moves file. Please try again!\n";
                }
                else{
                    cout << "Wczytuje " << argv[i+1] << "\n";
                    LoadFromFile(argv[i+1]);
                }
            }
        }

    }
    
    //WYSWIETL PLANSZE:
    system("clear");
    cout << "Your board:\n\n";
    DrawBoard(playersBoard);
    cout << "\nOpponent's board:\n\n";
    DrawBoard(enemysBoard);
}
