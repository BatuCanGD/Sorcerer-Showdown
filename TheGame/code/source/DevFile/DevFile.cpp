#include "Sorcerer.h"
#include "Techniques.h"
#include "Shikigami.h"
#include "Specials.h"
#include "Domain.h"
#include "Utils.h"

import std;

test_sorcerer::test_sorcerer() : Sorcerer(30000000.0, 30000000.0, 1000.0) {
    technique = std::make_unique<Copy>();
    domain = std::make_unique<KillEveryoneDomain>();
    counter_domain = std::make_unique<SimpleDomain>();
    black_flash_chance = 100;
}

void test_sorcerer::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    std::println("John Zenin just stands there... menacingly.");
    if (this->domain && !this->DomainActive() && !(this->GetTechnique()->BurntOut() || this->GetTechnique()->Boosted())) {
        this->ActivateDomain();
    }
    for (const auto& s : battlefield) {
        if (s.get() == this) continue;
        this->Taunt(s.get());
    }
}
std::string test_sorcerer::GetName() const {
    return "John Zenin";
}

bool test_sorcerer::CanBeHit() const {
    return true;
}

// Domain

KillEveryoneDomain::KillEveryoneDomain() : Domain(10000000.0, 10000000.0, 1000.0) {}

void KillEveryoneDomain::OnSureHit(Character& target) {
    if (clashing) return;
    target.DamageBypass(surehit_killdamage);
    std::println("{} got hit by {}'s SureHit!", target.GetName(), this->GetDomainName());
}
std::string KillEveryoneDomain::GetDomainName() const {
    return "KillEveryone Domain 3000";
}

double KillEveryoneDomain::GetUseCost() const {
    return domain_cost;
}