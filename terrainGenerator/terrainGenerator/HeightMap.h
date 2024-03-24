#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include "qdbmp.h"

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
	float getMaxHeight() {
		float max = 0;

		for (unsigned int i = 0; i < terrain.size(); i++)
		{
			float new_max = *std::max_element(terrain[i].begin(), terrain[i].end());
			if (new_max > max)
			{
				max = new_max;
			}
		}

		return max;
	}

	float getMinHeight() {
		float min =0;

		for (unsigned int i = 0; i < terrain.size(); i++)
		{
			float new_min = *std::min_element(terrain[i].begin(), terrain[i].end());
			if (new_min < min)
			{
				min = new_min;
			}
		}
		return min;
	}

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

	int saveAsBitMap(const char* filename)
	{

		std::vector<int> ColorList;

		/*for (int i = 0; i < 255; i++)
		{
			ColorList.push_back(i);
		}*/

		ColorList.push_back(0);
		ColorList.push_back(25);
		ColorList.push_back(50);
		ColorList.push_back(70);
		ColorList.push_back(80);
		ColorList.push_back(95);
		ColorList.push_back(110);
		ColorList.push_back(125);
		ColorList.push_back(140);
		ColorList.push_back(155);
		ColorList.push_back(170);
		ColorList.push_back(185);
		ColorList.push_back(200);
		ColorList.push_back(230);
		ColorList.push_back(255);



		cout << "Generating image...\n";
		BMP* bmp = BMP_Create(this->getWidth(), this->getLength(), 32);

		UINT width = BMP_GetWidth(bmp);
		UINT height = BMP_GetHeight(bmp);

		/* Iterate through all the image's pixels */
		for (UINT x = 0; x < width; ++x)
		{
			for (UINT y = 0; y < height; ++y)
			{
				float height = this->getHeightValue(x, y);
				//std::cout << height << "\n";
				for (unsigned int i = 0; i < ColorList.size(); i++)
				{
					if (height < (i * 1.0 / ColorList.size()) - 0.25)
					{
						/* Set new color */
						BMP_SetPixelRGB(bmp, x, y, ColorList[i], ColorList[i], ColorList[i]);
						break;
					}
				}
				
			}
		}

		cout << "Saving image...\n";

		BMP_WriteFile(bmp, filename);
		if (BMP_GetError() != BMP_OK)													\
		{																				\
			fprintf((stdout), "BMP error: %s\n", BMP_GetErrorDescription());	\
			return -2;														\
		}

		/* Free all memory allocated for the image */
		BMP_Free(bmp);
		return 0;
	}

};