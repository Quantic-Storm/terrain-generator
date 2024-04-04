# Génération de cartes

Ce projet a été réalisé par @Quantic-Storm and @benoitStH.

## Installation

Le code a été réalisé en C++ sous l'IDE Visual Studio 2019.
Il est possible de récupérer les fichiers sources de ce projet à l'aide de ce même IDE.

Une fois le code récupéré, générez l'exécutable en Release (x64).

## Utilisation

Le programme doit être utilisé dans une interface console.
Il est possible de fournir des paramètres lors de l'utilisation de l'exécutable.

```
generator.exe -vrb <unsigned int> -seed <unsigned int>
```

-vrb permet de définir le niveau de verbose souhaité pour vérifier l'exécution du programme. Allant de 0 (pas d'affichage) à 2 (affichage détaillé)

-seed permet de définir la graine de génération aléatoire du programme. La même carte sera générée à partir de la même graine de génération.
Pour obtenir, à chaque exécution du programme, une carte différente, utilisez un script qui récupère la valeur de l'horloge interne de votre appareil.

Une fois terminé, une image au format '.bmp' sera créé dans le répertoire de l'exécutable. 

## Exemple

Voici ce que l'on obtient pour une image 1690x1080 avec une graine de génération 216.

![Exemple de Carte](ExempleCarte.bmp)