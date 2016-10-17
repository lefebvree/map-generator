//
//    _  _     __    P  R  O  J  E  C  T       _
//   FJ / ;    LJ    _ ___       ___ _      ___FJ      ____      _ _____
//   J |/ (|         J '__ J     F __` L    F __  L    F __ J    J '_  _ `,
//  |     L    FJ   | |__| |   | |--| |   | |--| |   | |--| |   | |_||_| |
//  F L:\  L  J  L  F L  J J   F L__J J   F L__J J   F L__J J   F L LJ J J
//  J__L \\__L J__L J__L  J__L  )-____  L J\____,__L J\______/F J__L LJ J__L
// |__L  \L_| |__| |__L  J__| J\______/F  J____,__F  J______F  |__L LJ J__|
// 						      J______/
//
//   C ++   h e a d e r   f i l e

#include <iostream>

#ifndef MAP_H
#define MAP_H

// Map Tile Size
const int MAP_WIDTH                 = 4096;
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
const int MAP_RAIN_RADIUS           = MAP_WIDTH / 64;
// Size of blocks checking for water quantity
const int MAP_RAIN_CHECK_RANGE      = MAP_WIDTH / 256;
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


const char TERRAIN_TYPE_LAND    = 'l';
const char TERRAIN_TYPE_COAST   = 'c';
const char TERRAIN_TYPE_RIVER   = 'r';
const char TERRAIN_TYPE_LAKE    = 'k';
const char TERRAIN_TYPE_OCEAN   = 'o';


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

const char BIOME_SNOWMOUNTAINS       = 'M';
const char BIOME_SNOWPLAINS          = 'S';

const char BIOME_FOREST              = 'T';

const char BIOME_RAINFOREST          = 'R';


const char BIOME_TAIGA               = 'I';

const char BIOME_WOODLAND            = 'F';
const char BIOME_PRAIRIE             = 'W';

const char BIOME_SAVANNA             = 'P';
const char BIOME_GRASSLAND           = 'A';


const char BIOME_TUNDRA              = 'U';

const char BIOME_MOUNTAIN            = 'O';
const char BIOME_CHAPARRAL           = 'G';

const char BIOME_MESA                = 'C';
const char BIOME_DESERT              = 'D';

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

const char BIOME_CORAL  = 'K';
const char BIOME_GROVE  = 'V';
const char BIOME_OCEAN  = 'E';
const char BIOME_ARCTIC = 'Z';

// Map access function

int  getWrappedIndex (int x, int y);

int  getAltitude     (int x, int y);
void setAltitude     (int x, int y, int altitude);

int  getTemperature  (int x, int y);
void setTemperature  (int x, int y, int temp);

char getTerrainType  (int x, int y);
void setTerrainType  (int x, int y, char terrain);

char getBiome        (int x, int y);
void setBiome        (int x, int y, char biome);
char getBiomeType    (char terrain, int temperature, int precipitation, int altitude);

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
