#include "HeightMap.h"
#include <stdexcept>
#include <limits>



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

void HeightMap::computeMinMaxValues()
{
	float max = std::numeric_limits<float>::min();

	for (std::vector<float>col : terrain) {
		for (float el : col) {
			if (el > max) max = el;
		}
	}
	max_value = max;

	float min = std::numeric_limits<float>::max();

	for (std::vector<float>col : terrain) {
		for (float el : col) {
			if (el < min) min = el;
		}
	}
	min_value = min;
}

float HeightMap::getMaxValue() {
	return max_value;
}

float HeightMap::getMinValue() {
	return min_value;
}
