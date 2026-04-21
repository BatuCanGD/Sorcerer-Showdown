#include "MalevolentShrine.h"
#include "Character.h"

import std;

MalevolentShrine::MalevolentShrine() : Domain(1000.0, 300.0, 20.0) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitsEveryone;
}
void MalevolentShrine::OnSureHit(CurseUser& user, Character& target) {
    if (CheckDomainSurehit(target)) return;
    target.DamageBypass(surehit_slashdamage * DomainRangeMult());
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}
std::string MalevolentShrine::GetDomainName() const {
    return "\033[31mMalevolent Shrine\033[0m";
}
double MalevolentShrine::GetUseCost() const {
    return domain_cost;
}
