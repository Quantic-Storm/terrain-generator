#include "Utils.h"
#include <stdexcept>


// implémentation du produit scalaire de deux vecteurs de meme taille
float Utils::dotProduct(std::vector<float> v1, std::vector<float> v2) {
	if (v1.size() != v2.size()) throw std::invalid_argument("vector must be same size");
	unsigned int i;
	float res = .0;

	// somme des produits de chaque dimension du vecteur 1 avec le vecteur 2
	for (i = 0; i < v1.size(); i++) {
		res += v1[i] * v2[i];
	}
	return res;
}
