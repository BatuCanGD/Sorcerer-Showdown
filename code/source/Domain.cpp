#include "Domain.h"
#include "Character.h"

import std;

// --- Base Domain  ---

Domain::Domain(double hp, double as, double rn) : 
    domain_health(hp), domain_overwhelm_strength(as), base_range(rn), current_range(rn) {}


bool Domain::Clashing() const {
    return clashing;
}

void Domain::SetClashState(bool a) {
    clashing = a;
}

double Domain::DomainRangeMult() const {
    return current_range / base_range;
}

std::string Domain::GetDomainName() const {
	return "Domain";
}

double Domain::GetDomainHealth() const {
    return domain_health;
}
double Domain::GetDomainStrength() const {
    return domain_overwhelm_strength;
}
double Domain::GetDomainRange() const {
    return current_range;
}

void Domain::DamageDomain(double damage) {
    domain_health = std::max(domain_health - damage, 0.0);
}

void Domain::ClashDomains(Domain& user1, Domain& user2) {
    if (user1.GetDomainRange() > user2.GetDomainRange()) {
        user2.DamageDomain(user1.GetDomainStrength());
    }
    else if (user2.GetDomainRange() > user1.GetDomainRange()) {
        user1.DamageDomain(user2.GetDomainStrength());
    }
    else {
        user1.DamageDomain(user2.GetDomainStrength() * 0.5);
        user2.DamageDomain(user1.GetDomainStrength() * 0.5);
    }
}

// ---------------- Infinite Void ----------------

InfiniteVoid::InfiniteVoid() : Domain(500.0, 100.0, 15.0) {}


void InfiniteVoid::OnSureHit(Character & target) {
    if (clashing || target.IsHeavenlyRestricted()) return;
    target.DamageBypass(surehit_braindamage * DomainRangeMult());
    target.SetStunState(true);
}
std::string InfiniteVoid::GetDomainName() const {
    return "Infinite Void";
}
// ---------------- Malevolent Shrine ----------------

MalevolentShrine::MalevolentShrine() : Domain(600.0, 200.0, 20.0) {}

void MalevolentShrine::OnSureHit(Character & target) {
    if (clashing) return;
    target.DamageBypass(surehit_slashdamage * DomainRangeMult());
}
std::string MalevolentShrine::GetDomainName() const {
    return "Malevolent Shrine";
}

// test domain for testing and fun purposes, use it to kill everyone in the game

KillEveryoneDomain::KillEveryoneDomain() : Domain(10000000.0, 10000000.0, 1000.0) {}

void KillEveryoneDomain::OnSureHit(Character & target) {
    if (clashing) return;
    target.DamageBypass(surehit_killdamage);
}
std::string KillEveryoneDomain::GetDomainName() const {
    return "KillEveryone Domain 3000";
}