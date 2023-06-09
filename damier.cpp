#include "damier.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

Damier::Damier(int l, int c, int vd)
{
    Alloc(l, c);
    Init();

}

Damier::Damier(const Damier &D)
{
    Alloc(D.L, D.C);
    for(int i=0; i<L; i++)
        for(int j=0; j<C; j++)
            T[i][j] = D.T[i][j];
}


Damier::~Damier(){
    if (T != NULL) {
        Free();
        T = NULL;
    }
}

void Damier::Free(){
    for (int i=0; i<L; i++) {
        delete [] T[i];
    }
    delete [] T;
}

void Damier::Alloc(int l, int c){
    L = l;
    C = c;
    T = new int*[L];
    for(int i=0; i<L; i++)
        T[i] = new int[C];
}

void Damier::Print(){
    cout << endl;
    for(int i=0; i<L; i++) {
        cout << endl;
        for(int j=0; j<C; j++)
            cout << T[i][j] << ", ";
    }
}

void Damier::Init(){
    for(int i=0; i<L; i++)
        for(int j=0; j<C; j++)
            T[i][j]= 0;
    score = 0;
    srand(time(0));
    int i_0 = rand() % L;
    int j_0 = rand() % C;
    int valeur = (rand() % 2 + 1)*2;
    T[i_0][j_0] = valeur;
}

void Damier::Set(int l, int c, int value) {
    if ((l>=0) && (l<L) && (c>=0) & (c<C))
        T[l][c]=value;
}


void Damier::ReDim(int l, int c, int vd) {
    Free();
    Alloc(l, c);
    Init();
}

// La fonction spawn fait apparaître un 2 ou un 4 sur une case aléatoire

void Damier::Spawn(){
    // On répertorie l'ensemble des indices des cases vides
    vector<tuple<int,int>> case_vide;
    for(int i=0;i<L;i++){
        for(int j=0;j<C;j++){
            if(T[i][j]==0){
                case_vide.insert(case_vide.end(),{i,j});
            }
        }
    }
    int n = case_vide.size();
    if(n==0){
        loose = true; // si aucune case n'et libre, la partie est perdue
    }
    else{
        srand(time(0)); // On choisit le temps comme seed pour le random generator pour ne pas avoir toujours la même séquence
        tuple<int,int> indice_0 = case_vide.at(rand() % n); // sinon, on choisit une case au hasard pour y faire apparaitre un 2 ou un 4
        int valeur = (rand() % 2 + 1)*2;
        T[get<0>(indice_0)][get<1>(indice_0)] = valeur;
    }
    //int i_0 = rand() % L;
    //int j_0 = rand() % C;
}


std::vector<std::vector<int>> Damier::getBoard() const {
    std::vector<std::vector<int>> board(L, std::vector<int>(C, 0));
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            board[i][j] = T[i][j];
        }
    }
    return board;
}

QVector<QVector<int>> Damier::getBoardAsQvector() const{
    QVector<QVector<int>> qBoard(L, QVector<int>(L));
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            qBoard[i][j] = T[i][j];
        }
    }
    return qBoard;

}

/*void Damier::boardChanged() {
    emit boardChanged();
}*/

// Quand le joueur joue vers le haut
void Damier::play_up(){
    // On créer un vecteur qui mémorise si la case a déjà fusionné, auquel cas elle ne peut pas fusionner à nouveau
    vector<vector<bool>> tab_fusion(L,vector<bool>(C, false));
    for(int col=0;col<C;col++){
        for(int ligne = 0;ligne<L-1;ligne++){
            bool bloque = false;
            int i = 1;
            while(bloque==false && ligne+i>0){ // Si la case n'est pas bloquée, on la traite
                if(T[ligne+i-1][col]==0){ // si la case adjacente est vide la case traitée doit prendre sa place
                    if(T[ligne+i][col]!=0){ // si la case est vide pas besoin de traiter, sinon on la déplace
                        T[ligne+i-1][col] = T[ligne+i][col];
                        T[ligne+i][col] = 0;
                    }
                }
                else{
                    if(T[ligne+i][col] == T[ligne+i-1][col] && tab_fusion[ligne+i-1][col]== false){ // Si la case est bloquée par une case identique, on les fusionne
                        T[ligne+i-1][col] = 2*T[ligne+i-1][col];
                        score += T[ligne+i-1][col];
                        T[ligne+i][col] = 0;
                        tab_fusion[ligne+i-1][col]= true; // indique que cette case a été fusionné et ne pourra donc plus l'être pour ce play
                        }
                    bloque = true;
                    }
                i = i-1;
            }
        }
    }
    Spawn();
    //emit boardChanged();


}


// Quand le joueur joue vers le bas
void Damier::play_down(){
    // On créer un vecteur qui mémorise si la case a déjà fusionné, auquel cas elle ne peut pas fusionner à nouveau
    vector<vector<bool>> tab_fusion(L,vector<bool>(C, false));
    for(int col=0;col<C;col++){
        for(int ligne = L-2;ligne>=0;ligne--){
            bool bloque = false;
            int i = 1;
            while(bloque==false && ligne+i<L){ // Si la case n'est pas bloquée, on la traite
                if(T[ligne+i][col]==0){ // si la case adjacente est vide la case traitée doit prendre sa place
                    if(T[ligne+i-1][col]!=0){ // si la case est vide pas besoin de traiter, sinon on la déplace
                        T[ligne+i][col] = T[ligne+i-1][col];
                        T[ligne+i-1][col] = 0;
                    }
                }
                else{
                    if(T[ligne+i][col] == T[ligne+i-1][col] && tab_fusion[ligne+i][col]== false){ // Si la case est bloquée par une case identique, on les fusionne
                        T[ligne+i][col] = 2*T[ligne+i][col];
                        score+=T[ligne+i][col];
                        T[ligne+i-1][col] = 0;
                        tab_fusion[ligne+i][col]= true; // indique que cette case a été fusionné et ne pourra donc plus l'être pour ce play
                        }
                    bloque = true;
                    }
                i = i+1;
            }
        }
    }
    Spawn();
}

// Quand le joueur joue vers la gauche
void Damier::play_left(){
    // On créer un vecteur qui mémorise si la case a déjà fusionné, auquel cas elle ne peut pas fusionner à nouveau
    vector<vector<bool>> tab_fusion(L,vector<bool>(C, false));
    for(int col=0;col<C-1;col++){
        for(int ligne = 0;ligne<L;ligne++){
            bool bloque = false;
            int i = 1;
            while(bloque==false && col+i>0){ // Si la case n'est pas bloquée, on la traite
                if(T[ligne][col+i-1]==0){ // si la case adjacente est vide la case traitée doit prendre sa place
                    if(T[ligne][col+i]!=0){ // si la case est vide pas besoin de traiter, sinon on la déplace
                        T[ligne][col+i-1] = T[ligne][col+i];
                        T[ligne][col+i] = 0;
                    }
                }
                else{
                    if(T[ligne][col+i] == T[ligne][col+i-1] && tab_fusion[ligne][col+i-1]== false){ // Si la case est bloquée par une case identique, on les fusionne
                        T[ligne][col+i-1] = 2*T[ligne][col+i-1];
                        score+=T[ligne][col+i-1];
                        T[ligne][col+i] = 0;
                        tab_fusion[ligne][col+i-1]= true; // indique que cette case a été fusionné et ne pourra donc plus l'être pour ce play
                        }
                    bloque = true;
                    }
                i = i-1;
            }
        }
    }
    Spawn();
}

// Quand le joueur joue vers la droite
void Damier::play_right(){
    // On créer un vecteur qui mémorise si la case a déjà fusionné, auquel cas elle ne peut pas fusionner à nouveau
    vector<vector<bool>> tab_fusion(L,vector<bool>(C, false));
    for(int col=C-2;col>=0;col--){
        for(int ligne = 0;ligne<L;ligne++){
            bool bloque = false;
            int i = 1;
            while(bloque==false && col+i<L){ // Si la case n'est pas bloquée, on la traite
                if(T[ligne][col+i]==0){ // si la case adjacente est vide la case traitée doit prendre sa place
                    if(T[ligne][col+i-1]!=0){ // si la case est vide pas besoin de traiter, sinon on la déplace
                        T[ligne][col+i] = T[ligne][col+i-1];
                        T[ligne][col+i-1] = 0;
                    }
                }
                else{
                    if(T[ligne][col+i] == T[ligne][col+i-1] && tab_fusion[ligne][col+i]== false){ // Si la case est bloquée par une case identique, on les fusionne
                        T[ligne][col+i] = 2*T[ligne][col+i];
                        score+= T[ligne][col+i];
                        T[ligne][col+i-1] = 0;
                        tab_fusion[ligne][col+i]= true; // indique que cette case a été fusionné et ne pourra donc plus l'être pour ce play
                        }
                    bloque = true;
                    }
                i = i+1;
            }
        }
    }
    Spawn();
}



QVector<QVector<int>> Damier::readBoard() const{
    QVector<QVector<int>> qBoard(L, QVector<int>(L));
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            qBoard[i][j] = T[i][j];
        }
    }
    return qBoard;
}

Damier& Damier::operator= (const Damier &D){
    if ( this != &D) { // protection autoréférence
        Free();
        Alloc(D.L, D.C);
        for(int i=0; i<L; i++)
            for(int j=0; j<C; j++)
                T[i][j] = D.T[i][j];
    }
    return *this;
}
