#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "tp4.h"

T_Pile *creer_pile(int size){
    T_Pile *p = malloc(sizeof(T_Pile));
    p->sommet = -1;
    p->size = size;
    p->tab = malloc(size * sizeof(T_Noeud));
    return p;
}

int pile_vide (T_Pile *p){
    return (p->sommet == -1);
}

int pile_pleine (T_Pile *p){
    return (p->sommet == p->size-1);
}

int empiler(T_Pile *p, T_Noeud *noeud){
    if (pile_pleine(p)){
        return 0;
    }

    else {
        p->sommet++;
        p->tab[p->sommet] = *noeud;
        return 1;
    }
}

T_Noeud *depiler(T_Pile *p){
    if (pile_vide(p)){
        return NULL;
    }

    else {
        p->sommet--;
        return &(p->tab[p->sommet+1]);
    }
}

void afficherPosition(T_Position *listeP){
    while (listeP != NULL){
        printf("Ligne: %d, Ordre: %d, Phrase: %d\n", listeP->numeroLigne, listeP->ordre, listeP->numeroPhrase);
        listeP = listeP->suivant;
    }
}

T_Position *creerPosition(int ligne, int ordre, int phrase){

    T_Position *nouveau = malloc(sizeof(T_Position));
    if (nouveau == NULL) return NULL;

    nouveau->numeroLigne = ligne;
    nouveau->ordre = ordre;
    nouveau->numeroPhrase = phrase;
    nouveau->suivant = NULL;
    
    return nouveau;
}

T_Noeud *creerNoeud(char *mot, int ligne, int ordre, int phrase){

    T_Noeud *nouveauNoeud = (T_Noeud*) malloc(sizeof(T_Noeud));
    if (nouveauNoeud == NULL) return NULL;

    T_Position *nouvellePosition = creerPosition(ligne, ordre, phrase);
    
    nouveauNoeud->mot = strdup(mot);
    nouveauNoeud->nbOccurences = 1;
    nouveauNoeud->listePositions = NULL;
    nouveauNoeud->filsGauche = NULL;
    nouveauNoeud->filsDroit = NULL;
    nouveauNoeud->listePositions = nouvellePosition;

    return nouveauNoeud;

}

T_Index *creerIndex(){

    T_Index *nouveau = (T_Index*) malloc(sizeof(T_Index));
    if (nouveau == NULL) return NULL;

    nouveau->nbMotsDistincts = 0;
    nouveau->nbMotsTotal = 0;
    nouveau->racine = NULL;

    return nouveau;
}

T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase){

    // La gestion du mauvais ordre d'insertion est gérée dans ajouterOccurence
    // Création position

    T_Position *nouveau = creerPosition(ligne, ordre, phrase);
    T_Position *iter = listeP;
    T_Position *precedent = NULL;

    // Chercher la bonne position

    while (iter != NULL){

        if (iter->numeroLigne < ligne) // Parcourir les lignes
        {   
            precedent = iter;
            iter = iter->suivant;
        }

        else if (iter->numeroLigne == ligne) // On a trouvé la bonne ligne
        { 
            if (iter->ordre < ordre) // Parcourir les ordres
            {   
                precedent = iter;
                iter = iter->suivant;
            }

            else if (iter->ordre == ordre)
            {
                printf("Existe déjà\n");
                free(nouveau);
                return NULL;
            }

            else  // Ok insérer ici
            {
                break;
            }
        }

        else
        {   
            // Ici on arrive si jamais iter->numeroLigne > ligne. Cela arrive quand:

            // Pour une raison quelconque, on souhaite insérer en tête
            // La ligne que l'on veut insérer n'existe pas et on va l'insérer entre 2 lignes existantes en respectant l'ordre

            break;
        }
    }

    // On est à la bonne ligne et au bon ordre

    if (precedent == NULL) // On rajoute en tête de liste
    { 
        nouveau->suivant = listeP;
        return nouveau;
    }

    // Sinon on est au milieu ou à la fin

    nouveau->suivant = iter;
    precedent->suivant = nouveau;
    return listeP;
}

int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase){

    // Conversion du mot en minuscules pour l'ignorance de la case

    int i;
    int tailleMot = strlen(mot);
    for (i = 0; i < tailleMot; i++) {
        mot[i] = tolower(mot[i]);
    }
    
    // Recherche de l'emplacement d'insertion dans l'ABR

    T_Noeud *courant = index->racine;
    T_Noeud *precedent = NULL;
    
    while (courant != NULL) {

        int comparaison = strcmp(mot, courant->mot);
        
        if (comparaison == 0) {

            // Le mot existe déjà dans l'index, ajout de la nouvelle position

            T_Position *positionCourante = ajouterPosition(courant->listePositions, ligne, ordre, phrase);

            if (positionCourante == NULL){ // Si NULL alors pb d'insertion
                return 0;
            }

            // Si jamais on a inséré en tête on refresh le pointeur
            courant->listePositions = positionCourante; 
            
            courant->nbOccurences++;
            index->nbMotsTotal++;
            
            return 1; // Succès de l'ajout

        } else if (comparaison > 0) {

            precedent = courant;
            courant = courant->filsDroit;

        }

        else {

            precedent = courant;
            courant = courant->filsGauche;

        }
    }
    

    // Le mot n'existe pas dans l'index, création d'un nouveau nœud

    T_Noeud *nouveauNoeud = creerNoeud(mot, ligne, ordre, phrase);

    
    if (precedent == NULL) { // Il n'y avait rien au début

        index->racine = nouveauNoeud;

    } else {

        int comparaisonPrecedent = strcmp(mot, precedent->mot);

        if (comparaisonPrecedent < 0) {

            precedent->filsGauche = nouveauNoeud;

        } else {

            precedent->filsDroit = nouveauNoeud;

        }

    }
    
    index->nbMotsDistincts++;
    index->nbMotsTotal++;
    
    return 1; // Succès
}

int indexerFichier(T_Index *index, char *filename){

    FILE *file;
    int nb_mots_lus = 0;
    char mot[MAX_WORD_LENGTH];
    memset(mot, '\0', sizeof(mot));

    int track[3] = {1,1,1};  // Track de l'ordre


    //Ouvrir le fichier

    file = fopen(filename, "r");
    if (file == NULL){
        printf("Erreur dans l'ouverture du fichier.\n");
        return -1;
    }


    // Lire chaque caractère du fichier

    char c;

    
    
    while ((c = fgetc(file)) != EOF) {
        
        if (c == '\n') { // Nouvelle ligne

            if (strlen(mot) > 0) // Si on a lu qq chose comme mot
            { 
            ajouterOccurence(index, mot, track[0], track[1], track[2]);
            nb_mots_lus++;
            
            // On reset le mot
            memset(mot, '\0', sizeof(mot));
            }

            track[1] = 1;
            track[0]++;
            
        }

        else if (isspace(c)) { // C'est un espace
            
            // On finit la lecture du mot et on l'insère

            if (strlen(mot) > 0) // Si on a lu qq chose comme mot
            { 
            ajouterOccurence(index, mot, track[0], track[1], track[2]);
            nb_mots_lus++;
            
            // On reset le mot
            memset(mot, '\0', sizeof(mot));
            track[1]++;
            }

            // On reset le mot
            memset(mot, '\0', sizeof(mot));

        }


        else if (c == '.') { // Nouvelle phrase

            if (strlen(mot) > 0) // Si on a lu qq chose comme mot
            { 
            ajouterOccurence(index, mot, track[0], track[1], track[2]);
            nb_mots_lus++;
            
            // On reset le mot
            memset(mot, '\0', sizeof(mot));
            track[1]++;
            }

            track[2]++;
            
        }


        else {

            // On rajoute le caractère au mot courant
            strncat(mot, &c, 1);

        }
    }

    // Dernier mot si on a oublié le point à la fin du texte

    if (strlen(mot) > 0) {
        ajouterOccurence(index, mot, track[0], track[1], track[2]);
        nb_mots_lus++;
    }

    fclose(file);
    return nb_mots_lus;
}

void afficherIndex(T_Index index){
    
    T_Pile *p = creer_pile(index.nbMotsTotal);
    T_Noeud *noeud = index.racine;
    T_Position *position;
    char mot_affiche[MAX_WORD_LENGTH];

    char c = noeud->mot[0];

    while (pile_vide(p) == 0 || (noeud != NULL))


        if (noeud != NULL){
            empiler(p, noeud);
            noeud = noeud->filsGauche;
        }

        else {

            if (pile_vide(p) == 0){

                noeud = depiler(p);

                // Afficher 

                if (noeud->mot[0] == c){
                    
                    strcpy(mot_affiche, noeud->mot);
                    mot_affiche[0] = mot_affiche[0] - ('a' - 'A');
                    printf("|-- %s\n", mot_affiche);

                    position = noeud->listePositions;
                    while (position != NULL){
                        printf("|---- (l:%d, o:%d, p:%d)\n", position->numeroLigne,position->ordre, position->numeroPhrase);
                        position = position->suivant;
                    }
                    printf("|\n");

                }

                else {

                    c = noeud->mot[0];
                    printf("\n");
                    printf("%c\n", c - ('a' - 'A'));

                    strcpy(mot_affiche, noeud->mot);
                    mot_affiche[0] = mot_affiche[0] - ('a' - 'A');
                    printf("|-- %s\n", mot_affiche);

                    position = noeud->listePositions;
                    while (position != NULL){
                        printf("|---- (l:%d, o:%d, p:%d)\n", position->numeroLigne,position->ordre, position->numeroPhrase);
                        position = position->suivant;
                    }
                    printf("|\n");

                }

                noeud = noeud->filsDroit;
            }

            else break; // On a fini
            
        }


    free(p);
}

T_Noeud* rechercherMot(T_Index index, char *mot){

    T_Noeud *iter = index.racine;

    // Car tous les mots sont en mi
    int i;
    int tailleMot = strlen(mot);
    for (i = 0; i < tailleMot; i++) {
        mot[i] = tolower(mot[i]);
    }

    while (iter!=NULL)
    {

        if (strcasecmp(mot, iter->mot) == 0)
        {
            return iter;
        }

        else if(strcasecmp(mot, iter->mot) < 0)
        {
            iter = iter->filsGauche;
        }

        else
        {
            iter = iter->filsDroit;
        }
    }

    return NULL;
}

void drawBinaryTree(T_Noeud* node, int level) {
    if (node == NULL) {
        return;
    }
    
    drawBinaryTree(node->filsDroit, level + 1);
    
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
    
    printf("%s (%d)\n", node->mot, node->nbOccurences);
    
    drawBinaryTree(node->filsGauche, level + 1);
}

void drawIndexTree(T_Index* index) {
    drawBinaryTree(index->racine, 0);
}