#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tp4.h"

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

T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase){

    // Création position

    T_Position *nouveau = creerPosition(ligne, ordre, phrase);
    T_Position *iter = listeP;
    T_Position *precedent = NULL;

    // Pour tenir compte du bon ordre des phrases

    int track_phrase = 0;

    // Chercher la bonne position

    while (iter != NULL){
        if (iter->numeroLigne < ligne) // Parcourir les lignes
        {   
            if (iter->numeroPhrase > track_phrase) {track_phrase = iter->numeroPhrase;}
            precedent = iter;
            iter = iter->suivant;
        }

        else if (iter->numeroLigne == ligne) // On a trouvé la bonne ligne
        { 
            if (iter->ordre < ordre) // Parcourir les ordres
            {   
                if (iter->numeroPhrase > track_phrase) {track_phrase = iter->numeroPhrase;}
                precedent = iter;
                iter = iter->suivant;
            }

            else if (iter->ordre == ordre)
            {
                printf("Existe déjà\n");
                return listeP;
            }

            else if (iter->ordre > ordre) 
            {   
                // Si jamais on veut introduire dans une même ligne, un mot avant des mots déjà existants
                // Exemple: 
                // Sur la première ligne il y a seulement ce noeud:
                // Ligne: 1, Ordre: 4, Phrase: 10 
                // Et on souhaite rajouter, pour une raison ou autre, le noeud:
                // Ligne: 1, Ordre: 3, Phrase: 10
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

    if (track_phrase > phrase || (iter !=NULL && iter->numeroPhrase < phrase))
    {   
        printf("ERREUR: ordre incohérent des phrases.\n");
        printf("Bon ordre de ligne et de mot mais l'ordre de phrase que vous insérez est plus petit que celui à l'endroit où vous insérez.\n");
        printf("L'erreur concerne l'insertion ");
        afficherPosition(nouveau);
        return listeP;
    }

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
