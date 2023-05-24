#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tp4.h"

// Fonction permettant l'ajout d'un mot dans l'index en respectant les règles d'insertion dans les ABR. 
// Elle renvoie 1 en cas de succès, 0 sinon.
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase){
     // Conversion du mot en minuscules pour l'ignorance de la casse
    int i;
    int tailleMot = strlen(mot);
    for (i = 0; i < tailleMot; i++) {
        mot[i] = tolower(mot[i]);
    }
    
    // Création de la nouvelle position
    T_Position* nouvellePosition = (T_Position*)malloc(sizeof(T_Position));
    nouvellePosition->numeroLigne = ligne;
    nouvellePosition->ordre = ordre;
    nouvellePosition->numeroPhrase = phrase;
    nouvellePosition->suivant = NULL;
    
    // Recherche de l'emplacement d'insertion dans l'ABR
    T_Noeud *courant = index->racine;
    T_Noeud *precedent = NULL;
    
    while (courant != NULL) {
        int comparaison = strcmp(mot, courant->mot);
        
        if (comparaison == 0) {
            // Le mot existe déjà dans l'index, ajout de la nouvelle position
            T_Position *positionCourante = courant->listePositions;
            T_Position *positionPrecedente = NULL;
            
            // Parcours de la liste chaînée des positions pour trouver la position à insérer
            while (positionCourante != NULL) {
                positionPrecedente = positionCourante;
                positionCourante = positionCourante->suivant;
            }
            
            if (positionPrecedente != NULL) {
                positionPrecedente->suivant = nouvellePosition;
            } else {
                courant->listePositions = nouvellePosition;
            }
            
            courant->nbOccurences++;
            index->nbMotsTotal++;
            
            return 1; // Succès de l'ajout
        } else {
            precedent = courant;
            courant = courant->filsDroit;
        }
    }
    
    // Le mot n'existe pas dans l'index, création d'un nouveau nœud
    T_Noeud *nouveauNoeud = (T_Noeud*)malloc(sizeof(T_Noeud));
    nouveauNoeud->mot = strdup(mot);
    nouveauNoeud->nbOccurences = 1;
    nouveauNoeud->listePositions = nouvellePosition;
    nouveauNoeud->filsGauche = NULL;
    nouveauNoeud->filsDroit = NULL;
    
    if (precedent == NULL) {
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
