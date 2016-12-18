
#include <iostream>

#ifndef MAP_H
#define MAP_Hint

// Map Tile Size
const int MAP_WIDTH                 = 1024;
const int MAP_HEIGHT                = MAP_WIDTH;

// Minimal Trenches deapth and Maximal summit heights
const int MAP_ELEVATION_MIN         = 0;
const int MAP_ELEVATION_MAX         = 250;

// How diverse can the relief be
const int MAP_GRANULARITY           = MAP_WIDTH / 16;
// How chaotic will terrain generate
const int MAP_SCALE                 = MAP_ELEVATION_MAX / 10;

// Temperatures (biomes will scale to given range)
const int MAP_TEMPERATURE_MIN       = -30;
const int MAP_TEMPERATURE_MAX       =  50;

// How important is each factor to determine tile temperature ( 0 < .. < 1 )
const float TEMPERATURE_ALTITUDE_SIGNIFICANCE = 0.3;
const float TEMPERATURE_LATITUDE_SIGNIFICANCE = 0.7;

// Sea level in map elevation range
const int MAP_SEA_LEVEL             = 130;
// How often are rivers able to spawn
const int MAP_RIVER_FREQUENCY       = MAP_WIDTH / 16;
// At which altitude can a source appear
const int MAP_RIVER_MIN_ELEVATION   = 200;

// Radius of the effect of water tiles
const int MAP_RAIN_RADIUS           = MAP_WIDTH / 100;
// Size of blocks checking for water quantity
const int MAP_RAIN_CHECK_RANGE      = MAP_WIDTH / 512;
// Max value of precipitation (range: 0 - MAP_PRECIPITATION_MAX)
const int MAP_PRECIPITATION_MAX     = 100;


// const int TEMPARATURE_LEVEL_RANGE = (MAP_TEMPERATURE_MAX - MAP_TEMPERATURE_MIN) / 4;
// const int TEMPERATURE_LEVEL_LOW  = MAP_TEMPERATURE_MIN + TEMPARATURE_LEVEL_RANGE;
// const int TEMPERATURE_LEVEL_HIGH = MAP_TEMPERATURE_MAX - TEMPARATURE_LEVEL_RANGE;
// const int PRECIPITATION_LEVEL_RANGE = MAP_PRECIPITATION_MAX / 3;
// const int PRECIPITATION_LEVEL_LOW  = PRECIPITATION_LEVEL_RANGE;
// const int PRECIPITATION_LEVEL_HIGH = MAP_PRECIPITATION_MAX - PRECIPITATION_LEVEL_RANGE;
const int PRECIPITATION_LEVEL_LOW  = 15;
const int PRECIPITATION_LEVEL_HIGH = 30;
const int TEMPERATURE_LEVEL_LOW  = 0;
const int TEMPERATURE_LEVEL_HIGH = 30;


// Minimal height for mountains
const int MAP_MOUNTAIN_HEIGHT       = 180;
// Minimal deapth for oceans
const int MAP_OCEAN_DEAPTH          = 10;


const int TERRAIN_TYPE_OCEAN   = 0;
const int TERRAIN_TYPE_LAND    = 1;
const int TERRAIN_TYPE_RIVER   = 2;
const int TERRAIN_TYPE_COAST   = 3;
const int TERRAIN_TYPE_LAKE    = 4;


//                l a n d
// .::.         B I O M E S
//  ++  # # # # # # # # # # # # # #
//      #   M   |         |       #
//  +   #   S   |    T    |   R   #
//  r   #-------------------------#
//  a   #       |    F    |   P   #
//  i   #   I   |    W    |   A   #
//  n   #-------------------------#
//  -   #       |    O    |   C   #
//      #   U   |    G    |   D   #
// _.__ # # # # # # # # # # # # # #
//        * .*               ++ +
//              - t e m p +

const int BIOME_SNOWMOUNTAINS       = 10;
const int BIOME_SNOWPLAINS          = 11;

const int BIOME_FOREST              = 20;

const int BIOME_RAINFOREST          = 30;


const int BIOME_TAIGA               = 14;

const int BIOME_WOODLAND            = 24;
const int BIOME_PRAIRIE             = 25;

const int BIOME_SAVANNA             = 34;
const int BIOME_GRASSLAND           = 35;


const int BIOME_TUNDRA              = 18;

const int BIOME_MOUNTAIN            = 28;
const int BIOME_CHAPARRAL           = 29;

const int BIOME_MESA                = 38;
const int BIOME_DESERT              = 39;

//               o c e a n
//              B I O M E S
//  d   # # # # # # # # # # # # # #
//  e   #       |        |        #
//  a   #   Z   |    V   |   K    #
//  p   #       |        |        #
//  t   #-------------------------#
//  h   #            E            #
//      # # # # # # # # # # # # # #
//        * .*               ++ +
//              - t e m p +

const int BIOME_OCEAN  = 0;
const int BIOME_CORAL  = 1;
const int BIOME_GROVE  = 2;
const int BIOME_ARCTIC = 3;

// Map access function

int  getWrappedIndex (int x, int y);

int  getAltitude     (int x, int y);
void setAltitude     (int x, int y, int altitude);

int  getTemperature  (int x, int y);
void setTemperature  (int x, int y, int temp);

int getTerrainType   (int x, int y);
void setTerrainType  (int x, int y, int terrain);

int getBiome         (int x, int y);
void setBiome        (int x, int y, int biome);
int getBiomeType     (int terrain, int temperature, int precipitation, int altitude);

int  getPrecipitation     (int x, int y);
void setPrecipitation     (int x, int y, int precipitation);
void increasePrecipitaion (int x, int y, int radius);

// Map generation

void mapGeneration (int seed);
void imagesGeneration ();

void altitudeGeneration (int seed);
void squareSet  (int x, int y, int size, int altitude);
void diamondSet (int x, int y, int size, int altitude);

void riverGeneration (int seed);
void riverFrom (int x, int y, int riverLength);

void coastlinesGeneration ();
void precipitationGeneration ();
void precipitationCalibration();

void temperatureGeneration ();
void temperatureCalibration ();

void biomeGeneration ();

// Images representation generation

void elevationImageGeneration ();
void landmapImageGeneration ();
void precipitationImageGeneration ();
void temperatureImageGeneration ();
void biomeImageGeneration ();

// Database

void saveToDatabase();

// Misc

void consoleHeader();

#endif
