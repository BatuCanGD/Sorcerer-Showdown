#include "Sukuna.h"
#include "DomainList.h"
#include "Shrine.h"
#include "Limitless.h"
#include "MalevolentShrine.h"
#include "ShikigamiList.h"
#include "WorldCuttingSlash.h"
#include "Utils.h"

#include "PhysicallyGifted.h"


import std;

Sukuna::Sukuna() : Sorcerer(1000.0, 20000.0, 150.0) {
    domain = std::make_unique<MalevolentShrine>();
    counter_domain = std::make_unique<HollowWickerBasket>();
    technique = std::make_unique<Shrine>();
    shikigami.push_back(std::make_unique<Mahoraga>());
    shikigami.push_back(std::make_unique<Agito>());
    special = std::make_unique<WorldCuttingSlash>();
    black_flash_chance = 10;
    rct_skill = RCTProficiency::Expert;
}

std::unique_ptr<Character> Sukuna::Clone() const {
    return std::make_unique<Sukuna>();
}

std::string Sukuna::GetName() const {
    return "\033[31mSukuna\033[0m";
}
std::string Sukuna::GetSimpleName() const {
    return "Sukuna";
}

void Sukuna::OnCharacterTurn(Character* unused, std::vector<std::unique_ptr<Character>>& battlefield) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetNameWithID());
        return;
    }
    if (!this->HPMoreThanMax(0.25) && this->CEMoreThanMax(0.15))
    {
        this->BoostRCT();
    }
    else if (!this->HPMoreThanMax(0.75))
    {
        this->EnableRCT();
    }
    else
    {
        this->DisableRCT();
    }

    if (this->CEMoreThanMax(0.50) || !this->HPMoreThanMax(0.15)) {
        this->SetCurrentReinforcement(200.0);
    }
    else if (this->CEMoreThanMax(0.15)) {
        this->SetCurrentReinforcement(100.0);
    }
    else {
        this->SetCursedEnergyRegen(25.0);
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& target : battlefield) {
        if (target.get() == this || target->GetCharacterHealth() <= 0.0) continue;

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

    
    if (GetRandomNumber(1, 20) <= 11) {
        this->Taunt(strongest);
    }

    Mahoraga* makora = nullptr;
    Agito* agito = nullptr;

    for (const auto& s : shikigami) {
        if (s->GetSimpleName() == "Mahoraga") {
            makora = dynamic_cast<Mahoraga*>(s.get());
        }
        else if (s->GetSimpleName() == "Agito") {
            agito = dynamic_cast<Agito*>(s.get());
        }
    }

    Shrine* shrine = dynamic_cast<Shrine*> (this->GetTechnique());

    if (makora) {
        if (!shrine->WorldCuttingSlashUnlocked()) {
            if (!makora->IsActivePhysically() && this->CEMoreThanMax(0.40)) {
                makora->Manifest();
            }
            else if (!makora->IsActive() && this->CEMoreThanMax(0.025)) {
                makora->PartiallyManifest();
            }
            else if (!this->CEMoreThanMax(0.35)) {
                makora->Withdraw();
            }
        }

        if (makora->FullyAdapted() && !shrine->WorldCuttingSlashUnlocked()) {
            this->GetSpecial()->PerformSpecial(this);
            makora->Withdraw();
            return;
        }
    }

    if (agito && !this->HPMoreThanMax(0.35)) {
        if (!this->CEMoreThanMax(0.30) && agito->IsActive()) {
            agito->Withdraw();
        }
        else if (!agito->IsActive() && this->HPMoreThanMax(0.50) && this->CEMoreThanMax(0.30)) {
            agito->Manifest();
        }
    }

    if (shrine->WorldCuttingSlashUnlocked() && this->CEMoreThanMax(0.125) && GetRandomNumber(1, 100) >= 65) {
        if (makora && makora->IsActive()) {
            makora->Withdraw();
        }
        if (!shrine->FullyChanted()) {
            shrine->Chant();
            return;
        }
        if (shrine->FullyChanted() && this->CEMoreThanMax(0.125)) {
            shrine->UseTheWorldCuttingSlash(this, strongest);
            return;
        }
    }
    if (!domain_users.empty()) {
        if (!shrine->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (domain_users.size() == 1) {
                this->ActivateDomain();
                return;
            }
            else if (GetRandomNumber(1, 100) <= 1) {
                this->ActivateDomain();
                return;
            }
        }
        else if (!(this->CounterDomainActive() && this->DomainActive())) {
            this->ActivateCounterDomain();
            return;
        }
    }
    else {
        if (this->CounterDomainActive()) {
            this->DeactivateCounterDomain();
        }
        if (!shrine->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (GetRandomNumber(1, 100) <= 20) {
                this->ActivateDomain();
                return;
            }
        }
    }

    bool needs_da = false;
    if (auto* cu = dynamic_cast<CurseUser*>(strongest)) {
        if (auto* lim = dynamic_cast<Limitless*>(cu->GetTechnique())) {
            if (lim && lim->CheckInfinity()) needs_da = true;
        }
    }

    if (needs_da) this->SetAmplification(true);
    else if (this->DomainAmplificationActive()) this->SetAmplification(false);

    if (!needs_da) {
        if (GetRandomNumber(1, 100) <= 25 && !shrine->FullyChanted()) {
            shrine->Chant();
            return;
        }
        if (this->CEMoreThanMax(0.050)) {
            if (strongest->GetCharacterHealth() < strongest->GetCharacterMaxHealth() * 0.25 && GetRandomNumber(1, 100) <= 15) {
                shrine->UseShrineTechnique(Shrine::ShrineType::Cleave, this, strongest);
                return;
            }
            else if (strongest->CanBeHit()) {
                shrine->UseShrineTechnique(Shrine::ShrineType::Dismantle, this, strongest);
                return;
            }
        }
    }

    this->Attack(strongest);

    if (needs_da) {
        this->SetAmplification(false);
    }
}

bool Sukuna::CanBeHit() const {
    return true;
}

