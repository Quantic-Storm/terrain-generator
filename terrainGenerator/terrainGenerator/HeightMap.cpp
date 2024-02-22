#include "HeightMap.h"
#include <stdexcept>



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

HeightMap HeightMap::operator*(int coef) {

	HeightMap res(getWidth(), getLength());
	unsigned int i, j;
	for (i = 0; i < getWidth(); i++) {
		for (j = 0; j < getLength(); j++) {
			res.setHeightValue(i, j, getHeightValue(i, j) * coef);
		}
	}
	return res;
}