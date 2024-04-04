#include "Perlin.h"
#include "Utils.h"
#include <exception>
#include <cmath>
#include <stdexcept>
#include <random>
#include <algorithm>

// constructeur par défaut
Perlin::Perlin() {
	randoms = vector<char>(256);
	int rdmSeed = rand();
	this->setSeed(rdmSeed);
}

// constructeur avec seed imposée
Perlin::Perlin(long customSeed) {
	randoms = vector<char>(256);
	this->setSeed(customSeed);
}

// permet de changer aléatoirement la seed
void Perlin::changeSeed() {

	// Melange les valeurs al�atoires
	std::shuffle(randoms.begin(), randoms.end(), std::default_random_engine());

}

// algorithme de génération de bruit de Perlin en 2D
HeightMap* Perlin::generate(unsigned int xSize, unsigned int ySize, int chunkSize) {

	// définition des variables
	int x, y;
	HeightMap* res = new HeightMap(xSize, ySize);

	// variables de mesure du temps de calcul
	std::chrono::steady_clock::time_point timeStart = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point timeEnd;
	long duration;

	//vecteurs de génération aux coins du "chunk"
	vector<float> v1(2), v2(2), v3(2), v4(2);

	// vecteurs de position par rapports aux coins du chunk
	vector<float> v1Pos(2), v2Pos(2), v3Pos(2), v4Pos(2);

	// traitement pour chaque valeur de la première dimension
	for (x = 0; x < xSize; x++) {

		// calcul vecteurs position
		v1Pos[0] = (x % chunkSize);
		v2Pos[0] = (x % chunkSize) - chunkSize;
		v3Pos[0] = v1Pos[0];
		v4Pos[0] = v2Pos[0];

		// normalisation (valeurs voulues entre -1 et 1)
		v1Pos[0] /= chunkSize;
		v2Pos[0] /= chunkSize;
		v3Pos[0] /= chunkSize;
		v4Pos[0] /= chunkSize;


		float t1 = v1Pos[0];

		// poids pour l'interpolation
		float w1 = 3 * t1 * t1 - 2 * t1 * t1 * t1;

		// traitement pour la valeur de la deuxième dimension, interpolation et sauvegarde du résultat
		for (y = 0; y < ySize; y++) {

			// récuparation des vecteurs de génération
			v1[0] = getVecX(floor((float)x / chunkSize), floor((float)y / chunkSize)) / 128.;
			v1[1] = getVecY(floor((float)x / chunkSize), floor((float)y / chunkSize)) / 128.;

			v2[0] = getVecX(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize)) / 128.;
			v2[1] = getVecY(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize)) / 128.;

			v3[0] = getVecX(floor((float)x / chunkSize), floor((float)y / chunkSize) + 1) / 128.;
			v3[1] = getVecY(floor((float)x / chunkSize), floor((float)y / chunkSize) + 1) / 128.;

			v4[0] = getVecX(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize) + 1) / 128.;
			v4[1] = getVecY(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize) + 1) / 128.;

			// calcul des vecteurs position
			v1Pos[1] = (y % chunkSize);
			v2Pos[1] = v1Pos[1];
			v3Pos[1] = (y % chunkSize) - chunkSize;
			v4Pos[1] = v3Pos[1];

			// normalisation entre -1 et 1
			v1Pos[1] /= chunkSize;
			v2Pos[1] /= chunkSize;
			v3Pos[1] /= chunkSize;
			v4Pos[1] /= chunkSize;


			float t2 = (float)(y % chunkSize) / chunkSize;
			// poids pour l'interpolation
			float w2 = 3 * t2 * t2 - 2 * t2 * t2 * t2;

			//interpolation des produits scalaires
			float h1 = Utils::interpolate(Utils::dotProduct(v1Pos, v1), Utils::dotProduct(v2Pos, v2), w1);
			float h2 = Utils::interpolate(Utils::dotProduct(v3Pos, v3), Utils::dotProduct(v4Pos, v4), w1);

			//sauvegarde du résultat
			res->setHeightValue(x, y, Utils::interpolate(h1, h2, w2));
		}
	}

	// mesure du temps
	timeEnd = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();

	verbose << duration << " ms \n";

	return res;
}