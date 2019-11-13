# Projet Senkin

Le but du projet est de réaliser un jeu inspiré des mécaniques de Dofus et de Fire Emblem.

## Compilation

```
cmake *Emplacement du projet*/
make
```

## Rendu

##### Lancer le mode rendu :
```
./bin/client render
```

## Moteur de jeu

##### Lancer le mode engine :
```
./bin/client engine
```

##### Ajouter des commandes :
Ouvrir src/client/main.cpp et se placer dans la partie "engine".  
Ces commandes s'appliquent automatiquement au joueur qui a le tour d'action.

Se déplacer (exemple : aux coordonnées [11,13])
```
commandList.push_back(new Move({11,13}));
```
Utiliser une compétence/attaque (exemple : compétence n°1 appliqué sur la case [11,13])
```
commandList.push_back(new Attack(std::make_pair (11,13) , 1));
```
Terminer son tour
```
commandList.push_back(new EndActions());
```
