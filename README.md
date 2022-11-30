# SoS

## Quick start
```bash
#Compiler les sources
$ make

#Compiler un programme
$ ./sos [-o asm-file] [sos-file]

#Éxécuter un programme
$ spim -file asm-file
```

## Tests
```bash
#Lancer les tests
test.sh

#Lancer des tests spécifiques
test.sh [test-name] [test-name] ...
```
La procédure pour ajouter des tests est détaillée [ici](https://git.unistra.fr/projet-compilation/projet/-/wikis/Procédure-de-test)