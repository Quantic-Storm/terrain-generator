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

    cout << "Program launched\n";
    srand(time(nullptr));
    Perlin p; //(6874163524165342);
    HeightMap* hm = p.generate(500, 500, 80);
    //hm->print();
    cout << "Map generated !\n";
    hm->saveAsBitMap("C:/Users/benhi/Desktop/original0.bmp");

    HeightMap delta = HeightMap(*hm);
    //return 0;


    cout << "Applying harsh erosion\n";
    // Applying erosion
    Erosion erosionH(0.5, 0.6, 0.05, 0.4, 0.8, false);
    erosionH.applyOn(*hm, 100000);
    cout << "Erosion done !\n";
    hm->saveAsBitMap("C:/Users/benhi/Desktop/original1.bmp");


    cout << "Applying smooth erosion\n";
    // Applying erosion
    Erosion erosionS(0.5, 0.1, 0.01, 0.25, 0.01, true);
    erosionS.applyOn(*hm, 50000);
    cout << "Erosion done !\n";
    hm->saveAsBitMap("C:/Users/benhi/Desktop/original2.bmp");

    //hm->print();
    //delta.substract(*hm);

    //delta.print();

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