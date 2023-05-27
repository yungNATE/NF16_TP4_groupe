#ifndef TP4_H_INCLUDED
#define TP4_H_INCLUDED

#define MAX_WORD_LENGTH 100

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


typedef struct T_Pile {
    int sommet;
    int size;
    T_Noeud *tab;
} T_Pile;

// Fonctions

//Affichage positions d'un même mot
void afficherPosition(T_Position *listeP);

//Ajout d'une position
T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase);

T_Position *creerPosition(int ligne, int ordre, int phrase);

T_Noeud *creerNoeud(char *mot, int ligne, int ordre, int phrase);

int indexerFichier(T_Index *index, char *filename);

void afficherIndex(T_Index index);

T_Index *creerIndex();

T_Noeud* rechercherMot(T_Index index, char *mot);


// Gestion pile

T_Pile *creer_pile(int size);
int pile_vide(T_Pile *p);
int pile_pleine(T_Pile *p);
int empiler(T_Pile *p, T_Noeud *noeud);
T_Noeud *depiler(T_Pile *p);


void drawBinaryTree(T_Noeud* node, int level);
void drawIndexTree(T_Index* index);

#endif // TP4_H_INCLUDED
