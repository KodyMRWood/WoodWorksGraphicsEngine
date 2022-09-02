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

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"




struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1,
	};
	
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//Error Message
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program,fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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

	ShaderProgramSource source = ParseShader("Resources/Shader/Basic.shader");
	//Outputs the vertex and fragment shader (Maybe change to somehow get the name of the shader)
	//std::cout << "VERTEX" << std::endl;
	//std::cout << source.VertexSource << std::endl;
	//std::cout << "FRAGMENT" << std::endl;
	//std::cout << source.FragmentSource << std::endl;
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1); //Error check, Uniform not found
	GLCall(glUniform4f(location, 0.2f, 0.8f, 0.4f, 1.0f));

	va.Unbind();
	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// Rendering Process (Use shader, send Uniforms(done once a draw call), bind vao, draw call)
		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.8f, 0.4f, 1.0f));

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

	//Deletes the shaders being used for clean up
	glDeleteProgram(shader);
	}
    glfwTerminate();
    return 0;
}