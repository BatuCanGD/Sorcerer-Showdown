#include "Domain.h"
#include "Character.h"
#include "Sorcerer.h"

import std;

// --- Base Domain  ---

Domain::Domain(double hp, double as, double rn) : 
    domain_health(hp), 
    base_health(hp),
    domain_overwhelm_strength(as), 
    base_range(rn), 
    current_range(rn) {
}


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

void Domain::ClashDomains(Sorcerer& user1, Sorcerer& user2) {
    Domain* d1 = user1.GetDomain();
    Domain* d2 = user2.GetDomain();

    if (d1->GetDomainRange() > d2->GetDomainRange()) {
        d2->DamageDomain(d1->GetDomainStrength());
        std::println("{} is overwhelming {}'s barrier!", d1->GetDomainName(), d2->GetDomainName());
    }
    else if (d2->GetDomainRange() > d1->GetDomainRange()) {
        d1->DamageDomain(d2->GetDomainStrength());
        std::println("{} is overwhelming {}'s barrier!", d2->GetDomainName(), d1->GetDomainName());
    }
    else {
        d1->DamageDomain(d2->GetDomainStrength() * 0.5);
        d2->DamageDomain(d1->GetDomainStrength() * 0.5);
        std::println("The domains of {} and {} are locked in an even struggle!", user1.GetName(), user2.GetName());
    }

    if (d1->IsDestroyed()) {
        std::println("{} has been overwhelmed and has collapsed", d1->GetDomainName());
        user1.DeactivateDomain();
        d1->CollapseDomain(); 
    }
    else if (d2->IsDestroyed()) {
        std::println("{} has been overwhelmed and has collapsed", d2->GetDomainName());
        user2.DeactivateDomain(); 
        d2->CollapseDomain(); 
    }

}

void Domain::CollapseDomain() {
    domain_health = base_health; // reset domain stats for next time use
    clashing = false;
}

bool Domain::IsDestroyed() const {
    return domain_health <= 0.0;
}
bool Domain::IsNeutralizer() const {
    return is_neutralizer;
}