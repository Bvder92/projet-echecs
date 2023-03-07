# Projet Algorithmique et Programmation L2 MIASHS

## Implémentation de l'algorithme Minimax pour les échecs en C.

### Description

Notre IA est capable de jouer aux échecs à un niveau d'environ 1400 elo (testée sur Chess.com), elle est capable de prévoir 4 à 5 coups à l'avance
et ainsi d'anticiper la partie et de choisir le 'meilleur' coup pour maximiser ses chances. Néanmoins, elle est très faible à l'ouverture et en phase
de fin de partie car elle ne dispose pour l'instant d'aucune connaissances sur la façon de jouer durant ces phases cruciales.

### Compilation et Éxécution

Notre programme est fait pour foncitonner sous Linux et pour être compilé avec GCC. Pour compiler et éxécuter le programme, 
il suffit d'utiliser la **Makefile**:

    make && bin/main

Nous avons également inclus une **Dockerfile** permettant de générer un environnement Linux (Debian) incluant toutes les bonnes versions des 
programmes nécéssaires (git, gcc, make). Plus de détails sont fournis dans l'annexe de notre rapport, partie
 **B.1 - Mise en place de l'environnement et compilation**.

Par défaut, git ne traque pas les dossiers 'obj' et 'bin', il peut alors être nécéssaire de les créer manuellement.
