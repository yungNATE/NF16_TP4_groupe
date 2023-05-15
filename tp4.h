#ifndef TP4_H_INCLUDED
#define TP4_H_INCLUDED

// Structures et types
struct Position {
    int numeroLigne;
    int ordre;
    int numeroPhrase;
    struct Position* suivant;
};
typedef struct Position T_Position;

struct Noeud {
    char* mot;
    int nbOccurences;
    T_Position* listePositions;
    struct Noeud* filsGauche;
    struct Noeud* filsDroit;
};
typedef struct Noeud T_Noeud;


struct Index {
    T_Noeud* racine;
    int nbMotsDistincts;
    int nbMotsTotal;
};
typedef struct Index T_Index;

// Fonctions

//Affichage positions d'un même mot
void afficherPosition(T_Position *listeP);

//Ajout d'une position
T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase);

T_Position *creerPosition(int ligne, int ordre, int phrase);

#endif // TP4_H_INCLUDED
