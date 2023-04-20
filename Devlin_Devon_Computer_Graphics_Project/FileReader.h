#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
std::string readMazeLayout(std::string fileName);
std::vector<glm::vec3> getMazeLayout(std::string fileName);