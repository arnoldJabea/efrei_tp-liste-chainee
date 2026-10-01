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

---

## Exercice 3 - Provoquer les trois erreurs classiques

| Cas | Premier message exact | Compilation ou lien |
|---|---|---|
| 1 | `main.o: in function 'main': main.c:(.text+0x35): undefined reference to 'liste_inserer'` puis `collect2: error: ld returned 1 exit status` | **Lien** |
| 2 | `main.c:5:5: error: unknown type name 'Maillon'` | Compilation |
| 3 | `liste.h:4:16: error: redefinition of 'struct Maillon'` (avec `-std=c11`, voir la remarque) | Compilation |

| Question | Réponse |
|---|---|
| 1 | Le **cas 1**. On le voit à `ld` dans le message (`/usr/bin/x86_64-linux-gnu-ld.bfd`, `ld returned 1 exit status`) et à la forme `main.c:(.text+0x35)` : un **décalage dans le code machine** du fichier objet au lieu d'un numéro de ligne. `main.c` s'est compilé sans problème ; c'est l'assemblage qui ne trouve pas le code des fonctions, puisque `liste.o` manque. |
| 2 | Le **premier** : `unknown type name 'Maillon'`. Les suivants (`implicit declaration of function 'liste_inserer'`..., `assignment to 'int *' from 'int'`) en découlent tous : sans `liste.h`, le compilateur ne connaît ni le type ni les fonctions. Une seule correction, remettre l'`#include`, les fait toutes disparaître. |
| 3 | Dès qu'un **en-tête est inclus deux fois** dans un même `.c`, ce qui arrive presque toujours **indirectement** : par exemple `main.c` inclut `liste.h` et `test.h`, et `test.h` inclut lui-même `liste.h` (parce qu'il utilise `Maillon`). Le programmeur n'a écrit l'inclusion qu'une fois par fichier, mais le préprocesseur recopie `liste.h` deux fois. |

> **Remarque sur le cas 3.** Avec gcc 15 sans option, **le cas 3 compile sans erreur** : la norme par défaut est C23 (`__STDC_VERSION__ = 202311L`), qui autorise à redéfinir une structure **à l'identique**. L'erreur attendue n'apparaît qu'avec `-std=c11` (l'option du Makefile du TP). La garde d'inclusion reste indispensable : dès que le `.h` contient une définition de variable ou de fonction (`int compteur = 0;`), même C23 refuse la double inclusion.

---

## Exercice 4 - Écrire un premier Makefile

Trous remplis : `main.o: main.c liste.h` et `liste.o: liste.c liste.h`.
Vérification des tabulations avec `cat -A Makefile` : les lignes de commande commencent bien par `^I`.

`make clean` puis `make` :

```
rm -f main.o liste.o demo
gcc -Wall -Wextra -std=c11 -g -c main.c
gcc -Wall -Wextra -std=c11 -g -c liste.c
gcc -Wall -Wextra -std=c11 -g -o demo main.o liste.o
```

Second `make`, sans rien modifier :

```
make: 'demo' is up to date.
```

| Question | Réponse |
|---|---|
| A | `make` compare les **dates de modification** : une cible n'est reconstruite que si elle n'existe pas ou si l'une de ses dépendances est **plus récente** qu'elle. Ici `main.o` et `liste.o` sont plus récents que leurs `.c` et `.h`, et `demo` plus récent que les `.o` : il n'y a rien à faire. |
| B (message exact) | `Makefile:2: *** missing separator.  Stop.` (la ligne 2 est la commande où la tabulation a été remplacée par quatre espaces) |
