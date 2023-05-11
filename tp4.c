#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tp4.h"

// Fonction permettant l'ajout d'un mot dans l'index en respectant les règles d'insertion dans les ABR. 
// Elle renvoie 1 en cas de succès, 0 sinon.
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase){
    bool ajoutFlag = false;
    bool nouveauMot = false;

    T_Noeud *noeud = index->racine;
    parcourirArbre(noeud, mot, &ajoutFlag, &nouveauMot, ligne, ordre, phrase);

    if(ajoutFlag){
        index->nbMotsTotal++;
        if(nouveauMot) index->nbMotsDistincts++;
    }
}

// Fonction  permettant de parcourir l'arbre et d'ajouter un mot si besoin.
parcourirArbre(T_Noeud *noeud, char *mot, bool *ajoutFlag, bool *nouveauMot, int ligne, int ordre, int phrase){
    if(noeud == NULL){
        noeud = creerNoeud(mot);
        noeud->nbOccurences++;
        noeud->nbMotsTotal++;
        noeud->nbMotsDistincts++;
        noeud->ligne = ligne;
        noeud->ordre = ordre;
        noeud->phrase = phrase;
        *ajoutFlag = true;
        *nouveauMot = true;
    }
    else if(strcmp(mot, noeud->mot) < 0){
        noeud->nbMotsTotal++;
        noeud->nbMotsDistincts++;
        parcourirArbre(noeud->gauche, mot, ajoutFlag, nouveauMot, ligne, ordre, phrase);
    }
    else if(strcmp(mot, noeud->mot) > 0){
        noeud->nbMotsTotal++;
        noeud->nbMotsDistincts++;
        parcourirArbre(noeud->droite, mot, ajoutFlag, nouveauMot, ligne, ordre, phrase);
    }
    else if(strcmp(mot, noeud->mot) == 0){
        noeud->nbOccurences++;
        noeud->ligne = ligne;
        noeud->ordre = ordre;
        noeud->phrase = phrase;
        *ajoutFlag = true;
        *nouveauMot = false;
    }
}
