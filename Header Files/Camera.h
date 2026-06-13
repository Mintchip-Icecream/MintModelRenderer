#pragma once
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "Object.h"
#include <vec3.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera : public Object {
public:
	void handleKeyInput(int key);
	void update() override;
	void draw(unsigned int shaderProgram) override;
private:
	float moveSpeed = 10.0f;
	float rotateSpeed = 2.0f;
	float x = 0.0f;
	float y = -0.0f;
	float z = -10.0f;
	int currentKey = 0;
	float rotx = 0.0f;
	float roty = 0.0f;
	float waitTime = (float)1.0f / 60.0f;
};