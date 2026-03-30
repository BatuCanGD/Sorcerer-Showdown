#include "Gojo.h"
#include "Limitless.h"
#include "InfiniteVoid.h"
#include "SimpleDomain.h"
#include "Utils.h"

import std;

Gojo::Gojo() : Sorcerer(800.0, 4000.0, 40.0) {
    domain = std::make_unique<InfiniteVoid>();
    counter_domain = std::make_unique<SimpleDomain>();
    technique = std::make_unique<Limitless>();
    SetSixEyes(true);
    black_flash_chance = 15;
}
std::unique_ptr<Sorcerer> Gojo::Clone() const {
    return std::make_unique<Gojo>();
}

std::string Gojo::GetName() const {
    return "\033[96mGojo\033[0m";
}

void Gojo::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetName());
        return;
    }
    auto* limitless = dynamic_cast<Limitless*>(this->GetTechnique());
    if ((!this->HPMoreThanMax(0.35) && this->CEMoreThanMax(0.20)) || !limitless->CheckInfinity()) 
    {
        this->BoostRCT();
    }
    else if (!this->HPMoreThanMax(0.75) && this->CEMoreThanMax(0.20))
    {
        this->EnableRCT();
    }
    else 
    {
        this->DisableRCT();
    }
    double strongesthealth = -1.0;
    Sorcerer* strongest = nullptr;
    std::vector<Sorcerer*> domain_users;

    for (const auto& target : battlefield) {
        if (target.get() == this) continue;
        double perceived_health = target->GetCharacterHealth() + GetRandomNumber(-100, 100);
        if (perceived_health > strongesthealth || !strongest) {
            strongesthealth = perceived_health;
            strongest = target.get();
        }
        if (target->DomainActive()) {
            domain_users.push_back(target.get());
        }
    }

    int tntroll = GetRandomNumber(1, 20);
    if (tntroll <= 9) {
        this->Taunt(strongest);
    }

    int droll = GetRandomNumber(1, 100);
    if (droll <= 30 && domain_users.empty() && !limitless->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
        this->ActivateDomain();
        return;
    }
    else if (domain_users.size() == 1 && !limitless->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
        this->ActivateDomain();
        return;
    }
    else if (!domain_users.empty() && (limitless->BurntOut() || this->GetDomainUses() >= 5) && !this->CounterDomainActive()) {
        this->ActivateCounterDomain();
        return;
    }
    else if (domain_users.empty() && this->CounterDomainActive()) {
        this->DeactivateCounterDomain();
        return;
    }
 
    if (limitless && strongest && (limitless->Usable() || limitless->Boosted()) && this->GetCharacterCE() >= this->GetCharacterMaxCE() * 0.2) {
        int roll = GetRandomNumber(1, 100);
        int croll = GetRandomNumber(1, 10);

        if (croll <= 4 && limitless->GetChantPower() < 3.0) {
            limitless->Chant();
            return;
        }

        if (roll <= 15 && this->CEMoreThanMax(0.75)) {
            limitless->UseTheLimitlessTechnique(Limitless::LimitlessType::Purple, this, strongest);
        }
        else if (roll <= 60) {
            limitless->UseTheLimitlessTechnique(Limitless::LimitlessType::Blue, this, strongest);
        }
        else {
            limitless->UseTheLimitlessTechnique(Limitless::LimitlessType::Red, this, strongest);
        }
        return;
    }
    else if (strongest) {
        Limitless* str = dynamic_cast<Limitless*>(strongest->GetTechnique());

        if (str && str->CheckInfinity()) SetAmplification(true);
        else SetAmplification(false);

        this->Attack(strongest);
    }

}

bool Gojo::CanBeHit() const {
    auto* limitless = dynamic_cast<Limitless*>(technique.get());
    if (limitless->CheckInfinity()) {
        return false;
    }
    return true;
}
