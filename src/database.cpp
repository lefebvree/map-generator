#include <memory>
#include <cstdio>
#include <rethinkdb.h>

#include "../inc/map.h"

using namespace std;

namespace R = RethinkDB;

void saveToDatabase () {

	cout << "    Writting data to rethinkdb database... " << flush;

	unique_ptr<RethinkDB::Connection> conn = RethinkDB::connect("localhost", 28015);

	for (int y = 0; y < MAP_HEIGHT; y++) {

		string json("[{ \"id\": " + to_string(getWrappedIndex(0, y)) +
		", \"temperature\": "    + to_string(getTemperature(0,y)) +
		", \"altitude\": "       + to_string(getAltitude(0, y)) +
		", \"precipitation\": "  + to_string(getPrecipitation(0,y)) +
		", \"terrain\": "        + to_string(getTerrainType(0,y)) +
		", \"biome\": "          + to_string(getBiome(0,y)) +
		" }");

		for (int x = 1; x < MAP_WIDTH; x++) {
			json.append(", { \"id\": " + to_string(getWrappedIndex(x, y)) +
			", \"temperature\": "      + to_string(getTemperature(x,y)) +
			", \"altitude\": "         + to_string(getAltitude(x, y)) +
			", \"precipitation\": "    + to_string(getPrecipitation(x,y)) +
			", \"terrain\": "          + to_string(getTerrainType(x,y)) +
			", \"biome\": "            + to_string(getBiome(x,y)) +
			" }");
		}
		json.append("]");

		R::db("kingdom").table("map").insert(R::json(json)).run(*conn);

		cout << "    Writting data to rethinkdb database... (" << y << "/" << MAP_HEIGHT <<")" << flush;

	}

	cout << "\r✅  Writting data to rethinkdb database      " << endl;
}
