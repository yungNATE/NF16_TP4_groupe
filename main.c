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

    char filename[] = "test.txt"; // J'ai laissé exprès une ligne libre pour voir le comportement, ça a l'air ok 
    indexerFichier(index, filename);

    afficherIndex(*index);

    return 0;
}
