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
		glfwDestroyWindow((GLFWwindow*)handle);
		glfwTerminate();
		ERROR_LOG_EX("Cannot initialize GLAD", "Window Init", DebugVerbosity::LITTLE_DEBUG);
		return ;
	}

	glClearColor(1, 0, 0, 1);

	f32 vertices[] =
	{
		-1.0f,  1.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,
		
		-1.0f,  1.0f,  0.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	auto compileShader = [](u32 type, const char* data)
	{
		u32 shader = glCreateShader(type);
		glShaderSource(shader, 1, &data, nullptr);
		glCompileShader(shader);

		s32 success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetShaderInfoLog(shader, 512, nullptr, log);
			ERROR_LOG_EX(std::format("Shader error: {:}", log), "Window Init", DebugVerbosity::LITTLE_DEBUG);
		}
		return shader;
	};

	const char* vertexShaderSrc = R"(
#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

out vec2 UV;

void main()
{
    UV = aUV;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

	const char* fragmentShaderSrc = R"(
#version 460 core
in vec2 UV;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture(screenTexture, vec2(UV.x, 1.0-UV.y));
}
)";

	u32 vert = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
	u32 frag = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
	
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vert);
	glAttachShader(shaderProgram, frag);
	glLinkProgram(shaderProgram);
	glDeleteShader(vert);
	glDeleteShader(frag);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Window::~Window()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow((GLFWwindow*)handle);
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

void Window::SendToScreen(const std::vector<Color>& _image)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	s32 width, height;
	glfwGetWindowSize((GLFWwindow*)handle, &width, &height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, _image.data());

}

void Window::Update()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers((GLFWwindow*)handle);
	glfwPollEvents();
}