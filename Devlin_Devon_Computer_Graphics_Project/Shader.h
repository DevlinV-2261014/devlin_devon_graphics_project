#pragma once
#include <string>
#include <glm/glm.hpp>
using namespace std;

class Shader {
public:
	// program id
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	void readFile(string path, bool vertex);
	void shaderCompile(const char* code, bool vert);
};
