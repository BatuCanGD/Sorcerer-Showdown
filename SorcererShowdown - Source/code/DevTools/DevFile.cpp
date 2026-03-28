#include "Sorcerer.h"
#include "Shrine.h"
#include "WorldCuttingSlash.h"
#include "SimpleDomain.h"
#include "DevFile.h"
#include "ShikigamiList.h"

import std;

test_sorcerer::test_sorcerer() : Sorcerer(INT32_MAX, INT32_MAX, 0.0) {
    technique = std::make_unique<Shrine>();
    special = std::make_unique<WorldCuttingSlash>();
    domain = std::make_unique<KillEveryoneDomain>();
    counter_domain = std::make_unique<SimpleDomain>();
    shikigami.push_back(std::make_unique<Rika>());
    shikigami.push_back(std::make_unique<Agito>());
    shikigami.push_back(std::make_unique<Mahoraga>());
    black_flash_chance = 100;
}

void test_sorcerer::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    std::println("Exception thrown");
    std::println("Access violation reading location 0x000000000000000C.");
}
std::string test_sorcerer::GetName() const {
    return "NullPointer";
}

bool test_sorcerer::CanBeHit() const {
    return true;
}

KillEveryoneDomain::KillEveryoneDomain() : Domain(10000000.0, 10000000.0, 1000.0) {}

void KillEveryoneDomain::OnSureHit(Character& target) {
    target.DamageBypass(surehit_killdamage);
    std::println("{} got erased by {}'s SureHit!", target.GetName(), this->GetDomainName());
}
std::string KillEveryoneDomain::GetDomainName() const {
    return "Hard-Coded Inevitability";
}

double KillEveryoneDomain::GetUseCost() const {
    return domain_cost;
}
