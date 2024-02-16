// terrainGenerator.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "HeightMap.h"
#include "Perlin.h"
#include "Erosion.h"

/*
int main()
{
    HeightMap hm = HeightMap(25, 24);
    std::cout << hm.getWidth() << ", " << hm.getLength() << std::endl;
}*/


//main de test perlin

int main() {
    Perlin p(6874163524165342);
    HeightMap* hm = p.generate(200, 200, 20);
    hm->print();

    // Applying erosion
    Erosion erosion;
    erosion.applyOn(*hm, 200);

    delete(hm);
}


// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.