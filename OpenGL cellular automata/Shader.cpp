#include "Shader.h"

std::string readFromFile(const char* filepath)
{
	std::ifstream in(filepath, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

ShaderVF::ShaderVF(const char* vertexFP, const char* fragmentVP)
{
	std::string vS = readFromFile(vertexFP);
	std::string fS = readFromFile(fragmentVP);

	const char* vertexSource = vS.c_str();
	const char* fragmentSource = fS.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

ComputeShader::ComputeShader(const char* filepath)
{
	std::string cS = readFromFile(filepath);

	const char* computeSource = cS.c_str();

	computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &computeSource, NULL);
	glCompileShader(computeShader);

	programID = glCreateProgram();
	glAttachShader(programID, computeShader);
	glLinkProgram(programID);
}

void ComputeShader::dispatch(GLuint x, GLuint y, GLuint z)
{
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}