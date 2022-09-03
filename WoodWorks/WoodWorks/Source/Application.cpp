///////
//
// Author: Kody M. R. Wood
//
///////

//Preprocessers

//OpenGL Included Files
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"



int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Set up OpenGL Profile*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //(CORE_PROFILE must create a VAO, COMPAT_PROFILE gives a default one)

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1080, 720, "Wood Works", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Change sync of refresh rate to the monitor 1 = perfectsync, 1< slower
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error GLEW did not Initialize!" << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl; //Output GL Version to console
	{
	float vertPositions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f, 0.5f,
		-0.5f, 0.5f,
	};
	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	// Generating a Vertex Array Object (VAO)
	VertexArray va;

	// Vertex Buffer (Data Structure that hold vertex data)
	VertexBuffer vb(vertPositions, 4 * 2 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	// Index Buffer (Data Structure that hold the index of vertices)
	IndexBuffer ib(indices, 6);

	Shader shader("Resources/Shader/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.2f, 0.8f, 0.4f, 1.0f);

	va.Unbind();
	shader.Unbind();
	vb.Unbind();
	ib.Unbind();

	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// Rendering Process (Use shader, send Uniforms(done once a draw call), bind vao, draw call)
		shader.Bind();
		shader.SetUniform4f("u_Color", r, 0.8f, 0.4f, 1.0f);

		va.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (r > 1.0f)
		{
			increment = -0.05f;
		}
		else if (r < 0.0f)
		{
			increment = 0.05f;
		}
		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	}
    glfwTerminate();
    return 0;
}