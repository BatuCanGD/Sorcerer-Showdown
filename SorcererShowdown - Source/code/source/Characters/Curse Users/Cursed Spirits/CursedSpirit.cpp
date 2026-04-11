#include "CursedSpirit.h"

CursedSpirit::CursedSpirit(double hp, double ce, double regen) : CurseUser(hp, ce, regen) {};

std::string CursedSpirit::GetSimpleName() const {
	return "Cursed Spirit";
}
std::string CursedSpirit::GetName() const {
	return "\033[38;5;183mCursed Spirit\033[0m";
}
bool CursedSpirit::CanBeHit() const {
	return true;
}