#include "Domain.h"
#include "Character.h"

// --- Base Domain  ---

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


// ---------------- Infinite Void ----------------

void InfiniteVoid::OnSureHit(Character & target) {
    if (clashing || target.IsHeavenlyRestricted()) return;
    target.DamageBypass(surehit_braindamage * DomainRangeMult());
    target.SetStunState(true);
}
std::string InfiniteVoid::GetDomainName() const {
    return "Infinite Void";
}
// ---------------- Malevolent Shrine ----------------

void MalevolentShrine::OnSureHit(Character & target) {
    if (clashing) return;
    target.DamageBypass(surehit_slashdamage * DomainRangeMult());
}
std::string MalevolentShrine::GetDomainName() const {
    return "Malevolent Shrine";
}

// test domain for testing and fun purposes, use it to kill everyone in the game

void KillEveryoneDomain::OnSureHit(Character & target) {
    if (clashing) return;
    target.DamageBypass(surehit_killdamage);
}
std::string KillEveryoneDomain::GetDomainName() const {
    return "KillEveryone Domain 3000";
}