#include "Core/Window.hpp"
#include "Core/Logger.hpp"
#include "Core/LogLevels.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <queue>

Window::Window(s32 _width, s32 _height, const char* _title) : handle(nullptr), VAO(0), VBO(0), texture(0), shaderProgram(0), guiIO(nullptr), showDebugGui(false), screenShotOnQuit(true)
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

	ImGui::CreateContext();
	guiIO = &ImGui::GetIO();
	((ImGuiIO*)guiIO)->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	((ImGuiIO*)guiIO)->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)handle, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6);
}

Window::~Window()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

void Window::BeginUpdate()
{
	glfwPollEvents();

	if (glfwGetKey((GLFWwindow*)handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose((GLFWwindow*)handle, true);
	
	static bool canTrigger = true;
	if (canTrigger && glfwGetKey((GLFWwindow*)handle, GLFW_KEY_F3) == GLFW_PRESS) showDebugGui = !showDebugGui; canTrigger = false;
	if (!canTrigger && glfwGetKey((GLFWwindow*)handle, GLFW_KEY_F3) == GLFW_RELEASE) canTrigger = true;

	static bool shouldSave = false;
	if ((glfwGetKey((GLFWwindow*)handle, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS || glfwGetKey((GLFWwindow*)handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS))
	{
		if (glfwGetKey((GLFWwindow*)handle, GLFW_KEY_S) == GLFW_PRESS)
			shouldSave = true;
		if (shouldSave && glfwGetKey((GLFWwindow*)handle, GLFW_KEY_S) == GLFW_RELEASE)
		{
			screenShot();
			shouldSave = false;
		}
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	DrawDebugGUI();
}

void Window::EndUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers((GLFWwindow*)handle);
}

void Window::DrawDebugGUI()
{
	if (!showDebugGui) return;

	s32 windowWidth, windowHeight;
	glfwGetWindowSize((GLFWwindow*)handle, &windowWidth, &windowHeight);

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Screenshot", "Ctrl+S", nullptr))
		{
			screenShot();
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Exit", "Alt+F4", nullptr))
		{
			glfwSetWindowShouldClose((GLFWwindow*)handle, true);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Scene"))
	{
		static const char* aa= "test";
		static int sceneId = 0;
		if (ImGui::Combo("##sceneCombo", &sceneId, "Scene1\0Scene2")) onSceneChange(sceneId);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Settings"))
	{
		ImGui::MenuItem("Screenshot On Quit", NULL, &screenShotOnQuit);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	
	ImGui::SetNextWindowPos(ImVec2(20, windowHeight - 220));
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	if (ImGui::Begin("FPSWindow", &showDebugGui, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
	{
		f32 ms = ((ImGuiIO*)guiIO)->DeltaTime * 1000.f;
		ImGui::Text(std::format("FPS: {:7.3f}", ((ImGuiIO*)guiIO)->Framerate).c_str());
		ImGui::Text(std::format("ms : {:.3f}", ms).c_str());
		
		
		static f32 msQueue[50] = {};
		for (int i = 48; i >= 0; i--)
			msQueue[i + 1] = msQueue[i];
		msQueue[0] = ms;

		float average = 0.0f;
		for (int n = 0; n < IM_COUNTOF(msQueue); n++)
			average += msQueue[n];
		average /= (float)IM_COUNTOF(msQueue);
		char overlay[32];
		sprintf(overlay, std::format("avg {:.3f}", average).c_str());
		ImGui::PlotLines("##frameRateGraph", msQueue, IM_COUNTOF(msQueue), 0, overlay, -1.0f, 1.0f, ImVec2(182.f, 148.0f));

		ImGui::End();
	}
}