#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "glfw3.h"
#include "glm/vec4.hpp"

#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "OBJ.h"
#include "particle.h"

const GLuint SCREEN_WIDTH = 1500;
const GLuint SCREEN_HEIGHT = 1000;

const GLushort OPENGL_MAJOR_VERSION = 4;
const GLushort OPENGL_MINOR_VERSION = 6;

bool vSync = false;



GLfloat vertices[] =
{
	-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};


S species[] = {
	 S(
		 500,						// speed
		 25,						// sense distance
		 25,						// sense angle
		 1,							// sense radius
		 25,							// turn angle
		 glm::vec4(1, 0.5, 0.75, 1)	// colour
	 )
};

GLuint numP = 100000;

GLfloat decayRate = 1.5;
GLfloat diffuseRate = 1;

float deltaTime = 0;

bool renderGUI = true;

int main()
{
	glfwInit();

	Window window = Window(SCREEN_WIDTH, SCREEN_HEIGHT, OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION, "ayyyy", vSync);

	gladLoadGL();

	OBJ objects = OBJ(vertices, sizeof(vertices), indices, sizeof(indices));

	Texture2D screenTexture = Texture2D(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA32F, GL_READ_WRITE, 0);	

	ShaderVF screenShaders = ShaderVF("Screen_Vertex_Shader.vert", "Screen_Fragment_Shader.frag");

	Particles particles = Particles(numP, species, std::size(species));

	ComputeShader updatePhysarum = ComputeShader("update_physarum.comp");
	ComputeShader diffuseDecayPhysarum = ComputeShader("diffuse_decay_physarum.comp");

	GLint dtlocation = glGetUniformLocation(updatePhysarum.getID(), "deltaTime");

	GLint diffusedtlocation = glGetUniformLocation(diffuseDecayPhysarum.getID(), "deltaTime");
	GLint dRlocation = glGetUniformLocation(diffuseDecayPhysarum.getID(), "decayRate");
	GLint diRlocation = glGetUniformLocation(diffuseDecayPhysarum.getID(), "diffuseRate");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	float last = 0;
	float current = 0;

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		current = static_cast<float>(glfwGetTime());
		deltaTime = (current - last);
		last = current;

		if (glfwGetKey(window.getWindow(), GLFW_KEY_R) == GLFW_PRESS)
		{
			particles.initParticles(numP);
		}

		if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		{
			if (renderGUI)
				renderGUI = false;
			else
				renderGUI = true;
		}

		// Specify the color of the background
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(updatePhysarum.getID());
		glUniform1f(dtlocation, deltaTime);
		updatePhysarum.dispatch(ceil(numP / 16), 1, 1);

		glUseProgram(diffuseDecayPhysarum.getID());
		glUniform1f(diffusedtlocation, deltaTime);
		glUniform1f(diRlocation, diffuseRate);
		glUniform1f(dRlocation, decayRate);
		diffuseDecayPhysarum.dispatch(ceil(SCREEN_WIDTH / 8), ceil(SCREEN_HEIGHT / 8), 1);

		screenShaders.execute();
		screenTexture.bindTextureUnit();
		glUniform1i(glGetUniformLocation(screenShaders.getID(), "screen"), 0);
		glBindVertexArray(objects.VAO());
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		// Renders the ImGUI elements
		if (renderGUI)
		{
			// Tell OpenGL a new frame is about to begin
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// ImGUI window creation
			ImGui::Begin("diffuse settings");
			// Slider that appears in the window
			ImGui::SliderFloat("decay rate", &decayRate, 0.0f, 5.0f);
			ImGui::SliderFloat("diffuse rate", &diffuseRate, 0.0f, 5.0f);
			// Ends the window
			ImGui::End();

			bool changed = false;
			for (int i = 0; i < std::size(species); i++)
			{
				ImGui::Begin("physarum settings");
				ImGui::SliderFloat("Speed", &species[i].speed, 0.1f, 1000.0f);
				ImGui::SliderFloat("sense distance", &species[i].sDis, 1.0f, 100.f);
				ImGui::SliderFloat("sense angle", &species[i].sAngle, 0.1f, 180.0f);
				ImGui::SliderInt("sense radius", &species[i].sRad, 0, 4);
				ImGui::SliderFloat("turn angle", &species[i].tAngle, 0.1f, 180.0f);
				float* temp[3]{ &species[i].r, &species[i].g, &species[i].b };
				ImGui::ColorPicker3("colour", *temp, 0);
				if (ImGui::IsItemEdited)
				{
					particles.initSpecies(species, std::size(species));
				}
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	window.~Window();
	objects.~OBJ();
	screenShaders.~ShaderVF();
	updatePhysarum.~ComputeShader();
	diffuseDecayPhysarum.~ComputeShader();
	glfwTerminate();
}