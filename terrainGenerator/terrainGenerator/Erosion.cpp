#include "Erosion.h"


void Erosion::applyOn(HeightMap& heightMap, unsigned int nbDroplets)
{
	if (sourceErosion)
	{
		verbose << "Computing source points...\n";
		float maxHeight = heightMap.getMaxValue();

		for (unsigned int x = 0; x < heightMap.getWidth(); x++)
		{
			for (unsigned int y = 0; y < heightMap.getLength(); y++)
			{
				if (0.5 * maxHeight < heightMap.getHeightValue(x, y) && heightMap.getHeightValue(x, y) < 0.95 * maxHeight)
				{
					std::vector<unsigned int> sourcePoint;
					sourcePoint.push_back(x);
					sourcePoint.push_back(y);

					sources.push_back(sourcePoint);
				}
			}
		}
		std::shuffle(sources.begin(), sources.end(), std::default_random_engine());
		verbose << "nb of source points detected : " << (unsigned int)sources.size() << "\n";
	}


	for (unsigned int i = 0; i < nbDroplets; i++)
	{
		if ((i+1) % 10000 == 0) {
			verbose << "Droplet " << i+1 << "/" << nbDroplets << "\r";
		}
		applyDroplet(heightMap);
	}
	verbose << "\n";
}

void Erosion::applyDroplet(HeightMap& heightMap)
{
	// Apply erosion
	const float threshold = 0;
	const float minSlope = 0;
	const float capacity = 1;
	const float gravity = 9.8;


	// Spawn random Droplet Or at random summit
	RainDrop droplet;
	if (sourceErosion)
	{
		droplet = createSourceDroplet(heightMap);
	}
	else {
		droplet = createDroplet(heightMap);
	}

	bool outOfBounds = false;
	for (unsigned int nbIter = 0; nbIter < 50; nbIter++)
	{
		

		if (droplet.sediment != droplet.sediment)
		{
			verbose << nbIter << " \n"; verbose << "nan\n";
			outOfBounds = true;
			break;
		}
		// Checking if droplet is out of bound
		if ((droplet.pos[0] < 0) || (droplet.pos[0] >= heightMap.getWidth()) || (droplet.pos[1] < 0) || (droplet.pos[1] >= heightMap.getLength()))
		{
			outOfBounds = true;
			break;
		}

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
		droplet.dir[0] = newDir_X;
		droplet.dir[1] = newDir_Y;

		// Checking if droplet is out of bound (Is there a way without breaking the loop ?)
		if ((droplet.pos[0] < 0) || (droplet.pos[0] >= heightMap.getWidth()) || (droplet.pos[1] < 0) || (droplet.pos[1] >= heightMap.getLength()))
		{
			outOfBounds = true;
			break;
		}

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
		float squared_speed = droplet.speed * droplet.speed - deltaHeight * gravity;
		if (squared_speed < 0) { squared_speed = 0; }
		droplet.speed = sqrt(squared_speed);
		droplet.water = droplet.water * (1 - evaporationRate);




	}

	// If droplet still is on the map with sediment at the end of its life
	if (droplet.sediment > 0 && outOfBounds == false)
	{
		// Deposit droplet's remaining sediment
		DepositOn(heightMap, droplet, droplet.sediment);
		droplet.sediment = 0;
	}
}

RainDrop Erosion::createDroplet(HeightMap& heightMap) // Mettre heightMap en const
{
	// Generate a raindrop at random position on the height map
	unsigned int length = heightMap.getLength();
	unsigned int width = heightMap.getWidth();

	float x, y;


	x = (width - 1) * std::rand() / float(RAND_MAX);
	y = (length - 1) * std::rand() / float(RAND_MAX);


	float angle = std::rand() / float(RAND_MAX);
	float dir_x = cos(angle * 2.0 * PI);
	float dir_y = sin(angle * 2.0 * PI);

	RainDrop droplet;
	droplet.pos.push_back(x);
	droplet.pos.push_back(y);
	droplet.dir.push_back(dir_x);
	droplet.dir.push_back(dir_y);

	droplet.water = 1;
	droplet.sediment = 0;
	droplet.speed = 0;

	return droplet;
}

RainDrop Erosion::createSourceDroplet(HeightMap& heightMap)
{
	// If no summit where found, spawn at random position
	if (sources.size() == 0) { return createDroplet(heightMap); }

	// Randomly choosing a source point to spawn the droplet to
	float x = sources[sourcePointNumber][0];
	float y = sources[sourcePointNumber][1];
	sourcePointNumber = (sourcePointNumber + 1) % sources.size(); // Next index (loop back if at the end of the list)

	float angle = std::rand() / float(RAND_MAX);
	float dir_x = cos(angle * 2.0 * PI);
	float dir_y = sin(angle * 2.0 * PI);

	RainDrop droplet;
	droplet.pos.push_back(x);
	droplet.pos.push_back(y);
	droplet.dir.push_back(dir_x);
	droplet.dir.push_back(dir_y);

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

	unsigned int x1 = (x+1 >= heightMap.getWidth() ? x : x + 1);
	unsigned int y1 = (y+1 >= heightMap.getLength() ? y : y + 1);


	// Get height of the pixels (x, y) (x+1, y) (x, y+1) (x+1, y+1)
	float PXY = heightMap.getHeightValue(x, y);
	float PX1Y = heightMap.getHeightValue(x1, y);
	float PXY1 = heightMap.getHeightValue(x, y1);
	float PX1Y1 = heightMap.getHeightValue(x1, y1);

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

	unsigned int x1 = (x+1 >= heightMap.getWidth() ? x : x + 1);
	unsigned int y1 = (y+1 >= heightMap.getLength() ? y : y + 1);

	// Get height of the pixels (x, y) (x+1, y) (x, y+1) (x+1, y+1)
	float PXY = heightMap.getHeightValue(x, y);
	float PX1Y = heightMap.getHeightValue(x1, y);
	float PXY1 = heightMap.getHeightValue(x, y1);
	float PX1Y1 = heightMap.getHeightValue(x1, y1);

	float gradient_X = (PX1Y - PXY) * (1 - v) + (PX1Y1 - PXY1) * v;
	float gradient_Y = (PXY1 - PXY) * (1 - u) + (PX1Y1 - PX1Y) * u;

	gradient.push_back(gradient_X);
	gradient.push_back(gradient_Y);

	return gradient;
}

void Erosion::ErodeFrom(HeightMap& heightMap, const RainDrop& droplet, float amountToErode)
{
	float cumulativeErosionRate = 0;
	std::vector<float> erosionWeights;

	amountToErode = (amountToErode > 0.0001 ? amountToErode : 0);

	if (radius * 2 <= 1)
	{
		unsigned int x = unsigned int(droplet.pos[0]);
		unsigned int y = unsigned int(droplet.pos[1]);

		//verbose << "ErodedAmount : " << erosionFactor * amountToErode << "\n";
		//verbose << "X : " << unsigned int(droplet.pos[0]) << "  Y : " << unsigned int(droplet.pos[1]) << "\n";

		heightMap.setHeightValue(x, y, heightMap.getHeightValue(x, y) - erosionFactor * amountToErode);
		return;
	}

	for (float i = droplet.pos[0]-radius; i < droplet.pos[0] + radius; i += 1.0)
	{
		for (float j = droplet.pos[1] - radius; j < droplet.pos[1] + radius; j += 1.0)
		{

			if (i < 0 || i > heightMap.getWidth()) { continue; }
			if (j < 0 || j > heightMap.getLength()) { continue; } 

			unsigned int x = unsigned int(i);
			unsigned int y = unsigned int(j);

			float dist2_x = (x - droplet.pos[0]) * (x - droplet.pos[0]);
			float dist2_y = (y - droplet.pos[1]) * (y - droplet.pos[1]);
			float erosionRate = radius - sqrt(dist2_x+dist2_y);
			erosionRate = (erosionRate > 0.0001 ? erosionRate : 0);

			cumulativeErosionRate += erosionRate;
			erosionWeights.push_back(erosionRate);
		}
	}

	if (cumulativeErosionRate == 0) { return; }

	unsigned int k = 0;
	for (float i = droplet.pos[0] - radius; i < droplet.pos[0] + radius; i += 1.0)
	{
		for (float j = droplet.pos[1] - radius; j < droplet.pos[1] + radius; j += 1.0)
		{
			if (i < 0 || i > heightMap.getWidth()) { continue; }
			if (j < 0 || j > heightMap.getLength()) { continue; }

			unsigned int x = unsigned int(i);
			unsigned int y = unsigned int(j);

			// Remove an amount of sediment

			//verbose << "ErodedAmount : " << erosionFactor * amountToErode * erosionWeights[k] / cumulativeErosionRate << "\n";
			//verbose << "X : " << unsigned int(droplet.pos[0]) << "  Y : " << unsigned int(droplet.pos[1]) << "\n";
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

	if (x+1 < heightMap.getWidth())
		heightMap.setHeightValue(x+1, y, heightMap.getHeightValue(x+1, y) + X1Y);
	if(y+1 < heightMap.getLength())
		heightMap.setHeightValue(x, y+1, heightMap.getHeightValue(x, y+1) + XY1);
	if(x+1 < heightMap.getWidth() && y+1 < heightMap.getLength())
		heightMap.setHeightValue(x+1, y+1, heightMap.getHeightValue(x+1, y+1) + X1Y1);
}