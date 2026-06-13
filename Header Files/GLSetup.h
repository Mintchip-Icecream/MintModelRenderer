#pragma once
#include <string>
#include <glad/glad.h>

std::string loadShaderSource(const std::string& filepath);

GLuint compileShader(GLenum type, const std::string& source);

GLuint createShaderProgram(const std::string& vertPath, const std::string& fragPath);