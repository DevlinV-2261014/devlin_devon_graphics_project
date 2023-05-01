#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

class Model
{
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh>    meshes;
	std::string directory;
	bool gammaCorrection;

	Model(std::string path);
	void Draw(Shader& shader);

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	std::vector<Texture> loadTexture(aiMaterial *mat, aiTextureType type, std::string typeName);
};