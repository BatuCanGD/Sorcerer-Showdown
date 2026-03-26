#include "Domain.h"
#include "Character.h"
#include "Sorcerer.h"

import std;

InfiniteVoid::InfiniteVoid() : Domain(500.0, 100.0, 16.0) {}

void InfiniteVoid::OnSureHit(Character& target) {
    auto* s = dynamic_cast<Sorcerer*>(&target);
    if (clashing) return;
    else if (s && s->CounterDomainActive()) {
        std::println("{} protected himself from the {}'s surehit by using {}!", s->GetName(), this->GetDomainName(), s->GetCounterDomain()->GetDomainName());
        return;
    }
    else if (target.IsHeavenlyRestricted()) {
        std::println("{} couldnt detect {} due to their heavenly restriction\nThe domain's surehit didnt work!", this->GetDomainName(), target.GetName());
        return;
    }
    target.DamageBypass(surehit_braindamage * DomainRangeMult());
    target.SetStunState(true);
    std::println("{} got hit by {}'s SureHit!", target.GetName(), this->GetDomainName());
}
std::string InfiniteVoid::GetDomainName() const {
    return "Infinite Void";
}

double InfiniteVoid::GetUseCost() const {
    return domain_cost;
}