
#include "../inc/map.h"
using namespace std;


// Generate Elevation of each tile using diamonf-square algorithm
void altitudeGeneration (int seed) {

	srand(seed);

	// Base points placed at MAP_GRANULARITY intervals with random values

	int totalpoints = (MAP_WIDTH / MAP_GRANULARITY) * (MAP_HEIGHT / MAP_GRANULARITY);
	cout << "    Placing granular points for altitude (" << totalpoints << " points total)... " << flush;

	for (int y = 0; y < MAP_HEIGHT; y+= MAP_GRANULARITY) {
		for (int x = 0; x < MAP_WIDTH; x+= MAP_GRANULARITY) {
			int randomElevation = MAP_ELEVATION_MIN + (rand() % (int)(MAP_ELEVATION_MAX - MAP_ELEVATION_MIN + 1));
			setAltitude(x, y, randomElevation);
		}
	}
	cout << "\r✅  Placing granular points for altitude (" << totalpoints << " points total)         " << endl;

	// Recursively fill tiles
	// MAP_SCALE impact random elevation alteration for each tile

	int samplesize = MAP_GRANULARITY;
	int scale      = MAP_SCALE;

	cout << "    Generating altitude for each tile... " << flush;

	while (samplesize > 1) {
		int halfstep = samplesize / 2;

		// First step: Square attribution
		//   a   b
		//     #
		//   c   d

		for (int y = halfstep; y < MAP_HEIGHT + halfstep; y += samplesize) {
			for (int x = halfstep; x < MAP_WIDTH + halfstep; x += samplesize) {

				int randomElevationAlteration = scale * (rand()%2 - 1);
				squareSet(x, y, samplesize, randomElevationAlteration);

			}
		}

		// Secont step: 2 Diamond attribution to restore a grid layout
		//      c
		//   a  #  b
		//      d

		for (int y = 0; y < MAP_HEIGHT; y += samplesize) {
			for (int x = 0; x < MAP_WIDTH; x += samplesize) {

				int randomElevationAlteration1 = scale * (rand() % 2 + 1);
				diamondSet(x + halfstep, y, samplesize, randomElevationAlteration1);

				int randomElevationAlteration2 = scale * (rand() % 2 + 1);
				diamondSet(x, y + halfstep, samplesize, randomElevationAlteration2);
			}
		}

		samplesize = samplesize / 2;
		scale      = scale / 2;
	}

	cout << "\r✅  Generating altitude for each tile         " << endl;
}


void squareSet (int x, int y, int size, int altitudealteration) {
	// Average elevation is stored in the center tile with possible alteration

	int halfsize = size / 2;

	int a = getAltitude(x - halfsize, y - halfsize);
	int b = getAltitude(x + halfsize, y - halfsize);
	int c = getAltitude(x - halfsize, y + halfsize);
	int d = getAltitude(x + halfsize, y + halfsize);

	setAltitude(x, y, ((a + b + c + d) / 4) + altitudealteration);
}

void diamondSet (int x, int y, int size, int altitudealteration) {
	// Average elevation is stored in the center tile with possible alteration

	int halfsize = size / 2;

	int a = getAltitude(x - halfsize, y);
	int b = getAltitude(x + halfsize, y);
	int c = getAltitude(x, y - halfsize);
	int d = getAltitude(x, y + halfsize);

	setAltitude(x, y, ((a + b + c + d) / 4) + altitudealteration);
}


void riverGeneration (int seed) {

	srand(seed);

	int totalpoints = (MAP_WIDTH / MAP_RIVER_FREQUENCY) * (MAP_HEIGHT / MAP_RIVER_FREQUENCY);
	cout << "    seed: " << seed << endl;
	cout << "    freq: " << MAP_RIVER_FREQUENCY << endl;
	cout << "    maxs: " << totalpoints << endl;

	cout << "    Generating river paths... " << flush;

	int nbRivers = 0;

	// Divide map in tiles [MAP_RIVER_FREQUENCY x MAP_RIVER_FREQUENCY]

	for (int y = 0; y < MAP_HEIGHT - MAP_RIVER_FREQUENCY + 1; y+= MAP_RIVER_FREQUENCY) {
		for (int x = 0; x < MAP_WIDTH - MAP_RIVER_FREQUENCY + 1; x+= MAP_RIVER_FREQUENCY) {

			// Random spawnpoint for current tile
			int riverSpawnX = rand() % MAP_RIVER_FREQUENCY + x;
			int riverSpawnY = rand() % MAP_RIVER_FREQUENCY + y;

			// If spawnpoint is on land and is high enough ( altitude > MAP_RIVER_MIN_ELEVATION )
			if (getTerrainType(riverSpawnX, riverSpawnY) == TERRAIN_TYPE_LAND) {

				int riverSpawnAltitude = getAltitude(riverSpawnX, riverSpawnY);

				if (riverSpawnAltitude > MAP_RIVER_MIN_ELEVATION) {
					// Start river from spawnpoint
					nbRivers++;
					riverFrom(riverSpawnX, riverSpawnY, 0);
				}
			}
		}
	}

	cout << "\r✅  Generating river paths (" << nbRivers << " rivers spawned)" << endl;
}


void riverFrom (int x, int y, int riverLength) {

	// Until sea level
	if (getAltitude(x, y) > MAP_SEA_LEVEL) {

		// Adjacent Tiles
		int tiles [4][4] = {
			{ x, y - 1, getAltitude(x, y - 1), getTerrainType(x, y - 1) },
			{ x - 1, y, getAltitude(x - 1, y), getTerrainType(x - 1, y) },
			{ x, y + 1, getAltitude(x, y + 1), getTerrainType(x, y + 1) },
			{ x + 1, y, getAltitude(x + 1, y), getTerrainType(x + 1, y) }
		};

		// Lowest neighbor altitude
		int lowest = MAP_ELEVATION_MAX + 1;

		for (int i = 0; i < 4; i++) {
			if (tiles[i][3] != TERRAIN_TYPE_RIVER && tiles[i][3] != TERRAIN_TYPE_LAKE) {
				if (tiles[i][2] < lowest) lowest = tiles[i][2];
			}
		}

		// If there is an adjacent land tile (lowest value has changed)
		if (lowest <= MAP_ELEVATION_MAX) {
			setTerrainType(x, y, TERRAIN_TYPE_RIVER);

			// Neighbors at 'lowest' altitude
			int lt [4] = {};
			int n      = 0;

			for (int i = 0; i < 4; i++) {
				if (tiles[i][3] != TERRAIN_TYPE_RIVER && tiles[i][3] != TERRAIN_TYPE_LAKE && tiles[i][2] == lowest) {
					lt[n++] = i;
				}
			}

			// Randomly select a neighbor at 'lowest' altitude to expand river
			int randomland = rand() % n;
			riverFrom(tiles[lt[randomland]][0], tiles[lt[randomland]][1], riverLength + 1);

		} else {
			// No land neighboring tiles found
			setTerrainType(x, y, TERRAIN_TYPE_LAKE);

			// Continu river from higher adjacent river tile
			int l = rand() % 4;
			for (int i = 0; i < 4; i++) if (tiles[i][2] < tiles[l][2]) l = 2;

			riverFrom(tiles[l][0], tiles[l][1], riverLength - 1);
		}
	}
}


void coastlinesGeneration() {

	// Tag each land next to a water tile as COAST

	cout << "    Tagging coastlines... " << flush;

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {

			char terrain = getTerrainType(x,y);

			// Only check neighbor tiles if in aquatic terrain
			if (terrain == TERRAIN_TYPE_OCEAN || terrain == TERRAIN_TYPE_RIVER) {

				int tiles [4][3] = {
					{ x, y - 1, getTerrainType(x, y - 1) },
					{ x - 1, y, getTerrainType(x - 1, y) },
					{ x, y + 1, getTerrainType(x, y + 1) },
					{ x + 1, y, getTerrainType(x + 1, y) }
				};

				for (int i = 0; i <4; i++) {
					if (tiles[i][2] == TERRAIN_TYPE_LAND) setTerrainType(tiles[i][0], tiles[i][1], TERRAIN_TYPE_COAST);
				}
			}
		}
	}

	cout << "\r✅  Tagging coastlines     " << endl;
}


void precipitationGeneration() {

	// Aquatic terrain increase surrounding precipitation level

	cout << "    Generating precipitation levels... " << flush;

	int range2 = MAP_RAIN_CHECK_RANGE * MAP_RAIN_CHECK_RANGE;

	for (int y = 0; y < MAP_HEIGHT; y += MAP_RAIN_CHECK_RANGE) {
		for (int x = 0; x < MAP_WIDTH; x += MAP_RAIN_CHECK_RANGE) {

			int watertiles = 0;

			for (int i = 0; i < MAP_RAIN_CHECK_RANGE; i ++) {
				for (int j = 0; j < MAP_RAIN_CHECK_RANGE; j ++) {

					char terrain = getTerrainType(x+i,y+j);

					if (terrain == TERRAIN_TYPE_OCEAN) watertiles += 3;
					else if (terrain == TERRAIN_TYPE_COAST) watertiles += 40;
					else if (terrain == TERRAIN_TYPE_RIVER) watertiles += 50;
					else if (terrain == TERRAIN_TYPE_LAKE) watertiles += 60;
				}
			}

			int prec = watertiles / range2;

			int radius = prec * MAP_RAIN_RADIUS;

			increasePrecipitaion(x, y, radius);
		}
	}

	cout << "\r✅  Generating precipitation levels      " << endl;
}


void precipitationCalibration() {

	// Calibrate precipitation level to 0 - 100 range

	cout << "    Calibrating precipitation levels... " << flush;

	int precipitationmax = 0;

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			int precipitation = getPrecipitation(x, y);
			if (precipitation > precipitationmax) precipitationmax = precipitation;
		}
	}

	float calibrationfactor = (float)MAP_PRECIPITATION_MAX / (float) (precipitationmax + 1);

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			int newprecipitation = (float)getPrecipitation(x, y) * calibrationfactor;
			setPrecipitation(x, y, newprecipitation);
		}
	}

	cout << "\r✅  Calibrating precipitation levels (from " << precipitationmax << " to " << MAP_PRECIPITATION_MAX << ")" << endl;
}


void temperatureGeneration() {

	// Tile temperature calculated from its altitude and latitude

	cout << "    Generating temperatures... " << flush;

	// Hottest latitude
	int equator      = MAP_HEIGHT / 2;
	int maxelevation = MAP_ELEVATION_MAX - MAP_SEA_LEVEL;

	int temperaturerange = MAP_TEMPERATURE_MAX - MAP_TEMPERATURE_MIN;

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {

			int altitude  = getAltitude(x,y);
			int elevation = abs(altitude - MAP_SEA_LEVEL);

			// Factors from 0 to 1
			float alt = (1.0 - (float)elevation / (float)maxelevation) * TEMPERATURE_ALTITUDE_SIGNIFICANCE;
			float lat = 1.0 - (abs(y - equator) / (float)equator * TEMPERATURE_LATITUDE_SIGNIFICANCE);

			int temperature = (float)(alt + lat) * (float)temperaturerange + MAP_TEMPERATURE_MIN;

			setTemperature(x, y, temperature);
		}
	}
	cout << "\r✅  Generating temperatures      " << endl;
}


void biomeGeneration() {

	// Biome get assigned to a tile depending on its terrain, temperature, precipitation level and altitude

	cout << "    Generating biomes... " << flush;

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {

			char terrain      = getTerrainType(x, y);
			int temperature   = getTemperature(x, y);
			int precipitation = getPrecipitation(x, y);
			int altitude     = getAltitude(x, y);

			char biome = getBiomeType(terrain, temperature, precipitation, altitude);

			setBiome(x, y, biome);
		}
	}

	cout << "\r✅  Generating biomes      " << endl;
}
