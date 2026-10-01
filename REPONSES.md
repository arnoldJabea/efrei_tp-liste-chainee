# TP - Sortir du fichier unique : réponses

Environnement : WSL (Ubuntu), gcc 15.2.0, GNU make, git.

---

## Exercice 0 - Le dossier de projet et le premier commit

**Pourquoi ne versionne-t-on jamais les fichiers `.o` ni l'exécutable ?**
Ce sont des fichiers **générés** : on peut toujours les reconstruire à partir des sources avec `make`. Ils dépendent en plus de la machine (système, compilateur, options), sont binaires (donc illisibles dans un `diff`) et changent à chaque compilation. Les versionner gonflerait le dépôt et provoquerait des conflits inutiles. On versionne la **recette** (sources + Makefile), pas le plat.

---

## Exercice 1 - Découper la liste chaînée en trois fichiers

| Question | Réponse |
|---|---|
| A | `main.c` déclare des variables `Maillon *` et le compilateur doit connaître le type pour les compiler. Or chaque `.c` est compilé **séparément** : `main.c` ne voit jamais le contenu de `liste.c`, il ne voit que ce qu'il inclut. Le type partagé doit donc être dans le `.h`, inclus par les deux. |
| B | Pour que le compilateur **vérifie la cohérence** entre les déclarations (`.h`) et les définitions (`.c`) : si une signature diffère, l'erreur apparaît à la compilation de `liste.c`. Et parce que `liste.c` a lui-même besoin du type `Maillon` défini dans le `.h`. |

---

## Exercice 2 - Compiler en deux temps, à la main

```bash
gcc -Wall -Wextra -c main.c
gcc -Wall -Wextra -c liste.c
gcc -o demo main.o liste.o
./demo
```

Sortie du programme :

```
liste     : 50 -> 40 -> 30 -> 20 -> 10 -> NULL
longueur  : 5
contient 30 : oui
liberee
```

| Question | Réponse |
|---|---|
| A | **Deux** : `main.o` et `liste.o`, un par fichier `.c`. Il n'y a **pas** de `liste.h.o` : un en-tête n'est jamais compilé seul, le préprocesseur **recopie son texte** dans chaque `.c` qui l'inclut (`#include` = copier-coller). |
| B | Elle **recompile tout à chaque fois**, même les fichiers qui n'ont pas changé, et ne garde aucun `.o`. Sur 3 fichiers c'est négligeable ; sur un projet de centaines de fichiers, la compilation séparée ne recompile que ce qui a été modifié. |
