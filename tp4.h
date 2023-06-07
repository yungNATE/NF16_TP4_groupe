#ifndef TP4_H_INCLUDED
#define TP4_H_INCLUDED

#define MAX_WORD_LENGTH 100

// Structures et types
struct Position {
    int numeroLigne;
    int ordre;
    int numeroPhrase;
    struct Position* position_suivante;
    struct Position* mot_suivant;
    struct Noeud* noeud_parent;
};
typedef struct Position T_Position;

struct Noeud {
    char* mot;
    int nbOccurences;
    T_Position* listePositions;
    T_Position* derniere_position;
    struct Noeud* filsGauche;
    struct Noeud* filsDroit;
};
typedef struct Noeud T_Noeud;

struct Phrase {
    int numero;
    T_Position* premiermot;
    T_Position* derniermot;
    struct Phrase* suivant;
    struct Phrase* precedent;
};
typedef struct Phrase T_Phrase;

struct Texte {
    T_Phrase* premiere;
    T_Phrase* derniere;
};
typedef struct Texte T_Texte;



struct Index {
    T_Texte* texte;
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

// Création des structures
T_Position *creerPosition(int ligne, int ordre, int phrase);

T_Noeud *creerNoeud(char *mot, int ligne, int ordre, int phrase);

T_Index *creerIndex();

T_Phrase *creer_phrase(int numero);

T_Texte *creer_texte();

//Ajout d'une position
T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase);

// Indexer le fichier
int indexerFichier(T_Index *index, char *filename);

// Afficher l'index
void afficherIndex(T_Index index);

// Recherche d'un mot dans l'index
T_Noeud* rechercherMot(T_Index index, char *mot);

// Afficher les occurences d'un mot
void afficherOccurencesMot(T_Index index, char *mot);

// Gestion pile
T_Pile *creer_pile(int size);

int pile_vide(T_Pile *p);

int pile_pleine(T_Pile *p);

int empiler(T_Pile *p, T_Noeud *noeud);

T_Noeud *depiler(T_Pile *p);


// Fonctions auxiliaires

// Transforme la première lettre en majuscule
void premiereLettreEnMajuscule(char *mot);

// Affiche un mot dans la console
void printWord(T_Position *pos);

// Ecrit un mot dans le fichier
void fprintWord(T_Position *pos, FILE *f, int maj);

// Vide le buffer
void viderBuffer();

// Lit un mot écrit au clavier
char *getStringInput(char *prompt);

// Test si l'index existe
void *isIndexSet(T_Index *index, bool shouldWarnUser);

// Remplace le caractère de nouvelle ligne avec la fin du string, utile dans certain cas d'affichage
void replaceNewLine_WithNullTerminator(char *str);

// Libère la mémoire
void deallocateIndex(T_Index *index);

void deallocateArbre(T_Noeud *noeud);

#endif // TP4_H_INCLUDED