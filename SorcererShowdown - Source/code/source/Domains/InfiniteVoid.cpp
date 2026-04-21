#include "InfiniteVoid.h"
#include "Character.h"

import std;

InfiniteVoid::InfiniteVoid() : Domain(800.0, 150.0, 16.0) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitsCurseUsers;
}
void InfiniteVoid::OnSureHit(CurseUser& user, Character& target) {
    if (CheckDomainSurehit(target)) return;
    target.DamageBypass(surehit_braindamage * DomainRangeMult());
    target.SetStunState(true);
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}
std::string InfiniteVoid::GetDomainName() const {
    return "\033[34mInfinite Void\033[0m";
}
double InfiniteVoid::GetUseCost() const {
    return domain_cost;
}