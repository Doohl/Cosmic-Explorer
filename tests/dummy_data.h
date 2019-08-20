#pragma once

#include "json.hpp"

using json = nlohmann::json;

// Contains dummy data for use in testing
namespace DummyData {

	// A simple dummy solar system
	auto Sol = R"(
	{
	  "name": "Sol",
	  "mass": 1.98855e+30,
	  "radius": 695700,
	  "children": [
		{
			"name": "Mercury"
		},
		{
			"name": "Venus"
		},
		{
			"name": "Earth",
			"radius": 6371.0,
			"mass": 5.97237e+24,
			"orbit": {
				"semimajorAxis": 149598023,
				"eccentricity": 0.0167086,
				"epochTime": 0,
				"epochAnomaly": 6.259047404,
				"lAscending": -0.196535244,
				"aPeriapsis": 1.99330266505
			},
			"children": [
			{
				"name": "Luna",
				"radius": 1737.1,
				"mass": 7.342e+22,
				"orbit": {
					"semimajorAxis": 384399,
					"eccentricity": 0.0549,
					"epochTime": 0,
					"epochAnomaly": 2.214976985708,
					"lAscending": 2.18305783,
					"aPeriapsis": 5.55276502
				}
			}
			]
		},
		{
			"name": "Mars"
		}
		]
	}
)"_json;
}