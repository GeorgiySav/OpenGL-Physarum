#pragma once
#include "glad/glad.h"


class Texture2D
{
private:
	GLuint texture;
	GLuint unit;
public:
	Texture2D(GLuint WIDTH, GLuint HEIGHT, GLuint format, GLuint access, GLuint unit);


	void bindTextureUnit() { glBindTextureUnit(unit, texture); };

	GLuint getTexture() { return texture; }
	GLuint getUnit() { return unit; }
};