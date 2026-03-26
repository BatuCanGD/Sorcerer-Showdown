#include "Sorcerer.h"
#include "Techniques.h"
#include "Shikigami.h"
#include "Domain.h"
#include "Utils.h"

import std;

Gojo::Gojo() : Sorcerer(800.0, 4000.0, 40.0) {
    domain = std::make_unique<InfiniteVoid>();
    counter_domain = std::make_unique<SimpleDomain>();
    technique = std::make_unique<Limitless>();
    SetSixEyes(true);
    black_flash_chance = 15;
}

std::string Gojo::GetName() const {
    return "Gojo";
}

void Gojo::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetName());
        return;
    }
    auto* limitless = dynamic_cast<Limitless*>(this->GetTechnique());
    if ((this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.25 && \
         this->GetCharacterCE() > this->GetCharacterMaxCE() * 0.25) || \
        !limitless->CheckInfinity()) 
    {
        this->BoostRCT();
    }
    else if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.65 && \
        !(this->GetCharacterCE() <= this->GetCharacterMaxCE() * 0.10)) 
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
        if (target->GetCharacterHealth() > strongesthealth) {
            strongesthealth = target->GetCharacterHealth();
            strongest = target.get();
        }
        if (target->DomainActive()) {
            domain_users.push_back(target.get());
        }
    }

    int droll = GetRandomNumber(1, 100);
    if (droll <= 30 && domain_users.empty() && !limitless->BurntOut() && this->GetDomainUses() < 5) {
        this->ActivateDomain();
        return;
    }
    else if (domain_users.size() == 1 && !limitless->BurntOut() && this->GetDomainUses() < 5) {
        this->ActivateDomain();
        return;
    }
    else if (!domain_users.empty() && (limitless->BurntOut() || this->GetDomainUses() >= 5)) {
        this->ActivateCounterDomain();
        return;
    }
    else {
        if (this->CounterDomainActive()) this->DeactivateCounterDomain();
    }
 
    if (limitless && strongest && (limitless->Usable() || limitless->Boosted()) && this->GetCharacterCE() >= this->GetCharacterMaxCE() * 0.2) {
        int roll = GetRandomNumber(1, 100);
        int croll = GetRandomNumber(1, 10);

        if (croll <= 4 && limitless->GetChantPower() < 3.0) {
            limitless->Chant();
            return;
        }

        if (roll <= 15 && this->GetCharacterCE() > 3000) {
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
