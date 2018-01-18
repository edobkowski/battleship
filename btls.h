struct BOARD{
    bool hidden;
    bool occupied;
};

enum colorNumber{
        White = 0,
        Yellow = 1,
        Red = 2,
        Pink = 3,
        Green = 4,
        Black = 5,
        Blue = 6
    };

void Start();

void GameOver();

void Run();

void DrawBoard(BOARD board[10][10]);

void Check(BOARD board [10][10]);

void PlayersMove();

void ComputersMove(BOARD board[10][10]);

void PutShips(int masts, BOARD board [10][10]);

void ShowFormattedMsg(const char * msg);

void PrepareGame();

void Help();

void Settings();

void LoadSettings();

void LoadData();

void GenerateMap();

void Enter();

int ShowMenu();
