
#include "../inc/map.h"
using namespace std;


void elevationImageGeneration() {

	cout << "      Writting Elevation map... ";

	// P3 Format: rgb (255)

	char header[17];
	FILE * image = fopen ("images/elevation_map.ppm", "wb");
	sprintf(header, "P3\n%04d %04d\n255\n", MAP_WIDTH, MAP_HEIGHT);
	fwrite(&header, sizeof(char), sizeof(header), image);

	for (int y = 0; y < MAP_HEIGHT; y++)  {
		for (int x = 0; x < MAP_WIDTH; x++) {

			// RGB values
			int r, g, b;

			switch (getTerrainType(x, y)) {
				case TERRAIN_TYPE_LAND: {
						// r value in function of tile elevation
						int elevation = getAltitude(x, y);
						r = ((float)(elevation - MAP_SEA_LEVEL) / (float)(MAP_ELEVATION_MAX - MAP_SEA_LEVEL)) * 170;
						b = 0; g = 50;
					} break;

				case TERRAIN_TYPE_OCEAN: {
						// g value in function of ocean deapth
						int deapth = getAltitude(x, y);
						g = 10 + ((float)deapth / (float)MAP_SEA_LEVEL) * 80;
						r = 0; b = 100;
					} break;

				case TERRAIN_TYPE_RIVER:
					r = 0; g = 110; b = 170;
					break;

				case TERRAIN_TYPE_LAKE:
					r = 0; g = 72; b = 102;
					break;

				case TERRAIN_TYPE_COAST:
					r = 160; g = 160; b = 0;
					break;

				default:
					r = 0; g = 0; b = 0;
			}

			char level[12];
			sprintf(level, "%03d %03d %03d ", r, g, b);

			fwrite(&level, sizeof(char), sizeof(level), image);
		}

		fwrite("\n", sizeof(char), 1, image);
	}

	fclose(image);

	cout << "\r✅  Writting Elevation map (\"images/elevation_map.ppm\")" << endl;
}

void landmapImageGeneration () {

	cout << "      Writting Land map... ";

	// P1 format: either 1 or 0

	char header[13];
	FILE * image = fopen ("images/land_map.ppm", "wb");
	sprintf(header, "P1\n%04d %04d\n", MAP_WIDTH, MAP_HEIGHT);
	fwrite(&header, sizeof(char), sizeof(header), image);

	for (int y = 0; y < MAP_HEIGHT; y++)  {
		for (int x = 0; x < MAP_WIDTH; x++) {

			char isLand [2];
			sprintf(isLand, "%d ", (getTerrainType(x, y) == TERRAIN_TYPE_LAND));

			fwrite(&isLand, sizeof(char), 2, image);
		}

		fwrite("\n", sizeof(char), 1, image);
	}

	fclose(image);

	cout << "\r✅  Writting Land map (\"images/land_map.ppm\")" << endl;
}

void precipitationImageGeneration () {

	cout << "      Writting Precipitation map... ";

	// P2 format: Value from 0 to MAP_RAIN_RADIUS * 2

	char header[17];
	FILE * image = fopen ("images/rain_map.ppm", "wb");
	sprintf(header, "P2\n%04d %04d\n%03d\n", MAP_WIDTH, MAP_HEIGHT, MAP_PRECIPITATION_MAX);
	fwrite(&header, sizeof(char), sizeof(header), image);

	for (int y = 0; y < MAP_HEIGHT; y++)  {
		for (int x = 0; x < MAP_WIDTH; x++) {

			char precipitation [4];
			sprintf(precipitation, "%03d ", (getPrecipitation(x, y)));

			fwrite(&precipitation, sizeof(char), 4, image);
		}

		fwrite("\n", sizeof(char), 1, image);
	}

	fclose(image);

	cout << "\r✅  Writting Precipitation map (\"images/rain_map.ppm\")" << endl;
}

void temperatureImageGeneration() {

	cout << "      Writting Temperature map... ";

	// P3 format: rgb (255)

	char header[17];
	FILE * image = fopen ("images/temperature_map.ppm", "wb");
	sprintf(header, "P3\n%04d %04d\n255\n", MAP_WIDTH, MAP_HEIGHT);
	fwrite(&header, sizeof(char), sizeof(header), image);

	int temperaturerange = MAP_TEMPERATURE_MAX - MAP_TEMPERATURE_MIN;

	for (int y = 0; y < MAP_HEIGHT; y++)  {
		for (int x = 0; x < MAP_WIDTH; x++) {

			int r, g, b;

			int temperature = getTemperature(x, y);
			float templevel = (temperature - MAP_TEMPERATURE_MIN) / (float)temperaturerange;

			g = 0;
			b = 0;
			r = 170 * templevel;

			char level[12];
			sprintf(level, "%03d %03d %03d ", r, g, b);
			fwrite(&level, sizeof(char), sizeof(level), image);
		}

		fwrite("\n", sizeof(char), 1, image);
	}

	fclose(image);

	cout << "\r✅  Writting Temperature map (\"images/temperature_map.ppm\")" << endl;
}

void biomeImageGeneration () {

	cout << "      Writting Biomes map... ";

	// P3 format: rgb (255)

	char header[17];
	FILE * image = fopen ("images/biome_map.ppm", "wb");
	sprintf(header, "P3\n%04d %04d\n255\n", MAP_WIDTH, MAP_HEIGHT);
	fwrite(&header, sizeof(char), sizeof(header), image);

	for (int y = 0; y < MAP_HEIGHT; y++)  {
		for (int x = 0; x < MAP_WIDTH; x++) {

			int r, g, b;
			char biome = getBiome(x, y);

			switch (biome) {
				case BIOME_OCEAN:
					r = 13 ; g = 71 ; b = 161; break; // rgb(13 ,71 ,161)
				case BIOME_CORAL:
					r = 33 ; g = 150; b = 243; break; // rgb(33 ,150,243)
				case BIOME_GROVE:
					r = 103; g = 58 ; b = 183; break; // rgb(103,58 ,183)
				case BIOME_ARCTIC:
					r = 187; g = 222; b = 251; break; // rgb(187,222,251)
				case BIOME_SNOWMOUNTAINS:
					r = 121; g = 85 ; b = 72 ; break; // rgb(121,85 ,72 )
				case BIOME_SNOWPLAINS:
					r = 161; g = 136; b = 127; break; // rgb(161,136,127)
				case BIOME_FOREST:
					r = 46 ; g = 125; b = 50 ; break; // rgb(46 ,125,50 )
				case BIOME_RAINFOREST:
					r = 27 ; g = 94 ; b = 32 ; break; // rgb(27 ,94 ,32 )
				case BIOME_TAIGA:
					r = 0  ; g = 77 ; b = 64 ; break; // rgb(0  ,77 ,64 )
				case BIOME_WOODLAND:
					r = 67 ; g = 160; b = 71 ; break; // rgb(67 ,160,71 )
				case BIOME_PRAIRIE:
					r = 139; g = 195; b = 74 ; break; // rgb(139,195,74 )
				case BIOME_SAVANNA:
					r = 255; g = 167; b = 38 ; break; // rgb(255,167,38 )
				case BIOME_GRASSLAND :
					r = 205; g = 220; b = 57 ; break; // rgb(205,220,57 )
				case BIOME_TUNDRA:
					r = 144; g = 164; b = 174; break; // rgb(144,164,174)
				case BIOME_MOUNTAIN:
					r = 120; g = 144; b = 156; break; // rgb(120,144,156)
				case BIOME_CHAPARRAL:
					r = 192; g = 202; b = 51 ; break; // rgb(192,202,51 )
				case BIOME_MESA:
					r = 255; g = 160; b = 0  ; break; // rgb(255,160,0  )
				case BIOME_DESERT:
					r = 213; g = 0  ; b = 0  ; break; // rgb(213,0,0)
				default:
					r = 0; g = 0; b = 0; break;
			}

			char level[12];
			sprintf(level, "%03d %03d %03d ", r, g, b);
			fwrite(&level, sizeof(char), sizeof(level), image);
		}

		fwrite("\n", sizeof(char), 1, image);
	}

	fclose(image);

	cout << "\r✅  Writting Biomes map (\"images/biome_map.ppm\")" << endl;
}
