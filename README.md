# Projet Compilateur SoS

## Objectif du projet

Ce projet a pour but de créer un compilateur pour le langage SoS. SoS est un langage impératif simple qui utilise une syntaxe et des fonctionnalités provenant du sous-ensemble de langage shell unix (Sous-Shell). Ce compilateur représente un enjeu important du premier semestre des masters SDSC/SIRIS, dont nous sommes issus, délivrés par l'Université de Strasbourg.


## Projet réalisé par :

- Arnaud FILIPPI : Gestion des structures de contrôle, des boucles
- Charles JARDOT : Gestion des Tableaux, Création des tests 
- Nathanaël DEROUSSEAUX : Organisation de la structure, du git
- Thomas POIRRIER : Gestion des opérations

## Commandes

### Quick start

```bash
# Compiler les sources
$ make

# Compiler un programme
$ ./sos [-o asm-file] [sos-file]

# Exécuter un programme
$ spim -file asm-file
```

### Options supplémentaires

```bash
# Voir les membres du projet
$ ./sos -v

# Afficher la table des symboles
$ ./sos -tos
```

### Tests

```bash
# Lancer les tests
test.sh

# Lancer des tests spécifiques
test.sh [test-name] [test-name] ...
```

## Liens externes

La procédure pour ajouter des tests est détaillée [ici](https://git.unistra.fr/projet-compilation/projet/-/wikis/Procédure-de-test)

Vous pouvez trouver notre gitlab [ici](https://git.unistra.fr/projet-compilation/compilateur-sos)
