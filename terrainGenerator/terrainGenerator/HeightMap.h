#pragma once
#include <vector>
#include <iostream>

using namespace std;

class HeightMap {
private:
	vector<vector<float>> terrain;

public:
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


	float getHeightValue(unsigned int x, unsigned int y) { return terrain[x][y]; }
	float getLength() { return terrain[0].size(); }
	float getWidth() { return terrain.size(); }
	void setHeightValue(unsigned int x, unsigned int y, float value) { terrain[x][y] = value; }
	void addHeightValue(unsigned int x, unsigned int y, float value) { terrain[x][y] += value; }

	void print() {
		float min = 10000, max = -10000;
		int i = 0;
		for (vector<float> col : terrain) {
			cout << i << "\t";
			i++;
			for (float el : col) {
				if (el < min) min = el;
				if (el > max) max = el;
				cout << Value_character(el);
			}
			cout << endl;
		}
		cout << "min = " << min << ", max = " << max << endl;
	}

	string Value_character(float value)
	{
		string symbole = "#"; // default symbole for higher out of bound value
		std::vector<string> liste;  // liste of symbols from low value to high value

		liste.push_back(" ");
		liste.push_back(":");
		liste.push_back("-");
		liste.push_back("=");
		//liste.push_back("|");
		liste.push_back("I");
		//liste.push_back("O");
		liste.push_back("0");
		liste.push_back("@");

		for (unsigned int i = 0; i < liste.size(); i++)
		{
			if (value < (i * 1.0/liste.size()) - 0.25)
			{
				symbole = liste[i];
				break;
			}
		}

		return symbole;

	}

	HeightMap& substract(HeightMap& heightMap)
	{
		for (unsigned int i = 0; i < getWidth(); i++)
		{
			for (unsigned int j = 0; j < getLength(); j++)
			{
				this->setHeightValue(i, j, getHeightValue(i, j) - heightMap.getHeightValue(i, j));
			}
		}

		return *this;
	}

};