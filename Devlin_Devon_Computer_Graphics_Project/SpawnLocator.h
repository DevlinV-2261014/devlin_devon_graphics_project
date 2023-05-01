#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <ioStream>
using namespace std;

// Find a spot inside the requested space where there is no wall
glm::vec3 getSpawnLocation(vector<glm::vec3> mazeLayout, int minX, int maxX, int minZ, int maxZ);