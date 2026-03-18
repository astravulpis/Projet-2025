# ULTRAC00L
## Auteurs du projet
 - [Liam BERGE](https://github.com/LiamB72)
 - [François ROSSIGNOL](https://github.com/Lampadaire-py) 
 - [Guillaume REEVES](https://github.com/Greeves0) <br/>
<a href = "https://github.com/LiamB72/Projet-2025/graphs/contributors">
    <img src = "https://contrib.rocks/image?repo=LiamB72/Projet-2025"/>
</a>

## Présentation
Projet de deuxième année de licence informatique, a l’Université du Mans.  
Codé en langage C, Jeu 2D, inspiré de FPS 3D, notamment [ULTRAKILL](https://store.steampowered.com/app/1229490/ULTRAKILL/).  

### Outils utilisés :
- Utilisation de [SDL3](https://github.com/libsdl-org/SDL) pour l'interface graphique.
- Utilisation de [nob.h](https://github.com/tsoding/nob.h) pour la compilation des fichier `.c`.
- Utilisation de [Doxygen](https://github.com/doxygen/doxygen) pour la documentation du code.

## Construire le projet

```bash
cc -x c -o nob nob.c
./nob --run
```

### Qu'est-ce que nob?

L'outil `nob` est un programme C utilisant la librairie [nob.h](https://github.com/tsoding/nob.h) afin de proposer une compilation qui se fait avec le langage C.
Les différents flags implémentés:  
- `./nob --run`     Pour faire fonctionner l'application.
- `./nob --clean`   Pour supprimer les objets compilés.
- `./nob --debug`   Pour compiler le code avec les symboles de debug et faire tourner `gdb`.
- `./nob --debugui` Pour utiliser l'interface front-end de `gdb`: [gf2](https://github.com/nakst/gf). 
- `./nob --help`    Pour afficher les différents flags possibles.
- `./nob --tests`   Pour faire tourner les tests mis en place.

Note: Tous les flags peuvent être utilisés independamment des autres.  
Exemple: `./nob --clean --run` pour reconstruire le programme en supprimant le cache.  h
