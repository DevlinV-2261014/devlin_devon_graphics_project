#include "SpawnLocator.h"

// Find a spot inside the requested space where there is no wall
glm::vec3 getSpawnLocation(vector<glm::vec3> mazeLayout, int minX, int maxX, int minZ, int maxZ, float y) {
	vector<glm::vec3> possibleLocations;
	for (int i = 0; i < mazeLayout.size() - 1; i++) {
		if (mazeLayout[i].x >= minX && mazeLayout[i].x <= maxX) {
			if (mazeLayout[i].z >= minZ && mazeLayout[i].z <= maxZ) {
				// For every spot inbetween the walls on position i and i+1
				for (int j = mazeLayout[i].x + 1; j < mazeLayout[i + 1].x; j++) {
					// Put the position in possible locations.
					possibleLocations.push_back(glm::vec3((mazeLayout[i].x + (j - mazeLayout[i].x)), mazeLayout[i].y - y, mazeLayout[i].z));
				}
			}
		}
	}
	// Return a random position
	return possibleLocations[rand() % (possibleLocations.size() - 0 + 1) + 0];
}