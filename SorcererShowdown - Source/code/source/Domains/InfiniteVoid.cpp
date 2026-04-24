#include "InfiniteVoid.h"
#include "Character.h"

import std;

InfiniteVoid::InfiniteVoid() : Domain(800.0, 150.0, 16.0) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitsCurseUsers;
    domain_name = "Infinite Void";
    domain_color = "\033[34m";
}
void InfiniteVoid::OnSureHit(CurseUser& user, Character& target) {
    if (CheckDomainSurehit(target)) return;
    target.DamageBypass(surehit_braindamage * DomainRangeMult());
    target.SetStunState(true);
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}
double InfiniteVoid::GetUseCost() const {
    return domain_cost;
}

std::unique_ptr<Domain> InfiniteVoid::Clone() const {
    return std::make_unique<InfiniteVoid>(*this);
}