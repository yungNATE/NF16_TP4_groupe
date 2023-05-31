#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "tp4.h"

T_Phrase *creer_phrase(int numero){

    T_Phrase *new = (T_Phrase*) malloc(sizeof(T_Phrase));
    if (new == NULL) return NULL;

    new->numero = numero;
    new->premiermot = NULL;
    new->derniermot = NULL;
    new->suivant = NULL;
    new->precedent = NULL;

    return new;
}

T_Texte *creer_texte(){
    T_Texte *new = (T_Texte*) malloc(sizeof(T_Texte));
    if (new == NULL) return NULL;

    new->premiere = NULL;
    new->derniere = NULL;

    return new;
}

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
        listeP = listeP->position_suivante;
    }
}

T_Position *creerPosition(int ligne, int ordre, int phrase){

    T_Position *nouveau = malloc(sizeof(T_Position));
    if (nouveau == NULL) return NULL;

    nouveau->numeroLigne = ligne;
    nouveau->ordre = ordre;
    nouveau->numeroPhrase = phrase;
    nouveau->position_suivante = NULL;
    nouveau->mot_suivant = NULL;
    nouveau->noeud_parent = NULL;
    
    return nouveau;
}

T_Noeud *creerNoeud(char *mot, int ligne, int ordre, int phrase){

    T_Noeud *nouveauNoeud = (T_Noeud*) malloc(sizeof(T_Noeud));
    if (nouveauNoeud == NULL) return NULL;

    T_Position *nouvellePosition = creerPosition(ligne, ordre, phrase);
    
    nouveauNoeud->mot = strdup(mot);
    nouveauNoeud->nbOccurences = 1;
    nouveauNoeud->filsGauche = NULL;
    nouveauNoeud->filsDroit = NULL;


    // La création d'une position dummy sert juste à avoir au moins une fois le noeud parent dans les positions
    // Quand nous allons rajouter la première position et les suivantes
    // Elles vont copier le noeud parent de la position précédente
    // Comme au début on n'a pas de positions il n'y a pas l'info du noeud parent
    // Pour résoudre le problème on créé une position dummy avec ordre = -1 pour dire qu'il y a rien
    // Mais qui aura quand même en mémoire le noeud parent
    // Le noeud parent servira à accéder au mot
    // Cela évite de ré stocker chaque plusieurs fois le mot qui sera assez coûtant niveau mémoire

    nouveauNoeud->listePositions = nouvellePosition;
    nouveauNoeud->listePositions->ordre = -1; // Pour dire qu'il n'y a pas de positions pour l'instant
    nouveauNoeud->listePositions->noeud_parent = nouveauNoeud; // Pour indiquer ce noeud parent 
    nouveauNoeud->derniere_position = nouveauNoeud->listePositions; // On en a qu'une seule donc elle est première et dernière

    ajouterPosition(nouveauNoeud->listePositions, ligne, ordre, phrase);

    return nouveauNoeud;

}

T_Index *creerIndex(){

    T_Index *nouveau = (T_Index*) malloc(sizeof(T_Index));
    if (nouveau == NULL) return NULL;

    nouveau->nbMotsDistincts = 0;
    nouveau->nbMotsTotal = 0;
    nouveau->racine = NULL;
    nouveau->texte = creer_texte();

    return nouveau;
}

T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase){

    // La gestion du mauvais ordre d'insertion est gérée dans ajouterOccurence
    // La gestion du mot suivant dans le texte se fait dans ajouterOccurence

    // On rajoute en tête de liste si ordre = -1 cela veut dire qu'on n'a pas de positions
    // En fait le -1 est mis à la création du noeud juste pour rajouter dans la position le noeud parent
    // C'est un peu bizarre mais je me suis débrouillé pour laisser les prototypes des fonctions comme celles demandées
    // Ca aurait été beaucoup plus facile juste de donner en argument l'adresse du noeud parent et éviter les conventions bizarres comme -1

    // Insertion de la première position
    // Pas besoin de créér une nouvelleposition, on modifiera la position dummy
    // Pas besoin de modifier la dernière position

    T_Position *iter = listeP;

    if (iter->ordre == -1){
        iter->noeud_parent = listeP->noeud_parent;
        iter->position_suivante = NULL;
        iter->numeroLigne = ligne;
        iter->ordre = ordre;
        iter->numeroPhrase = phrase;
        return iter;
    }

    // Création position
    T_Position *nouveau = creerPosition(ligne, ordre, phrase);
    T_Position *precedent = NULL;

    // Chercher la bonne position

    while (iter != NULL){

        if (iter->numeroLigne < ligne) // Parcourir les lignes
        {   
            precedent = iter;
            iter = iter->position_suivante;
        }

        else if (iter->numeroLigne == ligne) // On a trouvé la bonne ligne
        { 
            if (iter->ordre < ordre) // Parcourir les ordres
            {   
                precedent = iter;
                iter = iter->position_suivante;
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
            // Ici on arrive si jamais iter->numeroLigne > ligne. 
            // Cela n'arrive jamais car on fait une insertion par ordre croissant.

            break;
        }
    }

    // On est à la bonne ligne et au bon ordre (à la fin normalement)

    nouveau->position_suivante = iter;
    precedent->position_suivante = nouveau;

    nouveau->noeud_parent = listeP->noeud_parent;
    nouveau->noeud_parent->derniere_position = nouveau; // Comme l'insertion se fait à la fin, cette nouvelle insertion devient la dernière position du noeud (mot)

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

            courant->nbOccurences++;
            index->nbMotsTotal++;

            // On fait le lien entre le dernier mot entré et celui là
            // Pour cela on cherche le dernier mot inséré de la dernière phrase insérée

            if (index->texte->derniere->premiermot == NULL) // Si la phrase est vide
            {   
                index->texte->derniere->precedent->derniermot->mot_suivant = courant->derniere_position;
                index->texte->derniere->premiermot = courant->derniere_position;
                index->texte->derniere->derniermot = courant->derniere_position;
            }

            else {

                index->texte->derniere->derniermot->mot_suivant = courant->derniere_position;
                index->texte->derniere->derniermot = courant->derniere_position;
                
            }

            // La mise à jour de la dernière position se fait dans ajouterPosition
            
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

    // On fait le lien entre le dernier mot entré et celui là
    // Pour cela on cherche le dernier mot inséré de la dernière phrase insérée

    if (index->texte->premiere->premiermot == NULL){ // Si on vient de commencer et il n'y a rien dans la première phrase
        index->texte->premiere->premiermot = nouveauNoeud->derniere_position;
        index->texte->premiere->derniermot = nouveauNoeud->derniere_position;
    }

    else if (index->texte->derniere->premiermot == NULL) // Si la phrase est vide
    {   
        index->texte->derniere->precedent->derniermot->mot_suivant = nouveauNoeud->derniere_position;
        index->texte->derniere->premiermot = nouveauNoeud->derniere_position;
        index->texte->derniere->derniermot = nouveauNoeud->derniere_position;
    }

    else {

        index->texte->derniere->derniermot->mot_suivant = nouveauNoeud->derniere_position;
        index->texte->derniere->derniermot = nouveauNoeud->derniere_position;
        
    }

    
    return 1; // Succès
}

int indexerFichier(T_Index *index, char *filename){

    //* La gestion des phrases se fait ici
    int flag_nouvelle_phrase = 1; // Servira à signaler la création d'une nouvelle phrase

    FILE *file;
    int nb_mots_lus = 0;
    char mot[MAX_WORD_LENGTH];
    memset(mot, '\0', sizeof(mot));

    int track[3] = {1,1,1};  // Track de l'ordre


    //* Ouvrir le fichier
    file = fopen(filename, "r");
    if (file == NULL){
        printf("Erreur dans l'ouverture du fichier.\n");
        return -1;
    }


    //* Lire chaque caractère du fichier
    T_Phrase *nouvellephrase = creer_phrase(1); // On initialise avec la première phrase
    index->texte->premiere = nouvellephrase;
    index->texte->derniere = nouvellephrase;
    
    char c;

    while ((c = fgetc(file)) != EOF) {

        if (c == '\n') { // Nouvelle ligne

            if (strlen(mot) > 0) // Si on a lu un mot
            { 
                // Si ce mot fait partie d'une nouvelle phrase
                if (track[2] > flag_nouvelle_phrase) { // On en crée une et met à jour le flag
                    nouvellephrase = creer_phrase(track[2]);
                    nouvellephrase->precedent = index->texte->derniere;
                    index->texte->derniere->suivant = nouvellephrase;
                    index->texte->derniere = nouvellephrase;
                    
                    flag_nouvelle_phrase = track[2];
                }

                ajouterOccurence(index, mot, track[0], track[1], track[2]);
                nb_mots_lus++;
                
                // On reset le mot
                memset(mot, '\0', sizeof(mot));
            }

            track[1] = 1;
            track[0]++;
            
        }

        // TODO : Peut-être remplacer isspace() par isblank() ? Comme isspace() prend en compte aussi les '\n' ? 
        // En l'état ça marche parcequ'on check '\n' avant mais faudrait pas changer l'ordre des conditions
        else if (isspace(c)) { 
            
            // On finit la lecture du mot et on l'insère

            if (strlen(mot) > 0) // Si on a lu un mot
            {
                // Si ce mot fait partie d'une nouvelle phrase
                if (track[2] > flag_nouvelle_phrase) { // On en crée une et met à jour le flag
                    nouvellephrase = creer_phrase(track[2]);
                    nouvellephrase->precedent = index->texte->derniere;
                    index->texte->derniere->suivant = nouvellephrase;
                    index->texte->derniere = nouvellephrase;
                    
                    flag_nouvelle_phrase = track[2];
                }

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
                //Si ce mot fait partie d'une nouvelle phrase
                if (track[2] > flag_nouvelle_phrase) { // On en crée une et met à jour le flag
                    nouvellephrase = creer_phrase(track[2]);
                    nouvellephrase->precedent = index->texte->derniere;
                    index->texte->derniere->suivant = nouvellephrase;
                    index->texte->derniere = nouvellephrase;
                    
                    flag_nouvelle_phrase = track[2];
                }

                ajouterOccurence(index, mot, track[0], track[1], track[2]);
                nb_mots_lus++;
                
                // On reset le mot
                memset(mot, '\0', sizeof(mot));
                track[1]++;

            }

            track[2]++;
            
        }


        else { // On rajoute le caractère au mot courant
            strncat(mot, &c, 1);
        }
    }

    // Si on a oublié le point à la toute fin du texte, on ajoute le dernier mot qui est resté en suspens
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
                        position = position->position_suivante;
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
                        position = position->position_suivante;
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

void afficherOccurencesMot(T_Index index, char *mot){

    char affiche[MAX_WORD_LENGTH];

    T_Noeud *cible = rechercherMot(index, mot);

    if (cible == NULL) {
        printf("Mot non trouvé.");
        return;
    }
    
    strcpy(affiche, cible->mot);
    premiereLettreEnMajuscule(affiche);

    printf("Mot = \"%s\"\n", affiche);
    printf("Occurences = %d\n", cible->nbOccurences);

    // Nous allons itérer les phrases une seule fois au total
    // Car on sait que naturellement les positions sont ordonnées par ordre de nr de phrase croissant
    // On n'aura donc pas besoin d'accèder à une phrase antérieure à un instant donné

    T_Position *iter_pos = cible->listePositions;
    T_Phrase *iter_phrase = index.texte->premiere;
    T_Position *iter_mot_affiche;

    while (iter_pos != NULL){

        printf("| Ligne %d, mot %d : ", iter_pos->numeroLigne, iter_pos->ordre);

        while (iter_phrase->numero != iter_pos->numeroPhrase) {
            iter_phrase = iter_phrase->suivant;
        }

        // On est à la bonne phrase et on l'affiche

        iter_mot_affiche = iter_phrase->premiermot; // Le premier mot

        strcpy(affiche, iter_mot_affiche->noeud_parent->mot);
        premiereLettreEnMajuscule(affiche);
        printf("%s ", affiche);

        iter_mot_affiche = iter_mot_affiche->mot_suivant;

        while (iter_mot_affiche != NULL && (iter_mot_affiche->numeroPhrase != (iter_pos->numeroPhrase + 1))) {
            printWord(iter_mot_affiche);
            iter_mot_affiche = iter_mot_affiche->mot_suivant;
        }

        printf("\n");

        iter_pos = iter_pos->position_suivante;

    }

}

void premiereLettreEnMajuscule(char *mot){

    if (mot[0] >= 'a' && mot[0] <= 'z') {
        mot[0] = mot[0] - ('a' - 'A');
    }

}

void printWord(T_Position *pos){

    printf("%s", pos->noeud_parent->mot);

    // Si c'est le dernier mot de la phrase afficher . et pas d'espace
    if (pos->mot_suivant == NULL || (pos->numeroPhrase != (pos->mot_suivant->numeroPhrase))) {
        printf(".");
    }

    // Espace sinon
    else {
        printf(" ");
    }

}

void fprintWord(T_Position *pos, FILE *f, int maj){

    char affiche[MAX_WORD_LENGTH];
    strcpy(affiche, pos->noeud_parent->mot);

    if (maj == 1) {
        premiereLettreEnMajuscule(affiche);
    }
    
    fprintf(f, "%s", affiche);

    // Si c'est le dernier mot de la phrase afficher . et et un espace
    if (pos->mot_suivant != NULL && (pos->numeroPhrase != (pos->mot_suivant->numeroPhrase))) {
        fprintf(f, ". ");
    }

    // Si c'est le dernier mot du fichier afficher . et pas d'espace
    else if (pos->mot_suivant == NULL) {
        fprintf(f, ".");
    }

    // Espace sinon
    else {
        fprintf(f, " ");
    }

}

void construireTexte(T_Index index, char *filename){

    FILE *file = fopen(filename, "w");
    int flag_maj = 1; // On initialise avec 1 car le premier mot sera avec majuscule

    T_Position *iter_mot = index.texte->premiere->premiermot;

    while (iter_mot != NULL){
        
        if (flag_maj == 1) {
            fprintWord(iter_mot, file, 1);
            flag_maj = 0;
        } else {
            fprintWord(iter_mot, file, 0);
        }

        if ((iter_mot->mot_suivant != NULL) && (iter_mot->numeroLigne != iter_mot->mot_suivant->numeroLigne)) {
            for (int k = 0; k < iter_mot->mot_suivant->numeroLigne - iter_mot->numeroLigne; k++){ // Si jamais on laisse 2 lignes ou plus d'espace
                fprintf(file, "\n");
            }
        }

        if (iter_mot->mot_suivant != NULL && iter_mot->mot_suivant->numeroPhrase > iter_mot->numeroPhrase) { // Le prochain sera avec majuscule
            flag_maj = 1;
        }

        iter_mot = iter_mot->mot_suivant;
    }

    fclose(file);
}

void viderBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

void replaceNewLine_WithNullTerminator(char *str){
    str[strcspn(str, "\n")] = '\0';
}

char *getStringInput(char *prompt){

    printf("%s", prompt);

    char *input = malloc(sizeof(char) * 100);
    fgets(input, 100, stdin);
    replaceNewLine_WithNullTerminator(input);
    
    return input;
}

void deallocateIndex(T_Index *index){

    // Free l'arbre et positions
    deallocateArbre(index->racine);

    // Free les phrases
    T_Phrase *iter_phrase = index->texte->premiere;
    T_Phrase *temp;

    while (iter_phrase != NULL) {
        temp = iter_phrase->suivant;
        free(iter_phrase);
        iter_phrase = temp;
    }

    free(index->texte);

    free(index);

}

void deallocateArbre(T_Noeud *noeud){

    T_Position *temp;

    if (noeud == NULL) {
        return;
    }

    deallocateArbre(noeud->filsGauche);
    deallocateArbre(noeud->filsDroit);

    // Free les positions du noeud
    T_Position *iter_pos = noeud->listePositions;

    while (iter_pos != NULL) {   
        temp = iter_pos->position_suivante;
        free(iter_pos);
        iter_pos = temp;
    }

    // Puis free le noeud
    free(noeud);

}