#include "HeightMap.h"
#include <stdexcept>
#include <limits>


// opérateur + permettant de sommer deux heightMap de meme dimensions point par point
HeightMap HeightMap::operator+(const HeightMap& other) {
	if (other.getLength() != getLength() || other.getWidth() != getWidth())
		throw std::invalid_argument("heightmaps are not the same size");

	HeightMap res(getWidth(), getLength());
	unsigned int i, j;
	for (i = 0; i < getWidth(); i++) {
		for (j = 0; j < getLength(); j++) {
			res.setHeightValue(i, j, getHeightValue(i, j) + other.getHeightValue(i, j));
		}
	}
	return res;
}

// opérateur de multiplication par un réel
HeightMap HeightMap::operator*(float coef) {

	HeightMap res(getWidth(), getLength());
	unsigned int i, j;
	for (i = 0; i < getWidth(); i++) {
		for (j = 0; j < getLength(); j++) {
			res.setHeightValue(i, j, getHeightValue(i, j) * coef);
		}
	}
	return res;
}

// trouve et enregistre les valeurs min et max dans le heightmap
void HeightMap::computeMinMaxValues()
{
	float max = std::numeric_limits<float>::min();
	float min = std::numeric_limits<float>::max();

	for (std::vector<float>col : terrain) {
		for (float el : col) {
			if (el > max) max = el;
			if (el < min) min = el;
		}
	}
	max_value = max;
	min_value = min;
}

// retourne la valeur maximale trouvée dans le heightmap
float HeightMap::getMaxValue() {
	return max_value;
}

// retourne la valeur minimale trouvée dans le heightmap
float HeightMap::getMinValue() {
	return min_value;
}
