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

// ---------------- Infinite Void ----------------

void InfiniteVoid::OnSureHit(Character & target) {
    if (clashing || target.IsHeavenlyRestricted()) return;
    target.Damage(surehit_braindamage * DomainRangeMult());
    target.SetStunState(true);
}

// ---------------- Malevolent Shrine ----------------

void MalevolentShrine::OnSureHit(Character & target) {
    if (clashing) return;
    target.Damage(surehit_slashdamage * DomainRangeMult());
}