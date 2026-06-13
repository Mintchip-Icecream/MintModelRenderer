#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm.hpp>
#include <vec3.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <chrono>
#include <stb/stb_image.h>
#include "GLSetup.h"
#include "Camera.h"
#include "Object.h"
#include "Model3D.h"


using namespace std;

float waitTime = 1.0 / 60; //update 60 times a second;
unsigned int shaderProgram;
GLuint texture;

std::string getThisDir() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string path(buffer);
	// get the solution dir
	path = path.substr(0, path.find_last_of("\\/"));
	path = path.substr(0, path.find_last_of("\\/"));
	path = path.substr(0, path.find_last_of("\\/"));
	return path + "\\";
}
// define 
class Properties {
public:
	const float waitTime = 1.0f / 60.0f; //update 60 times a second;
	const int width = 800;
	const int height = 600;
	float fovDegrees = 45.0f;
};

auto getNow() {
	return std::chrono::steady_clock::now();
}

void setUniformMatrix(const std::string& name, glm::mat4 matrixValue) {
	unsigned int uniLoc = glGetUniformLocation(shaderProgram, name.c_str());
	glUniformMatrix4fv(uniLoc, 1, GL_FALSE, glm::value_ptr(matrixValue));
}

void setUniformVec3(const std::string& name, glm::vec3 value) {
	unsigned int uniLoc = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform3fv(uniLoc, 1, glm::value_ptr(value));
}

void setUniformI(const std::string& name, int value) {
	GLuint tex0Uni = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform1i(tex0Uni, value);
}

Camera camera;


//std::vector<glm>
int main() {
	Properties consts;
	// initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//initialize window, handle errors
	GLFWwindow* window = glfwCreateWindow(consts.width, consts.height, "The Window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to Create Window:" << std::endl;
		glfwTerminate();
		return -1;
	}
	// set ctx to our window
	glfwMakeContextCurrent(window);

	//load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	gladLoadGL();

	// setup viewport, background color, etc before starting the loop
	glViewport(0, 0, consts.width, consts.height);
	glClearColor(0.07f, 0.13f, 0.13f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	int frameRate = 60;
	float waitTime = 1.0f / 60;

	//GLuint shaderProgram;
	shaderProgram = createShaderProgram("./Shaders/default.vert", "./Shaders/default.frag");
	glUseProgram(shaderProgram);
	std::cout << "Shader Load Success!" << std::endl;

	//setup the projection matrix frustrum
	glm::mat4 projection = glm::perspective(
		glm::radians(consts.fovDegrees),
		(float) consts.width / consts.height, // aspect ratio
		0.1f, // near
		1000.0f // far
	);

	//setup camera view position
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

	// assign projection into shader
	setUniformMatrix("projection", projection);
	// assign view position into shader
	setUniformMatrix("view", view);

	// set the glfw callback that resizes the viewport
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		float aspect = (float)width / height;
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
		setUniformMatrix("projection", projection);
	});

	// set callback function to change camera depending on key;
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		//if (action == GLFW_PRESS) {
			camera.handleKeyInput(key);
		//};
	});

	
	// set up depth buffer
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	//assign fragment shader uniforms
	setUniformVec3("defaultAmbient", glm::vec3(0.5f, 0.5f, 0.5f));
	setUniformVec3("defaultSpecular", glm::vec3(0.7f, 0.7f, 0.7f));
	setUniformVec3("lightColor", glm::vec3(0.9f, 0.9f, 0.9f));
	setUniformVec3("LightPos", glm::vec3(5, 20, -20));
	
	
	// different models
	//Model theModel(getThisDir() + "Resource Files\\Shih Tzu\\aSiz_b.nsbmd\\Siz_b.dae");
	Model theModel(getThisDir() + "Resource Files\\nintendog\\scene.gltf");
	//Model theModel(getThisDir() + "Resource Files\\Serena\\Serena_ColladaMax.DAE");
	//Model theModel(getThisDir() + "Resource Files\\Cloud\\Cloud.dae");
	//objects.push_back(model);


	auto lastTime = getNow();
	double timeAccumulator = 0.0;
	while (!glfwWindowShouldClose(window)) { // loop while window isn't closed

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// process input methods here

		// update state

		// ensure that our time to updaate state is ready
		auto currentTime = getNow();
		std::chrono::duration<double> deltaTime = currentTime - lastTime;
		timeAccumulator += deltaTime.count();

		//UPDATE HERE
		if (timeAccumulator >= waitTime) {
			//for (int i = 0; i < cubes.size(); i++) {
			//	cubes[i].update();
			//}
			theModel.update();
			camera.update();
			timeAccumulator = 0.0;
			//cout << "updated!" << endl;
		}

		// rendering here
		glUseProgram(shaderProgram);
		camera.draw(shaderProgram);
		theModel.draw(shaderProgram);
		//for (int i = 0; i < cubes.size(); i++) {
		//	cubes[i].draw(shaderProgram);
		//}


		//call events and swap buffers here
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	glDeleteTextures(1, &texture);
	return 0;
}

// extra stuff in project
float cubeVerts[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};
// draws a cube
class Cube : public Object {
public:
	float* vertices;
	unsigned int vbo;
	unsigned int vao;
	float currentRotation = 0.0f;
	float rotationPerFrame = -1.0f;
	float speed = 0.72f;
	float posx, posy, posz;
	Cube(float posX, float posY, float posZ) {
		posx = posX;
		posy = posY;
		posz = posZ;

		vertices = cubeVerts;

		///setup vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//setup the vertex buffer
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);//setup the vertex buffer

		glBufferData(GL_ARRAY_BUFFER, 36 * 5 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
	void update() {
		currentRotation += rotationPerFrame * waitTime * speed;
	}

	void draw(unsigned int shaderProgram) {
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(posx, posy, posz));

		model = glm::rotate(model, glm::radians(currentRotation),
			glm::vec3(0.5f + posx, 1.0f + posy, 0.0f));


		setUniformMatrix("model", model);

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
};
