#include "Sorcerer.h"
#include "Limitless.h"
#include "UnlimitedPurple.h"
#include "SimpleDomain.h"
#include "DevFile.h"
#include "ShikigamiList.h"
#include "CursedToolList.h"

import std;

test_sorcerer::test_sorcerer() : Sorcerer(INT32_MAX, INT32_MAX, 0.0) {
    technique = std::make_unique<Limitless>();
    special = std::make_unique<UnlimitedPurple>();
    domain = std::make_unique<KillEveryoneDomain>();
    counter_domain = std::make_unique<SimpleDomain>();
    shikigami.push_back(std::make_unique<Rika>());
    shikigami.push_back(std::make_unique<Agito>());
    shikigami.push_back(std::make_unique<Mahoraga>());
    inventory_curse.push_back(std::make_unique<Katana>());
    inventory_curse.push_back(std::make_unique<InvertedSpearofHeaven>());
    inventory_curse.push_back(std::make_unique<PlayfulCloud>());
    black_flash_chance = 100;
}
std::unique_ptr<Character> test_sorcerer::Clone() const {
    return std::make_unique<test_sorcerer>();
}

void test_sorcerer::OnCharacterTurn(Character*, Battlefield& bf) {
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