#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "tp4.h"
#include "tp4.c"

int main(void)
{

     // ============= MENU UTILISATEUR ============= */
    char choix = 'x';
    while (choix != '7') 
    {
        printf("\n======================================");
        printf("\n1. Charger un fichier.");
        printf("\n2. Caractéristiques de l'index.");
        printf("\n3. Afficher index.");
        printf("\n4. Rechercher un mot.");
        printf("\n5. Afficher les occurences d'un mot.");
        printf("\n6. Construire le texte à partir de l'index.");
        printf("\n7. Quitter");
        printf("\n======================================");
        printf("\n   Votre choix ? ");

        choix = getchar();
        viderBuffer();

        printf("\nChoix %c sélectionné \n", choix);


        switch (choix)
        {
            case '1' :
            
                T_Index *index = creerIndex();

                char *nomFichier = getStringInput("\nNom du fichier ? ");

                indexerFichier(index, nomFichier);

                printf("\nNombre de mots lus : %d", index->nbMotsTotal);

                free(nomFichier);

                break;
                
            case '2' :
            {
                printf("\nNombre de mots dinstincts : %d", index->nbMotsDistincts);
                printf("\nNombre de mots total : %d", index->nbMotsTotal);
                break;
            }

            case '3' :
            {
                afficherIndex(*index);

                break;
            }

            case '4' :
            {
                char *nom_mot_recherche = getStringInput("\nNom du mot ? ");

                T_Noeud *mot_recherche = rechercherMot(*index, nom_mot_recherche);

                if (mot_recherche != NULL)
                {

                    printf("\nOccurences :");
                
                    T_Position *iter_position = mot_recherche->listePositions;

                    while (iter_position != NULL)
                    {
                        printf("\nLigne : %d, Ordre dans la ligne : %d, Nr. phrase: %d.", iter_position->numeroLigne, iter_position->ordre, iter_position->numeroPhrase);
                        iter_position = iter_position->position_suivante;
                    }

                }

                else
                {
                    printf("\nMot non trouvé.");
                }

                
                free(nom_mot_recherche);

                break;       
            }

            case '5' :
            {
                char *nom_mot_recherche_occurences = getStringInput("\nNom du mot ? ");

                printf("\n");

                afficherOccurencesMot(*index, nom_mot_recherche_occurences);

                free(nom_mot_recherche_occurences);

                break;
            }

            case '6' :
            {
                char *nom_nouveau_fichier = getStringInput("\nNom du fichier ? ");

                construireTexte(*index, nom_nouveau_fichier);

                free(nom_nouveau_fichier);

                break;
            }

            case '7' :

                deallocateIndex(index);
                free(index);

                printf("\n======== PROGRAMME TERMINE ========\n");

                break;

            default :

                printf("\n\nERREUR : votre choix n'est pas valide ! ");

        }

        printf("\n\n\n");

    }

    return 0;
}