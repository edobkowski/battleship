#include <cstdlib>
#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>
#include "btls.h"

void LoadData(const char* file_name){
    fstream file;
    string line;
    string tmp;

    file.open(file_name,ios::in);
    if(file.good()==false){
        cerr << "There is no file named " << file_name << ". Trying to load default file('moves.txt')...\n"; // wyswietla blad na stumien bledow
    }
    else{
        while(getline(file,line)){
            /*HEHEHEH... ZASTOSUJEMY TUTAJ TO SAMO OBLESNE ROZWIAZANIE CO W DISPLAY.CPP LELZXDXDXDXD*/
            tmp = line[0];
            if(tmp == "P"){ //Tzn ze byl to ruch playera
            }
        }
    }
    file.close();
}

int main(int argc, char * argv[]){
    if(argc == 1){
        cerr << "You didn't specify moves file. I'll load data from default file ('moves.txt')\n";
        LoadData("moves.txt");
    }
    else{
        for(int i = 0; i < argc; i++){
            if(string(argv[i]) == "-r" || string(argv[i]) == "--ruchy"){
                if(! argv[i+1]){
                    cerr << "You didn't specify moves file. Please try again!\n";
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
}
