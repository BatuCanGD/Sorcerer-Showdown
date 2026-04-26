#include "CursedSpirit.h"
#include "Domain.h"
#include "Techniques.h"
import std;

CursedSpirit::CursedSpirit(double hp, double ce, double regen) : CurseUser(hp, ce, regen) {};

std::unique_ptr<Character> CursedSpirit::Clone() const {
    auto cs = std::make_unique<CursedSpirit>(max_health, max_cursed_energy, ce_regen);

    if (this->technique) cs->SetTechnique(this->technique->Clone());
    if (this->domain)    cs->SetDomain(this->domain->Clone());

    cs->SetCharacterName(this->char_name, this->name_color);

    return cs;
}

bool CursedSpirit::IsaCursedSpirit() const {
    return true;
}