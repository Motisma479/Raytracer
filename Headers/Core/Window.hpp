#pragma once
#include "Core/Type.hpp"
#include "Core/Color.hpp"
#include "LibMath/Maths.hpp"

#include <vector>
#include <functional>
#include <string>

class Window
{
public:
	Window() = delete;
	Window(s32 _width, s32 _height, const char* _title);

	~Window();

	Maths::Vec2 GetPos();
	void        SetPos(const Maths::Vec2& _newPos);

	Maths::Vec2 GetSize();
	void        SetSize(const Maths::Vec2& _newSize);

	bool        ShouldClose();

	void        SendToScreen(const std::vector<Color>& _image);

	void        BeginUpdate();
	void        EndUpdate();

	std::function<void(void)> screenShot = []() {};
	std::function<void(s32)> onSceneChange = [](s32){};

	struct WindowCallback
	{
		std::function<void(s32, s32)> onSizeChange = [](s32, s32){};
	} windowCallback;
private:
	void* handle;
	std::string defaultTitle;

	//since the window is only used for rendering it will contain the shader, texture and mesh
	u32 VAO, VBO, texture, shaderProgram;

	void* guiIO;
	bool showDebugGui;
	void DrawDebugGUI();
	bool screenShotOnQuit;
};