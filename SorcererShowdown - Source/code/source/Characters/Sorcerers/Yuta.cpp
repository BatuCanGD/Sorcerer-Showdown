#include "Yuta.h"
#include "Copy.h"
#include "Limitless.h"
#include "Rika.h"
#include "AuthenticMutualLove.h"
#include "SimpleDomain.h"
#include "Katana.h"
#include "Utils.h"

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

    if (!this->HPMoreThanMax(0.40) || rika->IsActivePhysically()) {
        this->BoostRCT();
    }
    else if (!(this->HPMoreThanMax(0.55))) {
        this->EnableRCT();
    }
    else {
        this->DisableRCT();
    }

    double strongesthealth = -1.0;
    Character* strongest = nullptr;
    std::vector<Sorcerer*> domain_users;

    if (!(this->HPMoreThanMax(0.40) || this->CEMoreThanMax(0.20)) && !((rika->GetActiveTime() > 5) && rika->IsActivePhysically())) {
        std::println("Come, Rika.");
        rika->Manifest();
    }

    for (const auto& s : battlefield) {
        if (s.get() == this) continue;
        if (auto sorcerer = dynamic_cast<Sorcerer*>(s.get())) {
            if (sorcerer->DomainActive()) {
                domain_users.push_back(sorcerer);
            }
        }

        double perceived_health = s->GetCharacterHealth() + GetRandomNumber(-100, 100);
        if (!strongest || perceived_health > strongesthealth) {
            strongesthealth = perceived_health;
            strongest = s.get();
        }
    }

    int tntroll = GetRandomNumber(1, 20);
    if (tntroll <= 4) {
        this->Taunt(strongest);
    }

    if (domain_users.size() > 0) {
        if (domain_users.size() == 1 && !this->CounterDomainActive() && (this->GetTechnique()->BurntOut() || this->GetDomainUses() >= 5)) {
            this->ActivateCounterDomain();
            return;
        }
        else if (domain_users.size() == 1 && !this->DomainActive() && !this->GetTechnique()->BurntOut() && this->GetDomainUses() < 5) {
            this->ActivateDomain();
            return;
        }
    }
    else {
        if (this->CounterDomainActive()) this->DeactivateCounterDomain();
    }

    if (strongest) {
        auto* sorcerer_target = dynamic_cast<Sorcerer*>(strongest);
        if (sorcerer_target) {
            auto* limitless = dynamic_cast<Limitless*>(sorcerer_target->GetTechnique());
            if (limitless && limitless->CheckInfinity()) {
                this->SetAmplification(true);
            }
            else {
                this->SetAmplification(false);
            }
        }
        else {
            this->SetAmplification(false);
        }
        this->Attack(strongest);
    }
}

bool Yuta::CanBeHit() const {
    return true;
}
