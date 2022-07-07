#pragma once
#include "glad/glad.h"
#include "glfw3.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <random>
#include <iostream>

struct S
{
	float speed;
	float sDis;
	float sAngle;
	int sRad;
	float tAngle;


	float r;
	float g;		//sending a vec4 only sends the r and a channel
	float b;		//b and g has no value
	float a = 1;

	S(float s, float sD, float sA, GLuint sR, float tA, glm::vec4 c)
	{
		speed = s;
		sDis = sD;
		sAngle = sA;
		sRad = sR;
		tAngle = tA;
		r = c.r;
		g = c.g;
		b = c.b;
	}
};

struct P
{
	glm::vec2 pos;
	GLfloat rot;

	int species;

	P(glm::vec2 p, GLfloat r, GLuint s)
	{
		pos = p;
		rot = r;
		species = s;
	}
};

class Particles
{
private:
	P* particles;

	S* species;

public:
	Particles(GLuint number, S* specs, GLuint numSpecs)
	{
		initParticles(number);
		initSpecies(specs, numSpecs);
	};

	void initParticles(GLuint number)
	{
		GLuint particleBuffer;
		glGenBuffers(1, &particleBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, number * sizeof(P), NULL, GL_STATIC_DRAW);

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // invalidate makes a ig difference when re-writting
		particles = (P*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, number * sizeof(P), bufMask);

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(100, 900); // distribution in range [1, 6]

		for (int i = 0; i < number; i++) {
			particles[i] = (P(glm::vec2(dist(rng), dist(rng)), 0, 0));
			//if (i > number / 2)
			//	particles[i].species = 1;
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particleBuffer);
	}

	void initSpecies(S* specs, GLuint numSpecs)
	{
		GLuint speciesBuffer;
		glGenBuffers(1, &speciesBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, speciesBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, numSpecs * sizeof(S), NULL, GL_STATIC_DRAW);

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // invalidate makes a ig difference when re-writting
		species = (S*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numSpecs * sizeof(S), bufMask);

		for (int i = 0; i < numSpecs; i++)
			species[i] = specs[i];

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, speciesBuffer);
	}

};