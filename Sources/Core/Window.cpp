#include "Core/Window.hpp"
#include "Core/Logger.hpp"
#include "Core/LogLevels.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

Window::Window(s32 _width, s32 _height, const char* _title) : handle(nullptr)
{
	if (!glfwInit())
	{
		ERROR_LOG_EX("Cannot initialize GLFW", "Window Init", DebugVerbosity::LITTLE_DEBUG);
		return;
	}
	defaultTitle = _title;
	handle = glfwCreateWindow(_width, _height, _title, NULL, NULL);
	if (!handle)
	{
		glfwTerminate();
		ERROR_LOG_EX("Cannot create GLFW window", "Window Init", DebugVerbosity::LITTLE_DEBUG);
		return;
	}

	glfwMakeContextCurrent((GLFWwindow*)handle);

	if (!gladLoadGL(glfwGetProcAddress))
	{
		glfwTerminate();
		ERROR_LOG_EX("Cannot initialize GLAD", "Window Init", DebugVerbosity::LITTLE_DEBUG);
		return ;
	}

	glClearColor(1, 0, 0, 1);
}

Window::~Window()
{
	glfwTerminate();
}

Maths::Vec2 Window::GetPos()
{
	s32 x, y;
	glfwGetWindowPos((GLFWwindow*)handle, &x, &y);
	return { (f32)x, (f32)y };
}

void Window::SetPos(const Maths::Vec2& _newPos)
{
	glfwSetWindowPos((GLFWwindow*)handle, (s32)_newPos.x, (s32)_newPos.y);
}

Maths::Vec2 Window::GetSize()
{
	s32 width, heigth;
	glfwGetWindowSize((GLFWwindow*)handle, &width, &heigth);
	return { (f32)width, (f32)heigth };
}
void Window::SetSize(const Maths::Vec2& _newSize)
{
	glfwSetWindowSize((GLFWwindow*)handle, (s32)_newSize.x, (s32)_newSize.y);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose((GLFWwindow*)handle);
}

void Window::Update()
{
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers((GLFWwindow*)handle);
	glfwPollEvents();
}