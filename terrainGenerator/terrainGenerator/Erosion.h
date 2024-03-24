#pragma once

#include "HeightMap.h"
#include <math.h>
#include <random>

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
	//float values; // necessarry constants for erosion (None for now)
	float inertia = 0.5; // [0, 1]
	float depositionRate = 0.02; // [0, 1]
	float evaporationRate = 0.01; // [0, 1]
	float radius = 0.25; // [0, 1]
	float erosionFactor = 0.08; // [0, 1]

	bool sourceErosion = false;
	std::vector<std::vector<unsigned int>> sources; // list of possible spawn point (x, y) for droplets (when source erosion enabled)
	unsigned int sourcePointNumber = 0; // index of the current source point used

public:
	Erosion();

	Erosion(float inertia, float depositionRate, float evaporationRate, float radius, float erosionFactor, bool sourceErosion) :
		inertia(inertia), depositionRate(depositionRate), evaporationRate(evaporationRate), radius(radius), erosionFactor(erosionFactor), sourceErosion(sourceErosion) {};

	/**
	 * Method that applies erosion on a given heightMap
	 * Doesn't into account an erosion Map yet (could be useful to know where it is more friable than wherever else)
	 * @param heighMap The map to apply erosion to.
	 * @param nbDroplets The total number of rain drop to apply
	 */
	void applyOn(HeightMap& heightMap, unsigned int nbDroplets);

	/**
	 * Method which will spawn a droplet at a random position and handle its descent on the given heightMap
	 * The height map will be eroded on the droplet path and updated
	 * @param heightMap The height Map to erode with a droplet
	 */
	void applyDroplet(HeightMap& heightMap);

private:

	/**
	 * Method that generate a rain drop on a given heightMap at a random position
	 * @param heightMap The map where the droplet will be generated.
	 * @return A RainDrop structure representing the generated Droplet
	 */
	RainDrop createDroplet(HeightMap& heightMap);

	/**
	* Method that generate a rain drop on the given heightMap at a random summit (>80% maxHeight)
	* @param heightMap The map where the droplet will be generated
	* @return A RainDrop structure representing the generated droplet
	*/
	RainDrop createSourceDroplet(HeightMap& heightMap);

	/**
	 * Method that return the height of a rain drop on a given heightMap
	 * @param heighMap The map to consider.
	 * @param droplet The rain drop to consider.
	 * @return The droplet's interpolated height
	 */
	float interpolatedHeight(HeightMap& heightMap, const RainDrop& droplet);

	/**
	 * Method that return the interpolated Gradient of the given height map slope the given droplet is positionned
	 * Return two values, one for each interpolation axis (X, Y)
	 * @param heightMap The height map where we will calculate the gradient
	 * @param droplet The droplet (whose position is on the heightMap)
	 * @return A std::vector<float> corresponding to the interpolated Gradient
	 */
	std::vector<float> interpolatedGradient(HeightMap& heightMap, const RainDrop& droplet);

	/**
	 * Method which Erode a given amount from the given height map according to a given droplet
	 * The height map is updated on the droplet position and the eroded amount is calculated according to the droplet size and position.
	 * @param heightMap The heightMap to erode
	 * @param droplet The droplet which will erode the heightMap
	 * @param amountToErode The total amount of eroded material
	 */
	void ErodeFrom(HeightMap& heightMap, const RainDrop& droplet, float amountToErode);

	/**
	 * Method which Deposit a given amount on the given height map according to a given droplet
	 * The height map is updated on the droplet position and the deposited amount is calculated according to the droplet position.
	 * @param heightMap The heightMap which will receive the deposit
	 * @param droplet The droplet which will deposit sediment
	 * @param amountToErode The total amount of deposited sediment
	 */
	void DepositOn(HeightMap& heightMap, const RainDrop& droplet, float amountToDeposit);

};

