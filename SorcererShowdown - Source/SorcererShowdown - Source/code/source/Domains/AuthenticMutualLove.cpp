#include "AuthenticMutualLove.h"
#include "Sorcerer.h"

import std;

AuthenticMutualLove::AuthenticMutualLove() : Domain(500.0, 100.0, 16.0) {}

void AuthenticMutualLove::OnSureHit(Character& target) {
    auto* s = dynamic_cast<Sorcerer*>(&target);
    if (clashing) return;
    else if (s && s->CounterDomainActive()) {
        std::println("{} protected themselves from the copied techniques using {}!", s->GetName(), s->GetCounterDomain()->GetDomainName());
        return;
    }
    else if (target.IsHeavenlyRestricted()) {
        std::println("{} couldnt detect {} due to their heavenly restriction\nThe domain's surehit didnt work!", this->GetDomainName(), target.GetName());
        return;
    }

    target.DamageBypass(imbued_surehit_damage * DomainRangeMult());
    std::println("{} got bombarded by a barrage of copied techniques inside {}!", target.GetName(), this->GetDomainName());
}

std::string AuthenticMutualLove::GetDomainName() const {
    return "\033[95mAuthentic Mutual Love\033[0m";
}

double AuthenticMutualLove::GetUseCost() const {
    return domain_cost;
}
