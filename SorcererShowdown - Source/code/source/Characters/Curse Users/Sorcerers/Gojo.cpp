#include "Gojo.h"
#include "BattlefieldHeader.h"
#include "Limitless.h"
#include "InfiniteVoid.h"
#include "UnlimitedPurple.h"
#include "SimpleDomain.h"
#include "Utils.h"

#include "PhysicallyGifted.h"

import std;

Gojo::Gojo() : Sorcerer(800.0, 4000.0, 150.0) {
    domain = std::make_unique<InfiniteVoid>();
    counter_domain = std::make_unique<SimpleDomain>();
    technique = std::make_unique<Limitless>();
    special = std::make_unique<UnlimitedPurple>();
    SetSixEyes(true);
    black_flash_chance = 15;
    rct_skill = RCTProficiency::Absolute;
}
std::unique_ptr<Character> Gojo::Clone() const {
    return std::make_unique<Gojo>();
}

std::string Gojo::GetName() const {
    return "\033[96mGojo\033[0m";
}
std::string Gojo::GetSimpleName() const {
    return "Gojo";
}

void Gojo::OnCharacterTurn(Character*, Battlefield& bf) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetNameWithID());
        return;
    }
    auto* limitless = dynamic_cast<Limitless*>(this->GetTechnique());
    if (!limitless->CheckInfinity() && this->CEMoreThanMax(0.03) && !limitless->BurntOut()) {
        limitless->SetInfinity(true);
    }
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
    
    if (this->CEMoreThanMax(0.70) || (limitless->UnlimitedHollowAllowed() && limitless->FullyChanted()) || !this->HPMoreThanMax(0.25)) {
        this->SetCurrentReinforcement(200.0);
    }
    else if (this->CEMoreThanMax(0.30)) {
        this->SetCurrentReinforcement(100.0);
    }
    else if (this->CEMoreThanMax(0.20)) {
        this->SetCurrentReinforcement(50.0);
    }
    else {
        this->SetCurrentReinforcement(0.0);
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;
    bool shrine_found = false;

    for (const auto& target : bf.battlefield) {
        if (target.get() == this) continue;

        double hp_ratio = target->GetCharacterHealth() / this->GetCharacterMaxHealth();
        double score = hp_ratio;

        if (auto curse_user = dynamic_cast<CurseUser*>(target.get())) {
            if (curse_user->DomainActive()) {
                domain_users.push_back(curse_user);
                score += 0.50;
            }
            if (curse_user->GetTechnique()) {
                std::string tech_name = curse_user->GetTechnique()->GetTechniqueSimpleName();
                if (tech_name == "Shrine") {
                    score += 1.0;
                }
                if (tech_name == "Limitless") {
                    auto* limitless = dynamic_cast<Limitless*>(curse_user->GetTechnique());
                    score += (limitless && !limitless->CheckInfinity()) ? 0.30 : 0.15;
                }
            }
        }
        else if (auto physically_gifted = dynamic_cast<PhysicallyGifted*>(target.get())) {
            score += 0.25;
        }
        score += GetRandomNumber(-5, 5) * 0.01;

        if (score > best_score) {
            best_score = score;
            strongest = target.get();
        }

    }
    int tntroll = GetRandomNumber(1, 100);

    if (tntroll <= 45) {
        this->Taunt(strongest);
    }

    if ((limitless->GetUsedRedAmount() >= 15 && limitless->GetUsedBlueAmount() >= 15 && limitless->GetUsedPurpleAmount() >= 5) && tntroll >= 70) {
        if (!limitless->UnlimitedHollowAllowed()) {
            this->GetSpecial()->PerformSpecial(this);
            return;
        }
    }
    
    if (!domain_users.empty()) {
        if (!limitless->BurntOut() && this->GetDomainUses() < 6 && !this->DomainActive()) {
            if (domain_users.size() == 1) {
                this->ActivateDomain(); 
                return;
            }
            else if (GetRandomNumber(1, 100) <= 1) {
                this->ActivateDomain();
                return;
            }
        }
        else if (!(this->CounterDomainActive() && this->DomainActive()) && !this->counter_on_cooldown) {
            this->ActivateCounterDomain();
            return;
        }
    }
    else {
        if (this->CounterDomainActive()) {
            this->DeactivateCounterDomain();
        }
        if (!limitless->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (GetRandomNumber(1, 100) <= 30) {
                this->ActivateDomain();
                return;
            }
        }
    }
    if (strongest && 
        (limitless->Usable() || limitless->Boosted()) && 
        (this->GetCharacterCE() >= this->GetCharacterMaxCE() * 0.2)) {

        CurseUser* cur = dynamic_cast<CurseUser*>(strongest);
        if (cur) {
            auto* target_tech = cur->GetTechnique();
            if (target_tech && target_tech->GetTechniqueSimpleName() == "Limitless") {
                bool checked = InfCheck(strongest);
                if (checked) {
                    HitCharacter(strongest);
                    return;
                }
            }
        }

        int roll = GetRandomNumber(1, 100);
        int croll = GetRandomNumber(1, 10);

        if ((croll <= 4 && !limitless->FullyChanted()) || (limitless->UnlimitedHollowAllowed() && !limitless->FullyChanted())) {
            limitless->Chant();
            return;
        }
        if (limitless->FullyChanted() && limitless->UnlimitedHollowAllowed()) {
            limitless->UseUnlimitedHollowPurple(this, bf);
            return;
        }

        if (roll <= 15 && this->CEMoreThanMax(0.75)) {
            limitless->UsePurple(this, strongest);
        }
        else if (roll <= 60) {
            limitless->UseBlue(this, strongest);
        }
        else {
            limitless->UseRed(this, strongest);
        }
        return;
    }
    HitCharacter(strongest);
}

bool Gojo::InfCheck(Character* strongest) {
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

void Gojo::HitCharacter(Character* strongest) {
    this->Attack(strongest);
    if (this->DomainAmplificationActive()) this->SetAmplification(false);
}

bool Gojo::CanBeHit() const {
    auto* limitless = dynamic_cast<Limitless*>(technique.get());
    if (limitless->CheckInfinity()) {
        return false;
    }
    return true;
}
