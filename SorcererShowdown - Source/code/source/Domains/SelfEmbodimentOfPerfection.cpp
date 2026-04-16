#include "SelfEmbodimentOfPerfection.h"
#include "CurseUser.h"

import std;

SelfEmbodimentOfPerfection::SelfEmbodimentOfPerfection() : Domain(500.0, surehit_damage, 16.0) {};

void SelfEmbodimentOfPerfection::OnSureHit(Character& target) {
    auto* s = dynamic_cast<CurseUser*>(&target);
    if (clashing) return;
    else if (s && s->CounterDomainActive()) {
        std::println("{} protected himself from {}'s surehit by using {}!", s->GetNameWithID(), this->GetDomainName(), s->GetCounterDomain()->GetDomainName());
        return;
    }
    target.DamageBypass(surehit_damage * DomainRangeMult());
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}

std::string SelfEmbodimentOfPerfection::GetDomainName() const {
	return "\033[35mSelf Embodiment Of Perfection\033[0m";
}

double SelfEmbodimentOfPerfection::GetUseCost() const {
	return domain_cost;
}