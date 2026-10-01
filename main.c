#include <stdio.h>
#include "liste.h"

int main(void)
{
    Maillon *liste = NULL;
    for (int i = 1; i <= 5; i++) liste = liste_inserer(liste, i * 10);

    printf("liste     : ");
    liste_afficher(liste);
    printf("longueur  : %d\n", liste_longueur(liste));
    printf("contient 30 : %s\n", liste_contient(liste, 30) ? "oui" : "non");

    Maillon *autre = NULL;
    for (int i = 1; i <= 3; i++) autre = liste_inserer(autre, i);
    printf("autre     : ");
    liste_afficher(autre);

    printf("blocs apres construction : %d\n", liste_blocs_en_circulation());

    liste_liberer(liste);
    liste_liberer(autre);
    printf("liberee\n");
    printf("blocs apres liberation   : %d\n", liste_blocs_en_circulation());
    return 0;
}
