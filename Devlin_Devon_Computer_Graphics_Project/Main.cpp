#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <irrKlang/irrKlang.h>

#include <string>
#include <vector>

#include "FileReader.h"
#include "Shader.h"
#include "TextureHandler.h"
#include "MazeGen.h"
#include "Model.h"
#include "SpawnLocator.h"

using namespace std;

// Function declarations
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void mouseCalllback(GLFWwindow* window, double xPosition, double yPosition);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
vector<glm::vec3> getLightPositions(vector<glm::vec3> mazeLayout, int highestX, int highestZ);
void setLightPositionsForShader(vector<glm::vec3> lightPositions, Shader shader);
bool checkCollisions(Model& model, vector<glm::vec3> vertices);
void instancing(unsigned int VAO, unsigned int texture, int size, bool floor);
bool minMaxCheck(float min, float max);

// Cube vertices
float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
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
glm::vec3 cameraPosition;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool mouse = true;
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float cameraLastX = SCREEN_WIDTH / 2.0f;
float cameraLastY = SCREEN_HEIGHT / 2.0f;
float cameraFov = 45.0f;

// boat init
glm::vec3 boatSpawn;
Model boat(boatSpawn);

// Timings
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool enableFlight{ false };
bool jumping{ false };
bool jumpEnd{ false };
float jumpHeight{ 1.0f };

// flashlight stuff
glm::vec3 flashLightSpawn;
bool playerHasFlashlight{ false };
bool flashLightOn{ false };

vector<glm::vec3> lightPositions;
vector<glm::vec3> wallCubeLocations;
vector<glm::vec3> cubeLocations;

int mazeSize;

int main() {
	// Create a sound engine
	irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();

	if (!soundEngine) {
		return -1;
	}
	// Play a song
	soundEngine->play2D("The_Dark_Maze.mp3", true);

	// generate random maze
	MazeGen(19, 31);

	// Read the maze file and create positions for each #
	cubeLocations = getMazeLayout("maze.txt");

	// calculate size of the actual maze to use later on
	mazeSize = cubeLocations.size();
	
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

	// calculate floor size
	int floorSize = cubeLocations.size() - mazeSize;

	// get locations of the walls
	for (int i = 0; i < mazeSize; i++)
	{
		wallCubeLocations.push_back(cubeLocations[i]);
	}

	cameraPosition = getSpawnLocation(cubeLocations, 0, 2, 0, highestZ, 0.0f);

	// Get random light positions
	lightPositions = getLightPositions(cubeLocations, highestX, highestZ);

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
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

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
	Shader floorShader("floor.vs", "floor.fs");
	Shader flashLightShader("flashlight.vs", "flashlight.fs");
	Shader boatShader("boat.vs", "boat.fs");
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Lighting
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Textures
	unsigned int wallTexture = createTexture("wall.png");
	unsigned int floorTexture = createTexture("water.jpg");
	unsigned int flashLightTexture = createTexture("wall.png");
	unsigned int boatTexture = createTexture("wood2.jpg");

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

	flashLightShader.use();
	flashLightShader.setInt("flashlightShader", 0);

	boatShader.use();
	boatShader.setInt("boatShader", 0);

	floorShader.use();
	floorShader.setInt("floorTexture", 0);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// Position the flashlight close to the player
	int minX = cameraPosition.x - 3;
	int maxX = cameraPosition.x + 3;
	int minZ = cameraPosition.z - 3;
	int maxZ = cameraPosition.z + 3;

	flashLightSpawn = getSpawnLocation(cubeLocations, minX, maxX, minZ, maxZ, 0.4f);
	boatSpawn = getSpawnLocation(cubeLocations, minX, maxX, minZ, maxZ, 0.4f);

	// Load models
	Model flashLight(flashLightSpawn);
	flashLight.loadModel("Linterna.obj");
	Model boat(boatSpawn);
	boat.loadModel("boat.obj");

	// Rendering loop
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

		// Enable jumping
		float cameraSpeed = static_cast<float>(2.5 * deltaTime);
		if (jumping) {
			// See if we are still in jumping range
			if (cameraPosition.y < jumpHeight) {
				boatSpawn += cameraSpeed * cameraUp;
			}
			else {
				// Jump top reached, start the descend
				jumping = false;
				jumpEnd = true;
			}
		}

		// If we reached the jump height and are descending
		if (jumpEnd) {
			// Check if we have not landed yet
			if (cameraPosition.y > 0.25f) {
				boatSpawn -= cameraSpeed * cameraUp;
			}
			else {
				// We landed, end the jump program.
				jumpEnd = false;
			}
		}

		// Bind textures
		mazeShader.use();
		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		mazeShader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(cameraFov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		mazeShader.setMat4("projection", projection);

		// Apply Light
		mazeShader.setVec3("objectColor", 0.19f, 0.34f, 0.30f);
		mazeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		mazeShader.setBool("flashLightOn", flashLightOn);
		if (flashLightOn) {
			mazeShader.setVec3("flashLight.lightColor", 0.0f, 1.0f, 1.0f);
			mazeShader.setFloat("flashLight.constant", 1.0f);
			mazeShader.setFloat("flashLight.linear", 0.35f);
			mazeShader.setFloat("flashLight.quadratic", 0.44f);
			mazeShader.setVec3("flashLight.lightPosition", cameraPosition.x, cameraPosition.y, cameraPosition.z);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		glBindVertexArray(cubeVAO);

		// Set light sources in the shader
		setLightPositionsForShader(lightPositions, mazeShader);

		// INSTANCING
		// generate maze instancing
		instancing(cubeVAO, wallTexture, mazeSize, false);

		// generate floor instancing
		instancing(cubeVAO, floorTexture, floorSize, true);

		// FLASH LIGHT
		if (!playerHasFlashlight) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, flashLightTexture);
			glBindVertexArray(cubeVAO);
			flashLightShader.use();
			setLightPositionsForShader(lightPositions, flashLightShader);
			flashLightShader.setVec3("objectColor", 0.5f, 0.5f, 0.5f);
			flashLightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			flashLightShader.setMat4("view", view);
			flashLightShader.setMat4("projection", projection);
			flashLightShader.setVec3("viewPosition", cameraPosition.x, cameraPosition.y, cameraPosition.z);
			glm::mat4 flModel = glm::mat4(1.0f);
			flModel = glm::translate(flModel, flashLight.position);
			flModel = glm::scale(flModel, glm::vec3(0.1f, 0.1f, 0.1f));
			flashLightShader.setMat4("model", flModel);
			flashLight.Draw(flashLightShader);
		}

		// boat
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boatTexture);
		glBindVertexArray(cubeVAO);
		boatShader.use();
		setLightPositionsForShader(lightPositions, boatShader);
		boatShader.setVec3("objectColor", 0.5f, 0.5f, 0.5f);
		boatShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		boatShader.setMat4("view", view);
		boatShader.setMat4("projection", projection);
		boatShader.setVec3("viewPosition", cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glm::mat4 boatModel = glm::mat4(1.0f);

		// collision detection
		bool coll = checkCollisions(boat, wallCubeLocations);
		if (coll == false)
			boat.position = boatSpawn;
		else
			boatSpawn = boat.position;
		// set camera location fixed to boat
		if (!enableFlight) {
			cameraPosition = boat.position + glm::vec3(-0.25f, 0.5f, 0);
		}

		boatModel = glm::translate(boatModel, boat.position);

		// if you scale the object, make sure to set the object size to the same factor
		int scaleFactor = 10;
		boatModel = glm::scale(boatModel, glm::vec3(1.0f / scaleFactor, 1.0f / scaleFactor, 1.0f / scaleFactor));
		boatShader.setMat4("model", boatModel);
		boat.Draw(boatShader);

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
	// Delete buffers
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	
	// Close the sound engine
	soundEngine->drop();

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

	float cameraSpeed = static_cast<float>(1.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		// If you enabled flying
		if (enableFlight) {
			cameraPosition += cameraSpeed * cameraFront;
		}
		else {
			// Only change X and Z to prevent flying
			boatSpawn.x += cameraSpeed * cameraFront.x;
			boatSpawn.z += cameraSpeed * cameraFront.z;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		// If you enabled flying
		if (enableFlight) {
			cameraPosition -= cameraSpeed * cameraFront;
		}
		else {
			// Only change X and Z to prevent flying
			boatSpawn.x -= cameraSpeed * cameraFront.x;
			boatSpawn.z -= cameraSpeed * cameraFront.z;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		boatSpawn -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		boatSpawn += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		// The old switcheroo (if you press N and C together)
		enableFlight = !enableFlight;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		// Check if the player is not already jumping or comming down
		if (!jumping && !jumpEnd)
			jumping = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (playerHasFlashlight) {
			// Switch the light on / off
			flashLightOn = !flashLightOn;
		}
	}
	// I just added this to see my coordinates
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
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

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// Get the x and y position of your mouse on the screen
		double xPosition;
		double yPosition;
		glfwGetCursorPos(window, &xPosition, &yPosition);

		// Set projection and view
		glm::mat4 projection = glm::perspective(glm::radians(cameraFov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

		// Stuff for the ray
		glm::vec3 ray_origin = cameraPosition; // Where is the ray comming from in the 3D world?
		glm::vec2 screen_pos = glm::vec2(xPosition, yPosition); // What is the position of the mouse on the screen?
		glm::vec4 view_pos = glm::vec4(screen_pos.x / SCREEN_WIDTH * 2.0 - 1.0, screen_pos.y / SCREEN_HEIGHT * 2.0 - 1.0, 0.0, 1.0); // What is the position of the view
		glm::mat4 inv_projection = glm::inverse(projection); // Projection
		glm::mat4 inv_view = glm::inverse(view);
		glm::vec4 world_pos = inv_view * (inv_projection * view_pos / (inv_projection * view_pos).w); // Position of the view inside the 3D world

		//world_pos /= world_pos.w;
		world_pos = inv_view * world_pos;

		// Cast the ray to the direction of the cameraFront
		glm::vec3 ray_direction = glm::normalize(cameraFront);

		// Set at what locations the object is spawned
		glm::vec3 box_min = flashLightSpawn - 0.5f;
		glm::vec3 box_max = flashLightSpawn + 0.5f;

		// Set the min and max x for the object's box
		float x_min, x_max, y_min, y_max, z_min, z_max;
		x_min = (box_min.x - ray_origin.x) / ray_direction.x;
		x_max = (box_max.x - ray_origin.x) / ray_direction.x;

		// Set the min and max y for the object's box
		y_min = (box_min.y - ray_origin.y) / ray_direction.y;
		y_max = (box_max.y - ray_origin.y) / ray_direction.y;

		// Set the min and max z for the object's box
		z_min = (box_min.z - ray_origin.z) / ray_direction.z;
		z_max = (box_max.z - ray_origin.z) / ray_direction.z;

		// If min > max, switch them
		if (minMaxCheck(x_min, x_max))
			std::swap(x_min, x_max);	

		// if min > max, switch
		if (minMaxCheck(y_min, y_max))
			std::swap(y_min, y_max);

		// This is needed to you can also click it from the above
		if (minMaxCheck(y_min,x_min))
			x_min = y_min;

		if (minMaxCheck(y_max, x_max))
			x_max = y_max;
		
		// if min > max, switch
		if (minMaxCheck(z_min, z_max))
			std::swap(z_min, z_max);

		// This is needed to you can also click it from the side
		if (minMaxCheck(z_min, x_min))
			x_min = z_min;

		if (minMaxCheck(z_max, x_max))
			x_max = z_max;

		// Get an intersection point
		glm::vec3 intersection_point = ray_origin + ray_direction * x_min;

		// If the intersactionpoint touches something inbetween the box
		if (intersection_point.x >= box_min.x
			&& intersection_point.x <= box_max.x
			&& intersection_point.y >= box_min.y
			&& intersection_point.y <= box_max.y
			&& intersection_point.z >= box_min.z
			&& intersection_point.z <= box_max.z) {
			playerHasFlashlight = true;
		}
	}
}

vector<glm::vec3> getLightPositions(vector<glm::vec3> mazeLayout, int highestX, int highestZ) {
	vector<glm::vec3> positions;
	for (int i = 0; i < 10; i++) {
		positions.push_back(getSpawnLocation(mazeLayout, 5, highestX - 5, 5, highestZ - 5, -1.0f)); // y = -1 because position is y of a block - (-1). This will make it One
	}

	return positions;
}

void setLightPositionsForShader(vector<glm::vec3> lightPositions, Shader shader) {
	shader.use();
	for (int i = 0; i < lightPositions.size(); i++) {
		shader.setVec3("lights[" + to_string(i) + "].lightPosition", lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
		shader.setFloat("lights[" + to_string(i) + "].constant", 1.0f);
		shader.setFloat("lights[" + to_string(i) + "].linear", 0.045f);
		shader.setFloat("lights[" + to_string(i) + "].quadratic", 0.0075f);
	}
}

bool checkCollisions(Model& model, vector<glm::vec3> vertices)
{
	bool axisXCollision;
	bool axisZCollision;
	int xScale = 10;
	int zScale = 7.5;

	for (int i = 0; i < vertices.size(); i++)
	{
		axisXCollision = false;
		axisZCollision = false;

		glm::vec3 cube = vertices[i];
		// x axis
		if (boatSpawn.x - model.size.x / xScale <= cube.x && cube.x - 1 <= boatSpawn.x || boatSpawn.x + model.size.x / xScale <= cube.x && cube.x + 1 <= boatSpawn.x || boatSpawn.x - model.size.x / xScale >= cube.x && cube.x - 1 >= boatSpawn.x || boatSpawn.x + model.size.x / xScale >= cube.x && cube.x + 1 >= boatSpawn.x)
			axisXCollision = true;
		// z axis
		if (boatSpawn.z - model.size.z / zScale <= cube.z && cube.z - 1 <= boatSpawn.z || boatSpawn.z - model.size.z / zScale >= cube.z && cube.z - 1 >= boatSpawn.z || boatSpawn.z + model.size.z / zScale >= cube.z && cube.z + 1 >= boatSpawn.z || boatSpawn.z + model.size.z / zScale <= cube.z && cube.z + 1 <= boatSpawn.z)
			axisZCollision = true;

		if (axisXCollision && axisZCollision)
			return true;
	}
	return false;
}

void instancing(unsigned int VAO, unsigned int texture, int size, bool floor) {
	// transformation matrices
	glm::mat4* modelMatsMaze = new glm::mat4[size];
	for (int i = 0; i < size; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		if (floor)
			model = glm::translate(model, cubeLocations[mazeSize + i]);
		else
			model = glm::translate(model, cubeLocations[i]);
		modelMatsMaze[i] = model;
	}

	// configure vertex buffer object (for both floor and maze)
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::mat4), &modelMatsMaze[0], GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < size; i++)
	{
		// vertex characteristics
		std::size_t vec4Size = sizeof(glm::vec4);
		for (int j = 0; j < 4; j++) {
			glEnableVertexAttribArray(3 + j);
			glVertexAttribPointer(3 + j, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(j*vec4Size));
			glVertexAttribDivisor(3 + j, 1);
		}
		glBindVertexArray(0);
	}

	// draw instanced cubes
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, size);
}

bool minMaxCheck(float min, float max) {
	if (min > max)
		return true;
	else
		return false;
}