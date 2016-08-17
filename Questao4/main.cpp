#include <bits/stdc++.h>
using namespace std;

enum Player{Null,X,O};

typedef struct board {
    board() {
        data = new Player*[3];
        for(int i = 0; i < 3; i++)
            data[i] = new Player[3];
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                data[i][j] = Null;
        score = 0;
    }
    Player **data;
    int score;
} Board;

Board *mainBoard;

Player human = X;
Player ia = O;

Player checkVictory(Board *current) {
    bool check = true;
    Player winner = Null;

    //Checar Diagonal E/D
    for(int i = 2; i > 0; i--){
        if(current->data[i][i] != current->data[i-1][i-1] ||
                current->data[i][i] == Null||
                current->data[i - 1][i - 1] == Null){
            check = false;
            break;
        }
        else winner = current->data[i][i];
    }

    if(check && winner != Null) {
        //        cout << "WINNER E/D: " << winner << endl;
        return winner;
    }
    check = true;
    winner = Null;

    //Checar Diagonal D/E
    for(int i = 2; i > 0; i--){
        if(current->data[i][2-i] != current->data[i - 1][2 - i + 1] ||
                current->data[i][2-i] == Null||
                current->data[i - 1][2 - i + 1] == Null){
            check = false;
            break;
        }
        else winner = current->data[i][2-i];
    }

    if(check && winner != Null) {
        //        cout << "WINNER D/E: " << winner << endl;
        return winner;
    }
    check = true;
    winner = Null;

    //Checar Linhas
    for(int i = 0; i < 3; i++){
        for(int j = 2; j > 0; j--){
            if(current->data[i][j] != current->data[i][j - 1] ||
                    current->data[i][j] == Null||
                    current->data[i][j - 1] == Null){
                check = false;
                break;
            }
            else{
                check = true;
                winner = current->data[i][j];
            }
        }
        if(check && winner != Null) {
            //            cout << "WINNER LINES: " << winner << endl;
            return winner;
        }
    }

    check = true;
    winner = Null;

    //Checar Colunas
    for(int i = 0; i < 3; i++){
        for(int j = 2; j > 0; j--){
            if(current->data[j][i] != current->data[j-1][i] ||
                    current->data[j][i] == Null||
                    current->data[j - 1][i] == Null){
                check = false;
                break;
            }
            else{
                check = true;
                winner = current->data[j][i];
            }
        }
        if(check && winner != Null) {
            //            cout << "WINNER COLUMNS: " << winner << endl;
            return winner;
        }
    }
    return Null;
}

Player turnStep(Player current) {
    return (current == human) ? ia : human;
}

Board *generateCopy(Board *current) {
    Board *board = new Board();
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            board->data[i][j] = current->data[i][j];
    return board;
}

Board *setSpaceData(int row, int col, Player data, Board *current) {
    if(current->data[row][col] != Null) return NULL;
    current->data[row][col] = data;
    return current;
}


bool compareMax (const Board *a, const Board *b) {
    return (a->score > b->score);
}
bool compareMin (const Board *a, const Board *b) {
    return (a->score < b->score);
}

vector<Board *> generateNextMoves(Board *current, Player turn, int depth) {
    vector<Board *> moves;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(current->data[i][j] == Null)
                moves.push_back(setSpaceData(i,j,turn,generateCopy(current)));
        }
    }
    for(Board *move : moves) {
        Player winner = checkVictory(move);
        if(winner == human)
            move->score = -1000 + depth;
        else if(winner == ia)
            move->score = 1000 - depth;
        else
            move->score = 0;
    }
    if(turn == ia)
        sort(moves.begin(),moves.end(), compareMax);
    else if(turn == human)
        sort(moves.begin(),moves.end(), compareMin);
    return moves;
}

void print(Board *current) {
    cout << "\t     0   1   2" << endl;
    for(int i = 0; i < 3; i++){
        cout << "\t " << i << " | ";
        for(int j = 0; j < 3; j++){
            switch (current->data[i][j]) {
            case Null:
                cout << "  | ";
                break;
            case X:
                cout << "X | ";
                break;
            case O:
                cout << "O | ";
                break;
            }
        }
        cout << endl;
    }
    cout << "==================================" << endl;
    //    cout << "\t        " << current->score << endl;
}

bool checkAnswer(int col, int row, Board *current) {
    if(col > 2 || col < 0 || row < 0 || row > 2) {
        cout << "=============  ERRO  =============" << endl;
        cout << "Posição selecionada não é válida!\n";
        return false;
    }
    if(current->data[row][col] != Null) {
        cout << "=============  ERRO  =============" << endl;
        cout << "Posição selecionada já está ocupada!\n";
        return false;
    }
    return true;
}

void humanTurn(Board *current) {
    int col = 0, row = 0;
    bool check = false;
    while(!check) {
        cout << "=============  MENU  =============" << endl;
        cout << "Digite a Linhas e a Coluna da sua" << endl
             << "jogada:" << endl;
        cin >> row;
        cin >> col;
        check = checkAnswer(col,row,current);
    }
    setSpaceData(row,col,human,current);
    cout << "============= HUMANO =============" << endl;
}

void copyAllData(Board *target, Board *source) {
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            target->data[i][j] = source->data[i][j];
}

int minimax(Board *current, Player turn, int depth) {
    vector<Board *> moves = generateNextMoves(current, turn, depth);
    for(Board *move : moves) {
        if(move->score == 0) {
            move->score = minimax(move, turnStep(turn), depth + 1);
        }
    }
    if(turn == ia)
        sort(moves.begin(),moves.end(), compareMax);
    else if(turn == human)
        sort(moves.begin(),moves.end(), compareMin);
    if(depth == 1) {
        copyAllData(current, moves.at(0));
    }
    if(moves.size() > 0)
        return moves[0]->score;
    else
        return current->score;
}

Player selectFirstOne() {
    cout << "=============  MENU  =============" << endl;
    cout << "Quem começa jogando?\n\t1)Humano\n\t2)IA\n";
    int option;
    cin >> option;
    if(option > 2 || option < 1) {
        cout << "=============  ERRO  =============" << endl;
        cout << "Escolha uma das opções listadas!" << endl;
        return selectFirstOne();
    }
    else if(option == 1)
        return human;
    else if(option == 2)
        return ia;
}

int main()
{
    Player currentTurn;
    mainBoard = new Board();
    currentTurn = selectFirstOne();
    cout << "=============  JOGO  =============" << endl;
    while(checkVictory(mainBoard) == 0) {
        print(mainBoard);
        if(currentTurn == human) {
            humanTurn(mainBoard);
        } else if(currentTurn == ia) {
            cout << "=============   IA   =============" << endl;
            minimax(mainBoard, ia, 1);
        }
        currentTurn = turnStep(currentTurn);
    }
    print(mainBoard);
    currentTurn = turnStep(currentTurn);
    cout << "============ VENCEDOR ============" << endl;
    if(currentTurn == human)
        cout << "============= HUMANO =============" << endl;
    else if(currentTurn == ia)
        cout << "=============   IA   =============" << endl;
    cout << "==================================" << endl;
    return 0;
}
