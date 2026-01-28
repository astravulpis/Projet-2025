# ULTRAC00L
## Présentation
Projet de deuxième année de licence informatique, a l’Université du Mans.  
Codé en langage C, Jeu 2D, inspiré de FPS 3D, notamment [ULTRAKILL](https://store.steampowered.com/app/1229490/ULTRAKILL/).  

### Outils utilisés :
- Utilisation de [SDL3](https://github.com/libsdl-org/SDL) pour l'interface graphique
- Utilisation de [nob.h](github.com/tsoding/nob.h) pour la compilation des fichier .c
- Utilisation de [Doxygen](https://github.com/doxygen/doxygen) pour la documentation du code

## Compilation du code

Compilation de l'outil nob: `cc -Wall -Wextra -o nob nob.c`  
Compilation du code       : `./nob`

## Execution du code
```bash
./nob -run
```

### Qu'est ce que nob?

L'outil `nob` est un programme C s'aidant de la librairie [nob.h](github.com/tsoding/nob.h) afin de proposer une compilation avec uniquement C.
Les différents flags implémentés:  
- `./nob -run` Pour faire fonctionner l'application
- `./nob -clean` Pour supprimer les objets compiles  
- `./nob -debug` Pour compiler le code avec les symboles de debug et faire tourner `gdb`  
- `./nob -debugui` Pour utiliser l'interface front-end de `gdb`: [gf2](https://github.com/nakst/gf)  
- `./nob -help` Pour afficher les différents flags possible.  

Note: Tout flags peuvent etre utilise independamment des autres.  
Exemple: `./nob -clean -run` pour reconstruire le programme en supprimant la cache.  
