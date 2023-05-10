#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"
#include <string>
using namespace std;

// init
string vertexCode, fragmentCode;
const char* vertexP;
const char* fragmentP;
const char* vShaderCode;
const char* fShaderCode;

unsigned int shader, vertex, fragment;
int success;
char infoLog[512];

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    vertexP = vertexPath;
    fragmentP = fragmentPath;

    readFile(vertexP, true);
    readFile(fragmentP, false);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // vertex Shader
    shaderCompile(vShaderCode, true);

    // fragment shader
    shaderCompile(fShaderCode, false);

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // exceptions happen, what failed?
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    // delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::readFile(string path, bool vertex) {
    // enable exceptions
    ifstream file;
    file.exceptions(ifstream::failbit | ifstream::badbit);
    try
    {
        file.open(path);
        stringstream stream;
        stream << file.rdbuf();
        file.close();
        if (vertex)
            vertexCode = stream.str();
        else
            fragmentCode = stream.str().c_str();
    }
    catch (ifstream::failure e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }
}

void Shader::shaderCompile(const char* code, bool vert)
{
    if (vert)
        shader = glCreateShader(GL_VERTEX_SHADER);
    else
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    // if compile errors were encountered, print them
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    };

    if (vert)
        vertex = shader;
    else
        fragment = shader;
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setMat4(const string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



