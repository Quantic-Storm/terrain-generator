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

float HeightMap::getMaxValue() {
	float max = std::numeric_limits<float>::min();

	for (vector<float>col : terrain) {
		for (float el : col) {
			if (el > max) max = el;
		}
	}
	return max;
}

float HeightMap::getMinValue() {
	float min = std::numeric_limits<float>::max();

	for (vector<float>col : terrain) {
		for (float el : col) {
			if (el < min) min = el;
		}
	}
	return min;
}


void HeightMap::print() {	//todo faire affichage custom entre min et max
	float min = std::numeric_limits<float>::max(),
		max = std::numeric_limits<float>::min();
	int i = 0;

	for (vector<float>col : terrain) {
		for (float el : col) {
			if (el < min) min = el;
			if (el > max) max = el;
		}
	}
	float interval = max - min;
	float quarter = interval / 4;

	for (vector<float> col : terrain) {
		cout << i << "\t";
		i++;
		for (float el : col) {
			char symbol = ' ';

			if (el < min + quarter) symbol = ' ';
			else if (el < min + 2 * quarter) symbol = '.';
			else if (el < min + 3 * quarter) symbol = '*';
			else symbol = '#';
			
			cout << symbol;
		}
		cout << endl;
	}
	cout << "min = " << min << ", max = " << max << endl;

}