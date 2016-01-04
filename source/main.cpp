#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include "IrisShader.h"

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	// uncomment these lines if on Apple OS X
	/*
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	*/

	window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	/* Start GLFW extension handler */
	glewExperimental = GL_TRUE;
	glewInit();

	/* Get version info */
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported: %s\n", version);

	/* Tell GL to only draw onto a pixel if the shape is closer to the vievwer*/
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	/* Load glsl from file */
	char* pvertex;
	char* pfragment;
	const char* vertex_name = "../../shader/vertex.glsl";
	const char* fragment_name = "../../shader/fragment.glsl";

	float points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	iris::IrisShader iris_shader;
	iris_shader.CreateProgram();
	iris_shader.LoadShader(vertex_name, GL_VERTEX_SHADER);
	iris_shader.Process();
	iris_shader.LoadShader(fragment_name, GL_FRAGMENT_SHADER);
	iris_shader.Process();
	iris_shader.LinkProgram();
	iris_shader.UseProgram();

	while (!glfwWindowShouldClose(window))
	{
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glUseProgram(shader_programme);
		//iris_shader.UseProgram();
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
