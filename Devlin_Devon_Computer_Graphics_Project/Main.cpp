#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "FileReader.h"
#include "Shader.h"
#include "TextureHandler.h"
#include "MazeGen.h"

using namespace std;

// Function declaradions
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void mouseCalllback(GLFWwindow* window, double xPosition, double yPosition);
vector<glm::vec3> getLightPositions();

// Cube vertices
float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f
};

float skyboxVertices[] = {
	// Back
	-1.0f,  1.0f, -1.0f, // Top left
	-1.0f, -1.0f, -1.0f, // Bottom left
	 1.0f, -1.0f, -1.0f, // Bottom right
	 1.0f, -1.0f, -1.0f, // Bottom right
	 1.0f,  1.0f, -1.0f, // Top right
	-1.0f,  1.0f, -1.0f, // Top left

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

// Screen settings
const unsigned int SCREEN_WIDTH = 1000;
const unsigned int SCREEN_HEIGHT = 800;

// Camera settings
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool mouse = true;
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float cameraLastX = SCREEN_WIDTH / 2.0f;
float cameraLastY = SCREEN_HEIGHT / 2.0f;
float cameraFov = 45.0f;

// Timings
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	// generate random maze
	MazeGen(19, 31);

	// Read the maze file and create positions for each #
	vector<glm::vec3> cubeLocations = getMazeLayout("maze.txt");

	// Floor
	// Find the highest x and Z to see where the floor should end.
	int highestX = cubeLocations[0].x;
	int highestZ = cubeLocations[0].z;
	for (glm::vec3 location : cubeLocations) {
		if (location.x > highestX) {
			highestX = location.x;
		}
		if (location.z > highestZ) {
			highestZ = location.z;
		}
	}
	// Add a block for every part of the maze
	for (int i = 0; i <= highestX; i++) {
		for (int j = 0; j <= highestZ; j++) {
			cubeLocations.push_back(glm::vec3(i, -1, j));
		}
	}

	// Initialize the window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Devlin_Devon_project", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCalllback);
	glfwSetScrollCallback(window, scrollCallback);

	// Capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Enable depth test to create proper cubes
	glEnable(GL_DEPTH_TEST);

	// Shaders for the maze
	Shader mazeShader("walls.vs", "walls.fs");
	Shader skyboxShader("skybox.vs", "skybox.fs");
	Shader lightShader("light.vs", "light.fs");

	unsigned int cubeVAO;
	unsigned int cubeVBO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Lighting
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Wall textures
	unsigned int wallTexture = createTexture("wall.png");

	// LIGHTNING
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Skybox
	vector<std::string> skyboxFaces
	{
		"SkyBoxRight.png", "SkyBoxLeft.png", "SkyBoxTop.png", "SkyBoxBottom.png", "SkyBoxFront.png", "SkyBoxBack.png"
	};

	unsigned int skyboxVAO;
	unsigned int skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	unsigned int skyboxTexture = createCubemap(skyboxFaces);

	// Use the shaders
	mazeShader.use();
	mazeShader.setInt("wallTexture", 0);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// Rendering in loop
	while (!glfwWindowShouldClose(window))
	{
		// Change deltatime
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind textures
		mazeShader.use();
		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		mazeShader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(cameraFov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		mazeShader.setMat4("projection", projection);
		// Apply Light
		mazeShader.setVec3("objectColor", 0.19f, 0.34f, 0.30f);
		mazeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		glBindVertexArray(cubeVAO);

		vector<glm::vec3> lightPositions = getLightPositions();
		for (int i = 0; i < lightPositions.size(); i++) {
			mazeShader.setVec3("lights[" + to_string(i) + "].lightPosition", lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
			mazeShader.setFloat("lights[" + to_string(i) + "].constant", 1.0f);
			mazeShader.setFloat("lights[" + to_string(i) + "].linear", 0.14f);
			mazeShader.setFloat("lights[" + to_string(i) + "].quadratic", 0.07f);
		}

		// Draw Cubes
		for (glm::vec3 cube : cubeLocations)
		{
			mazeShader.setVec3("viewPosition", cameraPosition.x, cameraPosition.y, cameraPosition.z);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cube);
			mazeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Draw light
		/*lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		for (int i = 0; i < lightPositions.size(); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/


		// Draw Skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		view = glm::mat4(glm::mat3(glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp)));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process keybinds
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = static_cast<float>(2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraPosition += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraPosition -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		cout << cameraPosition.x << " X -> " << cameraPosition.z << " Z";
	}
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	cameraFov -= (float)yOffset;
	if (cameraFov < 1.0f) {
		cameraFov = 1.0f;
	}
	if (cameraFov > 45.0f) {
		cameraFov = 45.0f;
	}
}

void mouseCalllback(GLFWwindow* window, double xPosition, double yPosition) {
	float xPos = static_cast<float>(xPosition);
	float yPos = static_cast<float>(yPosition);

	if (mouse) {
		cameraLastX = xPos;
		cameraLastY = yPos;
		mouse = false;
	}

	float xOffset = xPos - cameraLastX;
	float yOffset = cameraLastY - yPos;

	cameraLastX = xPos;
	cameraLastY = yPos;

	float cameraSensitivy = 0.1f;
	xOffset *= cameraSensitivy;
	yOffset *= cameraSensitivy;

	cameraYaw += xOffset;

	// This enables the user to look up
	cameraPitch += yOffset;

	if (cameraPitch > 89.0f) {
		cameraPitch = 89.0f;
	}
	if (cameraPitch < -89.0f) {
		cameraPitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	front.y = sin(glm::radians(cameraPitch));
	front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	cameraFront = glm::normalize(front);
}

vector<glm::vec3> getLightPositions() {
	vector<glm::vec3> positions;
	positions.push_back(glm::vec3(2.0f, 1.0f, 2.0f));
	positions.push_back(glm::vec3(15.0f, 1.0f, 15.0f));
	positions.push_back(glm::vec3(7.0f, 1.0f, 25.0f));
	positions.push_back(glm::vec3(25.0f, 1.0f, 2.0f));

	/*positions.push_back(glm::vec3(5.0f, 3.0f, 15.0f));
	positions.push_back(glm::vec3(15.0f, 3.0f, 15.0f));
	positions.push_back(glm::vec3(25.0f, 3.0f, 15.0f));*/
	return positions;
}