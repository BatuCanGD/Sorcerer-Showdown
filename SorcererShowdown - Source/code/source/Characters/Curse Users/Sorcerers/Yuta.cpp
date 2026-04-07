#include "Yuta.h"
#include "Copy.h"
#include "Limitless.h"
#include "Rika.h"
#include "AuthenticMutualLove.h"
#include "SimpleDomain.h"
#include "Katana.h"
#include "Utils.h"

#include "PhysicallyGifted.h"

import std;

// -------------- Yuta ------------------

Yuta::Yuta() : Sorcerer(800.0, 6000.0, 30.0) {
    technique = std::make_unique<Copy>();
    cursed_tool = std::make_unique<Katana>();
    domain = std::make_unique<AuthenticMutualLove>();
    counter_domain = std::make_unique<SimpleDomain>();
    shikigami.push_back(std::make_unique<Rika>());
    black_flash_chance = 10;
}

std::unique_ptr<Character> Yuta::Clone() const {
    return std::make_unique<Yuta>();
}

std::string Yuta::GetName() const {
    return "\033[95mYuta Okkotsu\033[0m";
}
std::string Yuta::GetSimpleName() const {
    return "Yuta Okkotsu";
}

void Yuta::OnCharacterTurn(Character* unused, std::vector<std::unique_ptr<Character>>& battlefield) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetName());
        return;
    }
    Shikigami* rika = this->ChooseShikigami(0);

    if (!(this->HPMoreThanMax(0.40) || this->CEMoreThanMax(0.20))) {
        if (!((rika->GetActiveTime() > 5) && rika->IsActivePhysically())) {
            std::println("Come, Rika.");
            rika->Manifest();
        }
    }

    if (!this->HPMoreThanMax(0.40) || rika->IsActivePhysically()) {
        this->BoostRCT();
    }
    else if (!(this->HPMoreThanMax(0.55))) {
        this->EnableRCT();
    }
    else {
        this->DisableRCT();
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& s : battlefield) {
        if (s.get() == this) continue;
        double score = s->GetCharacterHealth() / this->GetCharacterMaxHealth();

        if (auto curse_user = dynamic_cast<CurseUser*>(s.get())) {
            if (curse_user->DomainActive()) {
                domain_users.push_back(curse_user);
                score += 0.50;
            }

            if (curse_user->GetTechnique()) {
                std::string tech = curse_user->GetTechnique()->GetTechniqueSimpleName();
                if (tech == "Shrine") score += 0.80;
                if (tech == "Limitless") score += 0.15;
            }
        }
        else if (dynamic_cast<PhysicallyGifted*>(s.get())) {
            score += 0.25;
        }

        score += GetRandomNumber(-5, 5) * 0.01;

        if (score > best_score) {
            best_score = score;
            strongest = s.get();
        }
    }

    int tntroll = GetRandomNumber(1, 20);
    if (tntroll <= 4) {
        this->Taunt(strongest);
    }

    if (!domain_users.empty()) {
        if (!this->CounterDomainActive() && (this->GetTechnique()->BurntOut() || this->GetDomainUses() >= 5)) {
            this->ActivateCounterDomain();
            return;
        }
        else if (!this->DomainActive() && !this->GetTechnique()->BurntOut() && this->GetDomainUses() < 5) {
            int chroll = GetRandomNumber(1, 20);
            if (chroll >= 15) {
                this->ActivateDomain();
                return;
            }
            else {
                if (domain_users.size() <= 1) {
                    this->ActivateDomain();
                    return;
                }
            }
        }
    }
    else {
        if (this->CounterDomainActive()) this->DeactivateCounterDomain();
    }
    InfCheck(strongest);
    HitCharacter(strongest);
}

void Yuta::HitCharacter(Character* strongest) {
    this->Attack(strongest);
    if (this->DomainAmplificationActive()) this->SetAmplification(false);
}

bool Yuta::InfCheck(Character* strongest) {
    bool needs_amplification = false;

    if (auto curse_user = dynamic_cast<CurseUser*>(strongest)) {
        if (auto str = dynamic_cast<Limitless*>(curse_user->GetTechnique())) {
            if (str && str->CheckInfinity()) {
                needs_amplification = true;
            }
        }
    }
    if (needs_amplification) {
        this->SetAmplification(true);
    }
    else if (this->DomainAmplificationActive()) {
        this->SetAmplification(false);
    }
    return needs_amplification;
}

bool Yuta::CanBeHit() const {
    return true;
}
