#include "Core/Color.hpp"

std::string Color::RGB255_str()
{
	return std::to_string(static_cast<s32>(255 * r)) + " " + std::to_string(static_cast<s32>(255 * g)) + " " + std::to_string(static_cast<s32>(255 * b));
}