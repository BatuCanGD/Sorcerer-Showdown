#include "Specials.h"
#include "Utils.h"
import std;

std::string Specials::GetSpecialName() const {
	return std::format("{}{}{}", special_color, special_name, Color::Clear);
}