#pragma once
#include "Core/Type.hpp"
#include "LibMath/Maths.hpp"

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

	void        Update();
private:
	void* handle;
	std::string defaultTitle;
};