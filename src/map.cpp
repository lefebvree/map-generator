
#include "../inc/map.h"
using namespace std;


 //  Map Data
int  map_elevation     [MAP_HEIGHT * MAP_WIDTH] = { };
int  map_temperature   [MAP_HEIGHT * MAP_WIDTH] = { };
int  map_precipitation [MAP_HEIGHT * MAP_WIDTH] = {0};

int  map_terrain_type  [MAP_HEIGHT * MAP_WIDTH] = { };
int  map_biome         [MAP_HEIGHT * MAP_WIDTH] = { };

int main ( int argc, char const ** argv ) {

	// Convert potential argument to seed (int)
	// 1476314752
	int seed;
	if (argc > 1) {
		seed = 0;
	    while (*argv[1]) seed = seed << 1 ^ *argv[1]++;
	} else seed = time(0);

	consoleHeader();

	mapGeneration(seed);

	imagesGeneration();

	// saveToDatabase();

	return 0;
}


void mapGeneration(int seed) {

	cout << " - TERRAIN CONSTRUCTION" << endl;
	cout << "    size: [" << MAP_WIDTH << "x" << MAP_HEIGHT << "]" << endl;
	cout << "    seed: " << seed << endl;

	// Create Land, Ocean and Rivers
	altitudeGeneration(seed);

	cout << endl << " - RIVER GENERATION" << endl;

	riverGeneration(seed);

	cout << endl << " - TILES ATTRIBUTES" << endl;

	// Mark Coastlines and Precipitation level
	coastlinesGeneration();
	precipitationGeneration();
	precipitationCalibration();

	// Uses Latitude and Elevation to find Temperature
	temperatureGeneration();

	// Assign Biome regarding Precipitation, Temperature, Elevation and Terrain
	biomeGeneration();

	cout << endl;
}


// Map representations (PPM Format)
void imagesGeneration() {

	cout << " - IMAGES REPRESENTATIONS" << endl;

	elevationImageGeneration();
	landmapImageGeneration();
	precipitationImageGeneration();
	temperatureImageGeneration();
	biomeImageGeneration();

	cout << endl;
}


// Return Index between 0 and MAP_WIDTH * MAP_HEIGHT
// from coordinates x, y
int getWrappedIndex (int x, int y) {
	return ((y & (MAP_HEIGHT - 1)) * MAP_WIDTH) + (x & (MAP_WIDTH - 1));
	// Wrap values around borders
	//   3|1    3|1  //
	//  --|------|-- //
	//   2|4    2|4  //
	//   3|1    3|1  //
	//  --|------|-- //
	//   2|4    2|4  //
	// Coordinates can be negatives or farther than limits
}


// Elevation ( MAP_ELEVATION_MIN < Altitude < MAP_ELEVATION_MAX )
int getAltitude (int x, int y) {
	return map_elevation[getWrappedIndex(x, y)];
}
void setAltitude (int x, int y, int altitude) {
	altitude = (altitude > MAP_ELEVATION_MIN) ? altitude : MAP_ELEVATION_MIN;
	altitude = (altitude < MAP_ELEVATION_MAX) ? altitude : MAP_ELEVATION_MAX;

	int index = getWrappedIndex(x, y);

	map_terrain_type[index] = (altitude > MAP_SEA_LEVEL) ? TERRAIN_TYPE_LAND : TERRAIN_TYPE_OCEAN;

	map_elevation[index] = altitude;
}


// Terrain Type ( Land, Ocean, River, Lake or Coast )
int getTerrainType(int x, int y) {
	return map_terrain_type[getWrappedIndex(x, y)];
}
void setTerrainType (int x, int y, int terrain) {
	map_terrain_type[getWrappedIndex(x, y)] = terrain;
}


// 	Land Biomes (SNOW MOUNTAINS, SNOW PLAINS, TEMPERATE RAIN FOREST, RAIN FOREST, TAIGA, TEMPERATE FOREST, SWAMP, SAVANNA PLATEAU, SAVANNA, TUNDRA, WOODLAND, GRASSLAND, MESA, DESERT)
// 	Aquatic Biomes (CORAL, GROVE, OCEAN, ARCTIC)
int getBiome(int x, int y) {
	return map_biome[getWrappedIndex(x, y)];
}
void setBiome (int x, int y, int biome) {
	map_biome[getWrappedIndex(x, y)] = biome;
}
int getBiomeType (int terrain, int temperature, int precipitation, int altitude) {

	if (terrain == TERRAIN_TYPE_OCEAN) {
		int deapth = MAP_SEA_LEVEL - altitude;

		// Oceans at MAP_OCEAN_DEAPTH
		if (deapth > MAP_OCEAN_DEAPTH) return BIOME_OCEAN;

		// Biome based on temperature
		else {
			// Cold Biome
			if (temperature < TEMPERATURE_LEVEL_LOW) {
				return BIOME_ARCTIC;
			}
			// Temperate Biome
			else if (temperature < TEMPERATURE_LEVEL_HIGH) {
				return BIOME_GROVE;
			}
			// Hot Biome
			else {
				return BIOME_CORAL;
			}
		}

	} else {

		// Cold Biomes
		if (temperature < TEMPERATURE_LEVEL_LOW) {
			// Low precipitation
			if (precipitation < PRECIPITATION_LEVEL_LOW) {
				return BIOME_TUNDRA;
			}
			// Moderate precipitation
			else if (precipitation < PRECIPITATION_LEVEL_HIGH) {
				return BIOME_TAIGA;
			}
			// High precipitation
			else {
				return (altitude > MAP_MOUNTAIN_HEIGHT) ? BIOME_SNOWMOUNTAINS : BIOME_SNOWPLAINS;
			}
		}

		// Temperate Biomes
		else if (temperature < TEMPERATURE_LEVEL_HIGH) {
			// Low precipitation
			if (precipitation < PRECIPITATION_LEVEL_LOW) {
				return (altitude > MAP_MOUNTAIN_HEIGHT) ? BIOME_CHAPARRAL : BIOME_MOUNTAIN;
			}
			// Moderate precipitation
			else if (precipitation < PRECIPITATION_LEVEL_HIGH) {
				return (altitude > MAP_MOUNTAIN_HEIGHT) ? BIOME_WOODLAND : BIOME_PRAIRIE;
			}
			// High precipitation
			else {
				return BIOME_FOREST;
			}
		}

		// Hot Biomes
		else {
			// Low precipitation
			if (precipitation < PRECIPITATION_LEVEL_LOW) {
				return (altitude > MAP_MOUNTAIN_HEIGHT) ? BIOME_MESA : BIOME_DESERT;
			}
			// Moderate precipitation
			else if (precipitation < PRECIPITATION_LEVEL_HIGH) {
				return (altitude > MAP_MOUNTAIN_HEIGHT) ? BIOME_SAVANNA : BIOME_GRASSLAND;
			}
			// High precipitation
			else {
				return BIOME_RAINFOREST;
			}
		}
	}
}


// Temperature ( MAP_TEMPERATURE_MIN < Temperature < MAP_TEMPERATURE_MAX )
int getTemperature(int x, int y) {
	return map_temperature[getWrappedIndex(x, y)];
}
void setTemperature (int x, int y, int temp) {
	map_temperature[getWrappedIndex(x, y)] = temp;
}


// Precipitation level ( 0 < Precipitation < 100)
int getPrecipitation(int x, int y) {
	return map_precipitation[getWrappedIndex(x, y)];
}
void setPrecipitation(int x, int y, int precipitation) {
	map_precipitation[getWrappedIndex(x, y)] = precipitation;
}
// Increase tiles precipitation level in radius by one
void increasePrecipitaion (int x, int y, int radius) {
	int radius2 = radius*radius;
	for (int a = -radius; a < radius; a++) {
		for (int b = -radius; b < radius; b++) {
			if (a*a + b*b <= radius2) {
				map_precipitation[getWrappedIndex(x+a,y+b)]++;
	        }
		}
	}
}


void consoleHeader() {
	cout << endl << "      _  _     __    P  R  O  J  E  C  T       _" << endl;
	cout << "     FJ / ;    LJ    _ ___       ___ _      ___FJ      ____      _ _____" << endl;
	cout << "    J |/ (|         J '__ J     F __` L    F __  L    F __ J    J '_  _ `," << endl;
	cout << "    |     L    FJ   | |__| |   | |--| |   | |--| |   | |--| |   | |_||_| |" << endl;
	cout << "    F L:\\  L  J  L  F L  J J   F L__J J   F L__J J   F L__J J   F L LJ J J" << endl;
	cout << "   J__L \\\\__L J__L J__L  J__L  )-____  L J\\____,__L J\\______/F J__L LJ J__L" << endl;
	cout << "   |__L  \\L_| |__| |__L  J__| J\\______/F  J____,__F  J______F  |__L LJ J__|" << endl;
	cout << "                               J______/" << endl;
	cout << "                                              M a p   G e n e r a t i o n" << endl;
	cout << endl;
}
