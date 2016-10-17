#include <memory>
#include <cstdio>
#include <rethinkdb.h>

#include "../inc/map.h"

using namespace std;

namespace R = RethinkDB;

void saveToDatabase () {

	unique_ptr<R::Connection> conn = R::connect("localhost", 28015);

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			int temperature = getTemperature(x,y);
			int altitude = getAltitude(x, y);
			int precipitation = getPrecipitation(x,y);

			char terrain = getTerrainType(x,y);
			char biome = getBiome(x,y);

			string json =
				"{'id': "             + to_string(getWrappedIndex(x, y)) +
				", 'temperature' :"   + to_string(temperature) +
				", 'altitude' :"      + to_string(altitude) +
				", 'precipitation' :" + to_string(precipitation) +
				", 'terrain' :"       + to_string(terrain) +
				", 'biome' :"         + to_string(biome) +
				"}";
				
			R::Cursor cursor = R::table("map").insert(json).run(*conn);
		}
	}
}
