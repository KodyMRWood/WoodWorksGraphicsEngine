#include <iostream>

#include "Renderer.h"
// Error Clearing (After all errors are found and reported clear)
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
// Error checking and reporting
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << "In Function Call: " << function << ". In File: " << file << ". Line:" << line << std::endl;
		return false;
	}
	return true;
}