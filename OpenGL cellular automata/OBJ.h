#pragma once
#include "glad/glad.h"

class OBJ
{
private:
	GLuint vboID, vaoID, eboID;

public:
	OBJ(GLfloat* vertices, GLsizeiptr vertsize, GLuint* indices, GLsizeiptr indsize)
	{
		glCreateVertexArrays(1, &vaoID);
		glCreateBuffers(1, &vboID);
		glCreateBuffers(1, &eboID);

		glNamedBufferData(vboID, vertsize, vertices, GL_STATIC_DRAW);
		glNamedBufferData(eboID, indsize, indices, GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(vaoID, 0);
		glVertexArrayAttribBinding(vaoID, 0, 0);
		glVertexArrayAttribFormat(vaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);

		glEnableVertexArrayAttrib(vaoID, 1);
		glVertexArrayAttribBinding(vaoID, 1, 0);
		glVertexArrayAttribFormat(vaoID, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));

		glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, 5 * sizeof(GLfloat));
		glVertexArrayElementBuffer(vaoID, eboID);
	};
	~OBJ () 
	{
		glDeleteVertexArrays(1, &vboID);
		glDeleteVertexArrays(1, &vaoID);
		glDeleteVertexArrays(1, &eboID);
	}
	

	void bindVertexArray() { glBindVertexArray(vaoID); }

	GLuint VBO() { return vboID; }
	GLuint VAO() { return vaoID; }
	GLuint EBO() { return eboID; }

};