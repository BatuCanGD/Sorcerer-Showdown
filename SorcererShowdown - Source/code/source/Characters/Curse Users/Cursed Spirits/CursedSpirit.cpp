#include "CursedSpirit.h"
import std;

CursedSpirit::CursedSpirit(double hp, double ce, double regen) : CurseUser(hp, ce, regen) {};

std::string CursedSpirit::GetSimpleName() const {
	return "Cursed Spirit";
}
std::string CursedSpirit::GetName() const {
	return "\033[38;5;183mCursed Spirit\033[0m";
}
std::unique_ptr<Character> CursedSpirit::Clone() const {
	return nullptr;
}
bool CursedSpirit::CanBeHit() const {
	return true;
}
bool CursedSpirit::IsaCursedSpirit() const {
    return true;
}