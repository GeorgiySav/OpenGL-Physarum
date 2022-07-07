#pragma once
#include "glad/glad.h"
#include <glfw3.h>
#include <string>
#include <fstream>
#include <iostream>

std::string readFromFile(const char* filepath);

class ShaderVF
{
private:
	GLuint programID;
public:
	ShaderVF(const char* vertexFP, const char* fragmentFP);
	~ShaderVF() { glDeleteProgram(programID); }

	void execute() { glUseProgram(programID); }

	GLuint getID() { return programID; }

};

class ComputeShader
{
private:
	GLuint programID;
	GLuint computeShader;
public:
	ComputeShader(const char* filepath);
	~ComputeShader() { glDeleteProgram(programID); glDeleteShader(computeShader); }

	void dispatch(GLuint x, GLuint y, GLuint z);

	GLuint getID() { return programID; }
};