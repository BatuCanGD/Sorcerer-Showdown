#include "InfiniteVoid.h"
#include "Sorcerer.h"

import std;

InfiniteVoid::InfiniteVoid() : Domain(500.0, 100.0, 16.0) {}

void InfiniteVoid::OnSureHit(Character& target) {
    auto* s = dynamic_cast<CurseUser*>(&target);
    if (clashing) return;
    else if (s && s->CounterDomainActive()) {
        std::println("{} protected himself from the {}'s surehit by using {}!", s->GetNameWithID(), this->GetDomainName(), s->GetCounterDomain()->GetDomainName());
        return;
    }
    else if (target.IsPhysicallyGifted()) {
        std::println("{} couldnt detect {} due to their heavenly restriction\nThe domain's surehit didnt work!", this->GetDomainName(), target.GetNameWithID());
        return;
    }
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