#include "Sorcerer.h"
#include "Technique.h"

import std;

bool Sorcerer::DomainActive() const {
    return domain_active;
}

Domain* Sorcerer::GetDomain() {
    return domain.get();
}

Technique* Sorcerer::GetTechnique() {
    return technique.get();
}

CombatContext* Sorcerer::GetSpecial() {
    return special.get();
}

const std::vector<std::unique_ptr<Shikigami>>& Sorcerer::GetShikigami() const {
    return shikigami;
}

void Sorcerer::SetAmplification(bool t) {
    domain_amplification_active = t;
}

void Sorcerer::CheckSpecial(Sorcerer* sorcerer) {
    if (GetSpecial() == nullptr) return;

    auto wcs = dynamic_cast<WorldCuttingSlash*>(special.get());


    if (auto* limitless = dynamic_cast<Limitless*>(technique.get())) {
        std::println("no limitless special yet");
    }
    else if (auto* shrine = dynamic_cast<Shrine*>(technique.get())) {
        if (wcs) {
            wcs->WorldCuttingSlashReady(sorcerer);
        }
    }
}

void Sorcerer::SetSixEyes(bool t) {
    six_eyes = t;
}

bool Sorcerer::HasSixEyes() const {
    return six_eyes;
}

void Sorcerer::SpendCE(double ce) {
    double efficiency = 1.0;
    if (HasSixEyes()) {
        efficiency = 0.05;
        if (technique && technique->GetStatus() == Technique::Status::BurntOut) {
            efficiency = 0.50; 
        }
    }
    cursed_energy -= (ce * efficiency);
}

void Sorcerer::DisableRCT() {
    rct_state = ReverseCT::Disabled;
}

void Sorcerer::EnableRCT() {
    rct_state = ReverseCT::Active;
}

void Sorcerer::BoostRCT() {
    rct_state = ReverseCT::Overdrive;
}

void Sorcerer::DeactivateDomain() {
    domain_active = false;
    if (technique) {
        technique->Set(Technique::Status::BurntOut);
    }
}

void Sorcerer::Attack(Character* target) {
     target->Damage(base_attack_damage);
}


void Sorcerer::ActivateDomain() {
    if (!domain) {
        std::println("You don't have a domain to activate!");
        return;
	}
    if (domain_active) {
        std::println("Your domain is already active!");
        return;
	}
    if (total_domain_uses >= domain_limit) {
        this->DamageBypass(50.0);
        this->SetStunState(true);
        std::println("You have overused your domain! You take 50 damage and are stunned for the next turn.");
        return;
    }
    domain_active = true;
    total_domain_uses++;
    std::println("\n*****Domain Expansion*****\n**{}**", this->GetDomain()->GetDomainName());
    if (technique) {
        technique->Set(Technique::Status::DomainBoost);
    }
}

std::string Sorcerer::GetName() const {
    return "Sorcerer";
}

bool Sorcerer::IsThePlayer() const {
    return is_player;
}

void Sorcerer::SetAsPlayer(bool p) {
    is_player = p;
}

void Sorcerer::DomainDrain() {
    if (DomainActive()) {
        double shrinecost = 350.0;
        double limitlesscost = 500.0;

        Domain* currentDomain = GetDomain();

        if (dynamic_cast<InfiniteVoid*>(currentDomain)) {
            SpendCE(limitlesscost);
        }
        else if (dynamic_cast<MalevolentShrine*>(currentDomain)) {
            SpendCE(shrinecost);
        }

    }
}

bool Sorcerer::CanBeHit() const {
    return true;
}

// ---------------- Gojo -------------------

Gojo::Gojo() : Sorcerer(800.0, 4000.0, 50.0) {
    domain = std::make_unique<InfiniteVoid>();
    technique = std::make_unique<Limitless>();
    SetSixEyes(true);
}

std::string Gojo::GetName() const {
    return "Gojo";
}

void Gojo::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    if (this->GetCharacterHealth() <= (this->GetCharacterMaxHealth() * 0.65)) {
        Technique* tech = this->GetTechnique();
        double max_rct_regen = 125.0;
        double mult = tech->GetTechniqueOutput();
        this->Regen(max_rct_regen * mult);
        this->SpendCE(max_rct_regen / mult);
    }

    for (const auto& target : battlefield) {
        if (target.get() == this) continue;

        if (target->DomainActive() && !this->DomainActive()) {
            this->ActivateDomain();
            return;
        }

        std::println("Gojo fights back with his own technique!");
    }
}

bool Gojo::CanBeHit() const {
    auto* limitless = dynamic_cast<Limitless*>(technique.get());
    if (limitless->CheckInfinity()) {
        return false;
    }
    return true;
}

// ---------------- Sukuna -------------------

Sukuna::Sukuna() : Sorcerer(1000.0, 12000.0, 25.0) {
    domain = std::make_unique<MalevolentShrine>();
    technique = std::make_unique<Shrine>();
	shikigami.push_back(std::make_unique<Mahoraga>());
	shikigami.push_back(std::make_unique<Agito>());
	special = std::make_unique<WorldCuttingSlash>();
}

std::string Sukuna::GetName() const {
    return "Sukuna";
}

void Sukuna::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    Technique* tech = this->GetTechnique();
    double max_rct_regen = 125.0;

    if (this->GetCharacterHealth() <= (this->GetCharacterMaxHealth() * 0.70)) {
        double mult = tech->GetTechniqueOutput();
        this->Regen(max_rct_regen * mult);
        this->SpendCE(max_rct_regen / mult);
    }

    std::vector<Sorcerer*> domain_users;
    for (const auto& target : battlefield) {
        if (target.get() == this) continue;
        if (target->DomainActive()) {
            domain_users.push_back(target.get());
        }
    }
    if (domain_users.size() > 2 || domain_users.size() == 2) return;

    if (!this->GetTechnique()->BurntOut()) {
        this->ActivateDomain();
    }
    
}

bool Sukuna::CanBeHit() const {
    return true;
}

/// for testing stuff, use this class

test_sorcerer::test_sorcerer() : Sorcerer(30000000.0, 30000000.0, 1000.0) {
	domain = std::make_unique<KillEveryoneDomain>();
}

void test_sorcerer::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    std::println("I'm the strongest dude");
}
std::string test_sorcerer::GetName() const {
    return "John Zenin";
}

bool test_sorcerer::CanBeHit() const {
    return true;
}