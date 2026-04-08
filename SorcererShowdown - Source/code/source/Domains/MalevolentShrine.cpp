#include "MalevolentShrine.h"
#include "Sorcerer.h"

import std;

MalevolentShrine::MalevolentShrine() : Domain(600.0, 200.0, 20.0) {
    ref_level = Refinement::Absolute;
}

void MalevolentShrine::OnSureHit(Character& target) {
    auto* s = dynamic_cast<CurseUser*>(&target);
    if (clashing) return;
    else if (s && s->CounterDomainActive()) {
        std::println("{} protected himself from {}'s surehit by using {}!", s->GetNameWithID(), this->GetDomainName(), s->GetCounterDomain()->GetDomainName());
        return;
    }
    target.DamageBypass(surehit_slashdamage * DomainRangeMult());
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}
std::string MalevolentShrine::GetDomainName() const {
    return "\033[31mMalevolent Shrine\033[0m";
}

double MalevolentShrine::GetUseCost() const {
    return domain_cost;
}
