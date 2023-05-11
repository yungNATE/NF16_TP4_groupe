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

    if(ajoutFlag){
        index->nbMotsTotal++;
        if(nouveauMot) index->nbMotsDistincts++;
    }
}