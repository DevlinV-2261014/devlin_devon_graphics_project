#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

string readMazeLayout(string fileName) {
	string layout;
	fstream mazeFile;

	mazeFile.open(fileName, ios::in);
	if (mazeFile.is_open()) {
		cout << "reading file: " << fileName << endl;
		while (getline(mazeFile, layout)) {
			cout << layout << endl;
		}
		mazeFile.close();
	}
	else {
		cout << "Unable to read file: " << fileName << endl;
	}

	return layout;
}

std::vector<glm::vec3> getMazeLayout(std::string fileName) {
	string layout;
	vector<glm::vec3> vertices;
	fstream mazeFile;

	mazeFile.open(fileName, ios::in);
	if (mazeFile.is_open()) {
		cout << "reading file: " << fileName << endl;
		int row = 0;
		while (getline(mazeFile, layout)) {
			for (int i = 0; i < layout.size(); i++) {
				if (layout[i] == '#') {
					vertices.push_back(glm::vec3(i, 0, row));
				}
			}
			row++;
			layout.clear();
		}
		mazeFile.close();
	}
	else {
		cout << "Unable to read file: " << fileName << endl;
	}

	return vertices;
}