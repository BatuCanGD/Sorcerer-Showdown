#include "MalevolentShrine.h"
#include "Sorcerer.h"

import std;

MalevolentShrine::MalevolentShrine() : Domain(600.0, 200.0, 20.0) {}

void MalevolentShrine::OnSureHit(Character& target) {
    auto* s = dynamic_cast<Sorcerer*>(&target);
    if (clashing) return;
    else if (s && s->CounterDomainActive()) {
        std::println("{} protected himself from {}'s surehit by using {}!", s->GetName(), this->GetDomainName(), s->GetCounterDomain()->GetDomainName());
        return;
    }
    target.DamageBypass(surehit_slashdamage * DomainRangeMult());
    std::println("{} got hit by {}'s SureHit!", target.GetName(), this->GetDomainName());
}
std::string MalevolentShrine::GetDomainName() const {
    return "Malevolent Shrine";
}

double MalevolentShrine::GetUseCost() const {
    return domain_cost;
}
