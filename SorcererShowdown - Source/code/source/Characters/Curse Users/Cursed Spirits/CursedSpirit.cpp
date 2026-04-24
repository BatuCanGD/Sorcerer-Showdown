#include "CursedSpirit.h"
import std;

CursedSpirit::CursedSpirit(double hp, double ce, double regen) : CurseUser(hp, ce, regen) {};

std::unique_ptr<Character> CursedSpirit::Clone() const {
	return nullptr;
}
bool CursedSpirit::CanBeHit() const {
	return true;
}
bool CursedSpirit::IsaCursedSpirit() const {
    return true;
}