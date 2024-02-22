#pragma once
#include <vector>
#include <iostream>

using namespace std;

class HeightMap {
private :
	vector<vector<float>> terrain;

public :
	HeightMap() {};
	HeightMap(unsigned int width, unsigned int length) {
		terrain = vector<vector<float>>(width);
		unsigned int i;
		for (i = 0; i < terrain.size(); i++) {
			terrain[i] = vector<float>(length);
		}
	}
	HeightMap(HeightMap& hm) = default;
	HeightMap(HeightMap&& hm) = default;
	~HeightMap() = default;

	HeightMap& operator+(const HeightMap& other);

	float getHeightValue(unsigned int x, unsigned int y) const{ return terrain[x][y]; }
	float getLength() const{ return terrain[0].size(); }
	float getWidth() const{ return terrain.size(); }
	void setHeightValue(unsigned int x, unsigned int y, float value) { terrain[x][y] = value; }
	void addHeightValue(unsigned int x, unsigned int y, float value) { terrain[x][y] += value; }

	void print() {
		float min = 10000, max = -10000;
		int i = 0;
		int nbel = 0;
		for (vector<float> col : terrain) {
			cout << i << "\t";
			i++;
			for (float el : col) {
				if (el < min) min = el;
				if (el > max) max = el;
				cout << ((el >= 0) ? "#" : " ");
				if (el > 0) nbel++;
			}
			cout << endl;
		}
		cout << "min = " << min << ", max = " << max << ", nb cases pleines = " << nbel << endl;

	}

};