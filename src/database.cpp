#include <memory>
#include <cstdio>
#include <rethinkdb.h>

#include "../inc/map.h"

using namespace std;

// RethinkDB database must be running and accessible
namespace R = RethinkDB;

// RethinkDB access port
const int RETHINKDB_PORT = 28015;

// Create and populate data and map tables on rethinkDB database
void saveToDatabase () {

	cout << " - DATABASE CONSTRUCTION" << endl;
	cout << "    size: " << (MAP_WIDTH * MAP_HEIGHT) << " documents" << endl;

	// Connecting to database
	unique_ptr<RethinkDB::Connection> conn = RethinkDB::connect("localhost", RETHINKDB_PORT);

	// Tables creation
	cout << "    Creating rethinkdb tables... " << flush;

	R::db("kingdom").table_create("map").run(*conn);
	R::db("kingdom").table_create("data").run(*conn);

	cout << "\r✅  Creating rethinkdb tables    " << endl;

	// Data saving (map infos)
	cout << "    Writting data to rethinkdb database... " << flush;

	string data ("{ \"id\": 0, \"height\": " + to_string(MAP_HEIGHT) + ", \"width\": " + to_string(MAP_WIDTH) + " } ");
	R::db("kingdom").table("data").insert(R::json(data)).run(*conn);

	cout << "\r✅  Writting data to rethinkdb database    " << endl;

	// Map saving
	cout << "    Writting map to rethinkdb database... " << flush;

	// Tiles are saved row by row
	for (int y = 0; y < MAP_HEIGHT; y++) {

		// Row tiles are appended to json string
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

		// Saving row to database
		R::db("kingdom").table("map").insert(R::json(json)).run(*conn);

		cout << "\r    Writting map to rethinkdb database... (" << y << "/" << MAP_HEIGHT <<")" << flush;

	}

	cout << "\r✅  Writting map to rethinkdb database (" << MAP_HEIGHT << "/" << MAP_HEIGHT <<")      " << endl;
}
