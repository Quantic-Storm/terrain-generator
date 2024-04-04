#pragma once
#include <vector>
#include <iostream>
#include "Verbose.h"

class HeightMap {
private:
	std::vector<std::vector<float>> terrain;	// tableau 2D des valeurs en chaque point
	float min_value;							// valeur minimale trouvée dans le heightmap
	float max_value;							// valeur maximale trouvée dans le heightmap

public:
	HeightMap() { min_value = max_value = .0; }
	HeightMap(unsigned int width, unsigned int length) {
		min_value = max_value = .0;
		terrain = std::vector<std::vector<float>>(width);
		unsigned int i;
		for (i = 0; i < terrain.size(); i++) {
			terrain[i] = std::vector<float>(length);
		}
	}
	HeightMap(HeightMap& hm) = default;
	HeightMap(HeightMap&& hm) = default;
	~HeightMap() = default;

	HeightMap& operator=(const HeightMap& other) = default;
	HeightMap operator+(const HeightMap& other);
	HeightMap operator*(float coef);


	float getMaxValue();
	float getMinValue();

	float getHeightValue(unsigned int x, unsigned int y) const { return terrain[x][y]; }
	float getLength() const { return terrain[0].size(); }
	float getWidth() const { return terrain.size(); }
	void setHeightValue(unsigned int x, unsigned int y, float value) { terrain[x][y] = value; }
	void addHeightValue(unsigned int x, unsigned int y, float value) { terrain[x][y] += value; }


	void computeMinMaxValues();

	void print() {
		float min = 10000, max = -10000;
		int i = 0;
		for (std::vector<float> col : terrain) {
			std::cout << i << "\t";
			i++;
			for (float el : col) {
				if (el < min) min = el;
				if (el > max) max = el;
				std::cout << ((el > 0) ? "#" : " ");
			}
			std::cout << std::endl;
		}
		std::cout << "min = " << min << ", max = " << max << std::endl;
	}

};