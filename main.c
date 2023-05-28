#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "tp4.h"
#include "tp4.c"

int main(void){

    // On pourra supprimer drawbinarytree et drawindextree c'était pour des tests

    T_Index *index = creerIndex();

    char filename[] = "test.txt";
    indexerFichier(index, filename);
    // afficherIndex(*index);

    T_Position *premier = index->texte->premiere->premiermot;

    while (premier != NULL){
        printf("%s ", premier->noeud_parent->mot);
        premier = premier->mot_suivant;
    }

    return 0;
}