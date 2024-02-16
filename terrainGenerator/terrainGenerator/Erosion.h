#pragma once

#include "HeightMap.h"
#include <math.h>

#define PI 3.14159265358979323846

struct RainDrop
{
	float water;	// Amount of water in the droplet
	float sediment; // Ammount of sediment carried by the droplet
	float speed;	// traveling speed of the droplet
	vector<float> dir; // flowing direction of the droplet
	vector<float> pos; // current droplet's position
};

class Erosion
{
private:
	float values; // necessarry constants for erosion (None for now)

public:
	Erosion() : values(0) {};

	/**
	 * Method that applies erosion on a given heightMap
	 * Doesn't into account an erosion Map yet (could be useful to know where it is more friable than wherever else)
	 * @param heighMap The map to apply erosion to.
	 */
	void applyOn(HeightMap& heightMap);

private:

	/**
	 * Method that generate a rain drop on a given heightMap
	 * @param heighMap The map where the droplet will be generated.
	 * @return A RainDrop structure representing the generated Droplet
	 */
	RainDrop createDroplet(HeightMap& heightMap);

	/**
	 * Method that return the height of a rain drop on a given heightMap
	 * @param heighMap The map to consider.
	 * @param droplet The rain drop to consider.
	 * @return The droplet's interpolated height
	 */
	float interpolatedHeight(HeightMap& heightMap, const RainDrop& droplet);

	std::vector<float> interpolatedGradient(HeightMap& heightMap, const RainDrop& droplet);

	void ErodeFrom(HeightMap& heightMap, const RainDrop& droplet, float amountToErode);

	void DepositOn(HeightMap& heightMap, const RainDrop& droplet, float amountToDeposit);

};
