#include "Texture.h"

Texture2D::Texture2D(GLuint WIDTH, GLuint HEIGHT, GLuint format, GLuint access, GLuint unit)
{
	this->unit = unit;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(texture, 1, format, WIDTH, HEIGHT);
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, access, format);
}