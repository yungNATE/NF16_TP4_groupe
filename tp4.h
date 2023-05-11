#include <stdio.h>
#include <stdlib.h>

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