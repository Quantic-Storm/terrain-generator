#include "Erosion.h"


void Erosion::applyOn(HeightMap& heightMap)
{
	// Apply erosion
	const float inertia = 0.5; // [0, 1]
	const float threshold = 0;
	const float minSlope = 0;
	const float capacity = 1;
	const float depositionRate = 0.5; // [0, 1]
	const float evaporationRate = 0.1; // [0, 1]
	const float gravity = 9.8;

	// Spawn random Droplet
	RainDrop droplet = createDroplet(heightMap);

	for (unsigned int nbIter = 0; nbIter < 50; nbIter++)
	{

		// Calculate, from droplet's surroundings, its current height and direction of descent
		std::vector<float> gradient = interpolatedGradient(heightMap, droplet);
		float gradient_X = gradient[0];
		float gradient_Y = gradient[1];
		float height = interpolatedHeight(heightMap, droplet);

		float newDir_X = droplet.dir[0] * inertia - gradient_X * (1 - inertia);
		float newDir_Y = droplet.dir[1] * inertia - gradient_Y * (1 - inertia);

		// Select random direction if under a threshold
		if (newDir_X <= threshold && newDir_Y <= threshold)
		{
			float angle = std::rand() / float(RAND_MAX);
			newDir_X = cos(angle * 2.0 * PI);
			newDir_Y = sin(angle * 2.0 * PI);
		}
		else // Normalize direction
		{
			float norm = sqrt(double(newDir_X) * newDir_X + double(newDir_Y) * newDir_Y);
			newDir_X /= norm;
			newDir_Y /= norm;
		}

		// Move the droplet in that direction and get its new position
		droplet.pos[0] = droplet.pos[0] + newDir_X;
		droplet.pos[1] = droplet.pos[1] + newDir_Y;

		// Check its new height and calculate deltaHeight = old - new
		float newHeight = interpolatedHeight(heightMap, droplet);
		float deltaHeight = newHeight - height;

		// Calculate sediment capacity
		float sedimentCapacity = std::max(-deltaHeight, minSlope) * droplet.speed * droplet.water * capacity;


		// If droplet carries more sediment than its capability, the surplus is deposited in its surroundings
		if (droplet.sediment > sedimentCapacity)
		{
			float amountToDeposit = (droplet.sediment - sedimentCapacity) * depositionRate;

			// Deposit the sediment surplus
			DepositOn(heightMap, droplet, amountToDeposit);
			droplet.sediment -= amountToDeposit;

		}
		else if (deltaHeight > 0) // If the droplet is flowing upward
		{
			float amountToDeposit = std::min(deltaHeight, droplet.sediment);

			// Deposit droplet's sediment
			DepositOn(heightMap, droplet, amountToDeposit);
			droplet.sediment -= amountToDeposit;
		}
		else
		{
			float amountToErode = std::min((sedimentCapacity - droplet.sediment), -deltaHeight);

			// Erode the terrain
			ErodeFrom(heightMap, droplet, amountToErode);
			droplet.sediment += amountToErode;
		}

		// We update the droplet's speed and amount of water
		droplet.speed = sqrt(droplet.speed * droplet.speed + deltaHeight * gravity);
		droplet.water = droplet.water * (1 - evaporationRate);




	}
}

RainDrop Erosion::createDroplet(HeightMap& heightMap) // Mettre heightMap en const
{
	// Generate a raindrop at random position on the height map
	unsigned int length = heightMap.getLength();
	unsigned int width = heightMap.getWidth();

	float x = width * std::rand() / float(RAND_MAX);
	float y = length * std::rand() / float(RAND_MAX);

	float angle = std::rand() / float(RAND_MAX);
	float dir_x = cos(angle * 2.0 * PI);
	float dir_y = sin(angle * 2.0 * PI);

	RainDrop droplet;
	droplet.pos[0] = x;
	droplet.pos[1] = y;
	droplet.dir[0] = dir_x;
	droplet.dir[1] = dir_y;

	droplet.water = 1;
	droplet.sediment = 0;
	droplet.speed = 0;

	return droplet;
}

float Erosion::interpolatedHeight(HeightMap& heightMap, const RainDrop& droplet)
{
	// Get its grid position and offset
	unsigned int x = unsigned int(droplet.pos[0]);
	unsigned int y = unsigned int(droplet.pos[1]);
	float u = droplet.pos[0] - x;
	float v = droplet.pos[1] - y;

	// Get height of the pixels (x, y) (x+1, y) (x, y+1) (x+1, y+1)
	float PXY = heightMap.getHeightValue(x, y);
	float PX1Y = heightMap.getHeightValue(x + 1, y);
	float PXY1 = heightMap.getHeightValue(x, y + 1);
	float PX1Y1 = heightMap.getHeightValue(x + 1, y + 1);

	float height = (PXY * (1 - u) * (1 - v) + PX1Y * u * (1 - v) + PXY1 * (1 - u) * v + PX1Y1 * u * v);
	
	return height;
}

std::vector<float> Erosion::interpolatedGradient(HeightMap& heightMap, const RainDrop& droplet)
{
	std::vector<float> gradient;

	// Get its grid position and offset
	unsigned int x = unsigned int(droplet.pos[0]);
	unsigned int y = unsigned int(droplet.pos[1]);
	float u = droplet.pos[0] - x;
	float v = droplet.pos[1] - y;

	// Get height of the pixels (x, y) (x+1, y) (x, y+1) (x+1, y+1)
	float PXY = heightMap.getHeightValue(x, y);
	float PX1Y = heightMap.getHeightValue(x + 1, y);
	float PXY1 = heightMap.getHeightValue(x, y + 1);
	float PX1Y1 = heightMap.getHeightValue(x + 1, y + 1);

	float gradient_X = (PX1Y - PXY) * (1 - v) + (PX1Y1 - PXY1) * v;
	float gradient_Y = (PXY1 - PXY) * (1 - u) + (PX1Y1 - PX1Y) * u;

	gradient.push_back(gradient_X);
	gradient.push_back(gradient_Y);

	return gradient;
}

void Erosion::ErodeFrom(HeightMap& heightMap, const RainDrop& droplet, float amountToErode)
{
	const float radius = 0.5; // [0, 1]
	const float erosionFactor = 0.5; // [0, 1]

	float cumulativeErosionRate = 0;
	std::vector<float> erosionWeights;

	for (float i = droplet.pos[0]-radius; i < droplet.pos[0] + radius; i += 1.0)
	{
		for (float j = droplet.pos[1] - radius; j < droplet.pos[1] + radius; j += 1.0)
		{
			unsigned int x = unsigned int(i);
			unsigned int y = unsigned int(j);

			float dist2_x = (x - droplet.pos[0]) * (x - droplet.pos[0]);
			float dist2_y = (y - droplet.pos[1]) * (y - droplet.pos[1]);
			float depositRate = radius - sqrt(dist2_x+dist2_y);

			cumulativeErosionRate += depositRate;
			erosionWeights.push_back(depositRate);
		}
	}

	unsigned int k = 0;
	for (float i = droplet.pos[0] - radius; i < droplet.pos[0] + radius; i += 1.0)
	{
		for (float j = droplet.pos[1] - radius; j < droplet.pos[1] + radius; j += 1.0)
		{
			unsigned int x = unsigned int(i);
			unsigned int y = unsigned int(j);

			// Remove an amount of sediment
			heightMap.setHeightValue(x, y, heightMap.getHeightValue(x, y) - erosionFactor * amountToErode * erosionWeights[k] / cumulativeErosionRate);
			k++;
		}
	}
}

void Erosion::DepositOn(HeightMap& heightMap, const RainDrop& droplet, float amountToDeposit)
{
	// Get its grid position and offset
	unsigned int x = unsigned int(droplet.pos[0]);
	unsigned int y = unsigned int(droplet.pos[1]);
	float u = droplet.pos[0] - x;
	float v = droplet.pos[1] - y;

	// Calculating distribution using bilinear interpolation
	float XY = amountToDeposit * (1 - u) * (1 - v);
	float X1Y = amountToDeposit * u * (1 - v);
	float XY1 = amountToDeposit * (1 - u) * v;
	float X1Y1 = amountToDeposit * u * v;

	// Depositing sediments
	heightMap.setHeightValue(x, y, heightMap.getHeightValue(x, y) + XY);
	heightMap.setHeightValue(x+1, y, heightMap.getHeightValue(x+1, y) + X1Y);
	heightMap.setHeightValue(x, y+1, heightMap.getHeightValue(x, y+1) + XY1);
	heightMap.setHeightValue(x+1, y+1, heightMap.getHeightValue(x+1, y+1) + X1Y1);
}