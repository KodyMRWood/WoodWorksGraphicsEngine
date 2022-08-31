#pragma once

#include <GL/glew.h>

//Error Macros to wrap functions to check for errors
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// TODO: Make a function that converts the hex error code into real words