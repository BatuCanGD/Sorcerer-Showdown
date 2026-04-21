#include "SelfEmbodimentOfPerfection.h"
#include "Character.h"

import std;

SelfEmbodimentOfPerfection::SelfEmbodimentOfPerfection() : Domain(500.0, 100.0, 16.0) {
    ref_level = Refinement::Refined;
    hit_type = HitType::HitsCurseUsers;
};
void SelfEmbodimentOfPerfection::OnSureHit(CurseUser& user, Character& target) {
    if (CheckDomainSurehit(target)) return;
    target.DamageBypass(surehit_damage * DomainRangeMult());
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}
std::string SelfEmbodimentOfPerfection::GetDomainName() const {
	return "\033[35mSelf Embodiment Of Perfection\033[0m";
}
double SelfEmbodimentOfPerfection::GetUseCost() const {
	return domain_cost;
}