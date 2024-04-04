// terrainGenerator.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <ctime>
#include "Map.h"


std::vector<unsigned int> initVector()
{
    std::vector<unsigned int> v;
    v.push_back(0);
    return v;
}

unsigned int Verbose::level;
unsigned int Verbose::maxRequiredLevel;
std::vector<unsigned int> Verbose::requiredLevels = initVector();



/*
* Fonction principale de notre programme.
* Gère les paramètres -vrb et -seed passés à l'exécutable
* Lance le générateur de carte, puis enregistre le fichier dans "map_img.bmp"
* 
*/
int main(int argc, char* argv[]) {

    // paramètres par défaut
    unsigned int verboseLvl = 0;
    srand(time(0));
    unsigned int seed = rand();


    //lecture des éventuels paramètres (voir le README pour les détails des params)
    for (unsigned int i = 0; i < argc-1; i++) {
        
        // lecture du paramètre de verbose
        if (std::string(argv[i]) == std::string("-vrb")) {
            verboseLvl = atoi(argv[i + 1]);
        }
        // lecture du paramètre de seed
        if (std::string(argv[i]) == std::string("-seed")) {
            seed = atoi(argv[i + 1]);
        }
    }
    
    // creation verbose
    Verbose verbose;
    verbose.setLevel(verboseLvl);

    verbose.setRequiredLevel(1);
    verbose << "Program Launched\n";
    verbose.endRequiredLevel();

    // génération de la carte
    Map map(seed, 1690, 1080);

    // enregistrement dans le fichier
    map.build_image("map_img.bmp");
}
