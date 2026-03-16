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

std::string Sorcerer::GetRCTstatus() const {
    switch (rct_state) {
    case ReverseCT::Disabled: return "Disabled";
    case ReverseCT::Active: return "Active";
    case ReverseCT::Overdrive: return "Overdrive";
    }
}

void Sorcerer::UseRCT() {
    const double default_regen = 25.0;
    const double overdrive_regen = 75.0;
    if (rct_state == ReverseCT::Active) {
        this->Regen(default_regen);
        this->SpendCE(default_regen * 2);
    }
    else if (rct_state == ReverseCT::Overdrive) {
        this->Regen(overdrive_regen);
        this->SpendCE(overdrive_regen * 2);
    }
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
    std::println("\n*****Domain Expansion*****\n**{}**\n", this->GetDomain()->GetDomainName());
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

void Sorcerer::CleanupShikigami() {
    auto [removed_begin, removed_end] = std::ranges::remove_if(shikigami, [](const auto& s) {
        if (s->GetCharacterHealth() <= 0.0) {
            std::println("{} has been destroyed!", s->GetName());
            return true;
        }
        return false;
        });
    shikigami.erase(removed_begin, removed_end);
}

void Sorcerer::RecoverBurnout(Technique* t) {
    if (t == nullptr) return;
    
    if (t->BurntOut()) {
        burnout_time++;
        if (burnout_time >= max_burnout_time) {
            t->Set(Technique::Status::Usable);
            burnout_time = 0;
            std::println("{}'s cursed technique has recovered from burnout!", this->GetName());
        }
    }
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
    if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.60) {
        this->EnableRCT();
    }
    else if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.25) {
        this->BoostRCT();
    }

    for (const auto& target : battlefield) {
        if (target.get() == this) continue;

        if (target->DomainActive() && !this->DomainActive() && !this->GetTechnique()->BurntOut()) {
            this->ActivateDomain();
            return;
        }

        auto* limitless = dynamic_cast<Limitless*>(this->GetTechnique());
        if (limitless && (limitless->Usable() || limitless->Boosted())) {
            int roll = GetRandomNumber(1, 100);
            if (roll <= 15 && this->GetCharacterCE() > 3000) {
                limitless->UseTheLimitlessTechnique(Limitless::LimitlessType::Purple, this, target.get());
            }
            else if(roll <= 60){
                limitless->UseTheLimitlessTechnique(Limitless::LimitlessType::Blue, this, target.get());
            }
            else {
                limitless->UseTheLimitlessTechnique(Limitless::LimitlessType::Red, this, target.get());
            }
        }
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
    if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.55) {
        this->EnableRCT();
    }
    else if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.35) {
        this->BoostRCT();
    }

    std::vector<Sorcerer*> domain_users;
    for (const auto& target : battlefield) {
        if (target.get() == this) continue;
        if (target->DomainActive()) {
            domain_users.push_back(target.get());
        }
    }

    Mahoraga* makora = nullptr;
    Agito* agito = nullptr;

    for (const auto& s : shikigami) {
        if (s->GetName() == "Mahoraga") {
            makora = dynamic_cast<Mahoraga*>(s.get());
        }
        else if (s->GetName() == "Agito") {
            agito = dynamic_cast<Agito*>(s.get());
        }
    }

    if (makora) {
        if (this->GetCharacterHealth() < GetCharacterMaxHealth() * 0.40) {
            makora->Manifest();
        }
        else if (!makora->IsActive()) {
            makora->PartiallyManifest();
        }
    }

    if (agito && this->GetCharacterHealth() < 500) {
        if (!agito->IsActive()) {
            agito->Manifest();
        }
    }

    for (const auto& s : shikigami) {
        s->OnShikigamiTurn(this);
    }

    auto shrine = dynamic_cast<Shrine*> (this->GetTechnique());
    int roll = GetRandomNumber(1, 100);
    for (const auto& target : battlefield) {
        if (target.get() == this) continue;

        if (shrine->WorldCuttingSlashUnlocked()) {
            shrine->WorldCuttingSlashToTarget(this, target.get());
            return;
        }

        if (target->GetCharacterHealth() < GetCharacterMaxHealth() * 0.25 && roll <= 15) {
            shrine->UseShrineTechnique(Shrine::ShrineType::Cleave, this, target.get());
        }
        else if (target->CanBeHit()) {
            shrine->UseShrineTechnique(Shrine::ShrineType::Dismantle, this, target.get());
        }
    }


    if (domain_users.size() < 2 && !this->DomainActive() && !this->GetTechnique()->BurntOut()) {
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
    std::println("John Zenin just stands there... menacingly.");
}
std::string test_sorcerer::GetName() const {
    return "John Zenin";
}

bool test_sorcerer::CanBeHit() const {
    return true;
}