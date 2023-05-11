#include "tp4.h"

T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase){

    // Création position

    T_Position *nouveau = malloc(sizeof(T_Position));
    if (nouveau == NULL) return NULL;
    nouveau->numeroLigne = ligne;
    nouveau->ordre = ordre;
    nouveau->numeroPhrase = phrase;

    T_Position *iter = listeP;
    T_Position *precedent = NULL;


    // Chercher la bonne position
    while (iter != NULL){
        if (iter->numeroLigne < ligne){
            precedent = iter;
            iter = iter->suivant;
        }

        if (iter->numeroLigne = ligne){
            if (iter->ordre < ordre){
                precedent = iter;
                iter = iter->suivant;
            }

            else if (iter->ordre == ordre){
                // Déjà existe
            }

            else {
                // Insert and return
            }
        }
    }

    // Si on n'a pas retrouvé la bonne ligne c'est qu'elle existe pas
    // Ici donc iter = NULL et precedent est le dernier élément

    precedent->suivant = nouveau;
    return listeP;
}