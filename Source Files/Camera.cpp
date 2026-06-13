#include "Camera.h"
#include <glad.h>

// takes input and moves the camera around
void Camera::handleKeyInput(int key) {
	currentKey = key;
}

void Camera::update() {
	switch (currentKey) {
	case GLFW_KEY_W:
		z += waitTime * moveSpeed;
		break;
	case GLFW_KEY_S:
		z -= waitTime * moveSpeed;
		break;
	case GLFW_KEY_A:
		x += waitTime * moveSpeed;
		break;
	case GLFW_KEY_D:
		x -= waitTime * moveSpeed;
		break;
	case GLFW_KEY_SPACE:
		y += waitTime * moveSpeed;
		break;
	case GLFW_KEY_LEFT_SHIFT:
		y -= waitTime * moveSpeed;
		break;
	case GLFW_KEY_UP:
		roty += waitTime * rotateSpeed;
		break;
	case GLFW_KEY_DOWN:
		roty -= waitTime * rotateSpeed;
		break;
	case GLFW_KEY_LEFT:
		rotx += waitTime * rotateSpeed;
		break;
	case GLFW_KEY_RIGHT:
		rotx -= waitTime * rotateSpeed;
	default:
		break;
	}
	currentKey = 0;
}

void Camera::draw(unsigned int shaderProgram) {
	glm::vec3 pos = glm::vec3(x, y, z);
	glm::vec3 target = glm::vec3(x + rotx, y + roty, z + 1);
	glm::vec3 cameraDirection = glm::normalize(pos - target);

	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);


	glm::mat4 view;
	/*glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(x, y, z));*/
	view = glm::lookAt(pos, target, cameraUp);

	unsigned int uniLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniLoc, 1, GL_FALSE, glm::value_ptr(view));
}