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

// ---------------- Infinite Void ----------------

InfiniteVoid::InfiniteVoid() : Domain(500.0, 100.0, 15.0) {}


void InfiniteVoid::OnSureHit(Character & target) {
    auto* s = dynamic_cast<Sorcerer*>(&target);
    if (clashing || target.IsHeavenlyRestricted()) return;
    else if (s && s->CounterDomainActive()) {
        std::println("{} protected himself from the {}'s surehit by using {}!", s->GetName(), this->GetDomainName(),s->GetCounterDomain()->GetDomainName());
        return;
    }
    target.DamageBypass(surehit_braindamage * DomainRangeMult());
    target.SetStunState(true);
    std::println("{} got hit by {}'s SureHit!", target.GetName(), this->GetDomainName());
}
std::string InfiniteVoid::GetDomainName() const {
    return "Infinite Void";
}

double InfiniteVoid::GetUseCost() const {
    return domain_cost;
}

// ---------------- Malevolent Shrine ----------------

MalevolentShrine::MalevolentShrine() : Domain(600.0, 200.0, 20.0) {}

void MalevolentShrine::OnSureHit(Character & target) {
    auto* s = dynamic_cast<Sorcerer*>(&target);
    if (clashing) return;
    else if (s && s->CounterDomainActive()) {
        std::println("{} protected himself from {}'s surehit by using {}!", s->GetName(),this->GetDomainName(), s->GetCounterDomain()->GetDomainName());
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

// ---------------neutralizer domains---------

// simple domain

SimpleDomain::SimpleDomain() : Domain(std::numeric_limits<double>::infinity(), 3, 3) {
    is_neutralizer = true; 
}

void SimpleDomain::OnSureHit(Character&) {}

std::string SimpleDomain::GetDomainName() const { 
    return "Simple Domain";
}

double SimpleDomain::GetUseCost() const {
    return domain_cost;
}

// hollow wicker basket

HollowWickerBasket::HollowWickerBasket() : Domain(150.0, 2, 2) { 
    is_neutralizer = true; 
}

double HollowWickerBasket::GetUseCost() const {
    return domain_cost;
}

void HollowWickerBasket::OnSureHit(Character&) {}

std::string HollowWickerBasket::GetDomainName() const {
    return "Hollow Wicker Basket"; 
}


// test domain for testing and fun purposes, use it to kill everyone in the game

KillEveryoneDomain::KillEveryoneDomain() : Domain(10000000.0, 10000000.0, 1000.0) {}

void KillEveryoneDomain::OnSureHit(Character & target) {
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