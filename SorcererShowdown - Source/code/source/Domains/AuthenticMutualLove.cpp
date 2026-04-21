#include "AuthenticMutualLove.h"
#include "Character.h"

import std;

AuthenticMutualLove::AuthenticMutualLove() : Domain(600.0, 125.0, 16.0) {
    ref_level = Refinement::Refined;
    hit_type = HitType::HitsCurseUsers;
}
void AuthenticMutualLove::OnSureHit(CurseUser& user, Character& target) {
    if (CheckDomainSurehit(target)) return;
    target.DamageBypass(imbued_surehit_damage * DomainRangeMult());
    std::println("{} got bombarded by a barrage of copied techniques inside {}!", target.GetNameWithID(), this->GetDomainName());
}
std::string AuthenticMutualLove::GetDomainName() const {
    return "\033[95mAuthentic Mutual Love\033[0m";
}
double AuthenticMutualLove::GetUseCost() const {
    return domain_cost;
}
