#include <bits/stdc++.h>
using namespace std;

enum Side { Left, Right };
enum Move { CC,C,MM,M,CM };
typedef struct state {
    state(int cn, int mn, Side sn) {
        c = cn;
        m = mn;
        s = sn;
    }
    int c;
    int m;
    Side s;
} State;

vector<State *> visited;
State *initial;

//Checa se foi obtido o estado desejado
bool checkGoal(State *current) {
    if(current->s == initial->s && current->c == 0 && current->m == 0)
        return true;
    if(current->s != initial->s && current->c == 3 && current->m == 3)
        return true;
    return false;
}

//Compara dois estados
bool compare(State *a, State *b) {
    if(a->c != b->c) return false;
    if(a->m != b->m) return false;
    if(a->s != b->s) return false;
    return true;
}

//Checa se o estado já foi visitado
bool wasItVisitedAlready(State *current) {
    int i = 0;
    for(State *state : visited)
        if(compare(current,state))
            return true;
    return false;
}

vector<Move> generateNextMoves(State *current) {
    vector<Move> moves;
    //Checa se é válido passar dois canibais para o outro lado
    if(current->c >= 2 && (3 - current->m == 0 || (3 - current->m) - (3 - current->c) >= 2))
        moves.push_back(CC);
    //Checa se é válido passar um canibal para o outro lado
    if(current->c >= 1 && (3 - current->m == 0 || (3 - current->m) - (3 - current->c) >= 1))
        moves.push_back(C);
    //Checa se é válido passar dois missionarios para o outro lado
    if(current->m >= 2 &&
            (current->m - 2 == 0 ||
             (current->m - 2 >= current->c && (3 - current->c == 0)) ||
             (3 - current->c) - (3 - current->m) <= 2))
        moves.push_back(MM);
    //Checa se é válido passar um missionario para o outro lado
    if(current->m >= 1 &&
            (current->m - 1 == 0 ||
             (current->m - 1 >= current->c && (3 - current->c == 0)) ||
             (3 - current->c) - (3 - current->m) <= 1))
        moves.push_back(M);
    //Checa se é válido passar um canibal e um missionario para o outro lado
    if(current->c >= 1 && current->m >= 1 && (3 - current->c) <= (3 - current->m))
        moves.push_back(CM);
    return moves;
}

//Imprime o estado
void print(State *current) {
    cout << "\t   |   " << endl;
    switch(current->s) {
    case Left:
        cout << "\t " << current->m << " | " << 3 - current->m << " " << endl;
        cout << "\t " << current->c << " | " << 3 - current->c << " " << endl;
        cout << "\t   |   " << endl;
        cout << "\t  LEFT" << endl;
        break;
    case Right:
        cout << "\t " << 3 - current->m << " | " << current->m << " " << endl;
        cout << "\t " << 3 - current->c << " | " << current->c << " " << endl;
        cout << "\t   |   " << endl;
        cout << "\t RIGHT" << endl;
        break;
    }
    cout << endl;
}

//Retorna uma cópia do estado atual
State *generateCopy(State *current) {
    return new State(current->c,current->m,current->s,current);
}

//Define as propriedades de um estado
State *setState(State *current, int c, int m) {
    current->s = (current->s == Left) ? Right : Left;
    current->c = (3 - current->c) + c;
    current->m = (3 - current->m) + m;
    return current;
}

//Aplica o movimento requerido em um estado
State *applyMove(State *current, Move move) {
    switch(move) {
    case CC:
        return setState(current,2,0);
    case C:
        return setState(current,1,0);
    case MM:
        return setState(current,0,2);
    case M:
        return setState(current,0,1);
    case CM:
        return setState(current,1,1);
    }
}

//Árvore de busca por profundidade
int depthSearchTree(State *current, int depth) {
    //Checa se o estado atual é o desejado
    if(checkGoal(current)) {
        visited.push_back(current);
        return 0;
    }
    //Checa se o estado atual ja foi visitado
    //  Se sim, ele retorna a profundidade do estado
    if(wasItVisitedAlready(current)) return depth;
    //Adiciona o estado atual a lista de estados visitados
    visited.push_back(current);
    //Adiciona a um vetor os próximos possíveis movimentos
    vector<Move> moves = generateNextMoves(current);
    if(moves.size() == 0) return depth;
    //Para cada possível movimento ele aplica a ação
    for(Move m : moves) {
        int result = depthSearchTree(applyMove(generateCopy(current),m),depth + 1);
        if(result  == 0)
            return 0;
        else
            visited.erase(visited.begin() + depth, visited.end());
    }
}

int main() {
    //Gera o estado inicial
    initial = new State(3,3,Right, NULL);
    int result = depthSearchTree(initial,1);
    if(result == 0)
        for(State *s : visited)
            print(s);
    else
        cout << "SOLUTION NO FOUNDED!" << endl;
    return 0;
}
