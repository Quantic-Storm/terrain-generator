#pragma once
#include <vector>

namespace Utils {
	float dotProduct(std::vector<float> v1, std::vector<float> v2);
	inline float interpolate(float a, float b, float weight) { return (b - a) * weight + a; };
}