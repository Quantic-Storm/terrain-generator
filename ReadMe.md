# Génération de cartes

Ce projet a été réalisé par [Quantic-Storm](https://github.com/Quantic-Storm) and [benoitStH](https://github.com/benoitStH).

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


## References : 
● billet de blog sur la génération de bruit : https://factorio.com/blog/post/fff-390
● Page Wikipedia sur la génération procédurale :https://fr.wikipedia.org/wiki/G%C3%A9n%C3%A9ration_proc%C3%A9durale
● vidéo sur la génération de monde en 3D (Minecraft) : https://www.youtube.com/watch?v=ob3VwY4JyzE
● Papier concernant l’érosion de terrain par particules : https://www.firespark.de/resources/downloads/implementation%20of%20a%20methode%20for%20hydraulic%20erosion.pdf
● Vidéo sur l’érosion par particules : https://www.youtube.com/watch?v=eaXk97ujbPQ
