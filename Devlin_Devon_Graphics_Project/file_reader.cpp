#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string readMazeLayout(string fileName) {
	string layout;
	fstream mazeFile;

	mazeFile.open("../../../textfiles/" + fileName, ios::in);
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