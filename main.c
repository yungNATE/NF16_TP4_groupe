#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include "tp4.h"
#include "tp4.c"

int main(void){

    T_Position *test = creerPosition(2,1,1);

    test = ajouterPosition(test, 1,4,1);
    test = ajouterPosition(test, 2,4,2);
    test = ajouterPosition(test, 10,4,5);
    test = ajouterPosition(test, 3,4,2);
    // Tests pour l'erreur d'ordre de phrase:
    // test = ajouterPosition(test, 1,2,2);
    // test = ajouterPosition(test, 1,3,2);

    afficherPosition(test);
    return 0;
}