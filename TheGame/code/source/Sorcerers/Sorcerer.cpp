#include "Sorcerer.h"
#include "Techniques.h"
#include "Shikigami.h"
#include "Domain.h"
#include "CursedTool.h"
#include "Utils.h"
#include "Specials.h"

import std;

Sorcerer::~Sorcerer() = default;

Sorcerer::Sorcerer(double hp, double ce, double re) : Character(hp, ce, re) {}

bool Sorcerer::DomainActive() const {
    return domain_active;
}

Shikigami* Sorcerer::ChooseShikigami(size_t index)  const {
    if (index >= 0 && index < shikigami.size()) {
        return shikigami[index].get();
    }
    return nullptr; 
}

Domain* Sorcerer::GetCounterDomain() const {
    return counter_domain.get();
}

Domain* Sorcerer::GetDomain() const {
    return domain.get();
}

Technique* Sorcerer::GetTechnique() const {
    return technique.get();
}

Specials* Sorcerer::GetSpecial() const {
    return special.get();
}

CursedTool* Sorcerer::GetTool() const {
    return cursed_tool.get();
}

const std::vector<std::unique_ptr<Shikigami>>& Sorcerer::GetShikigami() const {
    return shikigami;
}
const std::vector<std::unique_ptr<CursedTool>>& Sorcerer::GetCursedTools() const {
    return inventory_curse;
}

void Sorcerer::SetAmplification(bool t) {
    domain_amplification_active = t;
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
    cursed_energy = std::max(cursed_energy - (ce * efficiency), 0.0);
}

void Sorcerer::SpendCEdirect(double ce) {
    cursed_energy = std::max(cursed_energy - ce , 0.0);
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
    default: return "Active";
    }
}

void Sorcerer::TickZone() {
    if (!technique) return;

    if (!this->DomainActive() && this->GetTechnique()->GetStatus() == Technique::Status::DomainBoost) {
        the_zone_time++;
        if (the_zone_time > 3) {
            this->GetTechnique()->Set(Technique::Status::Usable);
            the_zone_time = 0;
        }
    }
    else {
        the_zone_time = 0;
    }
}

void Sorcerer::TickShikigami() {
    for (const auto& s : shikigami) {
        s->OnShikigamiTurn(this);
    }
}

std::string Sorcerer::GetDAstatus() const {
    if (domain_amplification_active) return "Active";
    else return "Inactive";
}

void Sorcerer::UseRCT() {
    if (this->GetCharacterHealth() >= this->GetCharacterMaxHealth()) {
        return;
    }
    const double default_regen = 50.0;
    const double overdrive_regen = 100.0;
    if (rct_state == ReverseCT::Active) {
        this->Regen(default_regen);
        this->SpendCE(default_regen * 2);
    }
    else if (rct_state == ReverseCT::Overdrive) {
        this->Regen(overdrive_regen);
        this->SpendCE(overdrive_regen * 2);
    }
}

void Sorcerer::Attack(Character* target) {
    Sorcerer* target_sorcerer = dynamic_cast<Sorcerer*>(target);
    if (target_sorcerer) {
        if (Limitless* limitless = dynamic_cast<Limitless*>(target_sorcerer->GetTechnique())) {
            if (limitless->CheckInfinity() && !this->DomainAmplificationActive()) {
                std::println("{}'s attack was blocked by {}'s Infinity!", this->GetName(), target->GetName());
                return;
            }
        }
    }
    if (domain_amplification_active) {
        double ce_addon = std::sqrt(std::max(0.0, this->GetCharacterCE())) * 0.633;
        double amp_damage = base_attack_damage + ce_addon;

        target->DamageBypass(amp_damage);
        std::println("{} landed a strike on {} using domain amplification!", this->GetName(), target->GetName());
        return;
    }
    else if (cursed_tool) {
        cursed_tool->UseTool(this, target);
        return;
    }


    bool is_black_flash = false;
    if (!this->IsHeavenlyRestricted() && GetRandomNumber(1, 100) <= black_flash_chance) {
        is_black_flash = true;
        if (this->technique) {
            technique->Set(Technique::Status::DomainBoost);
        }
    }

    double final_damage = base_attack_damage * (is_black_flash ? black_flash_multiplier : 1.0);
    target->Damage(final_damage);

    if (is_black_flash) {
        std::println("\n*** BLACK FLASH! ***");
        std::println("{} landed a BlackFlash on {}!", this->GetName(), target->GetName());
    }
    else {
        std::println("{} landed a heavy strike on {}!", this->GetName(), target->GetName());
    }
}

void Sorcerer::TickDomain() {
    if (this->DomainActive()) {
        active_domain_time++;
        if (active_domain_time == max_domain_time) {
            std::println("{}'s domain will end soon", this->GetName());
        }
        else if (active_domain_time > max_domain_time) {
            std::println("{}'s domain has been deactivated after reaching its timed limit!", this->GetName());
            this->DeactivateDomain();
            this->GetDomain()->CollapseDomain();
            active_domain_time = 0;
        }
    }
    else if (this->CounterDomainActive()) {
        active_counter_time++;
        if (active_counter_time == max_counter_time) {
            std::println("{}'s {} is about to shatter", this->GetName(), this->GetCounterDomain()->GetDomainName());
        }
        else if (active_counter_time > max_counter_time) {
            std::println("{}'s {} has been shattered", this->GetName(), this->GetCounterDomain()->GetDomainName());
            this->DeactivateCounterDomain();
            active_counter_time = 0;
        }
    }
    if (!this->CounterDomainActive() && active_counter_time > 0) active_counter_time = 0;
}

void Sorcerer::ActivateDomain() {
    if (!domain) {
        std::println("You don't have a domain to activate!");
        return;
	}
    else if (domain_active) {
        std::println("Your domain is already active!");
        return;
	}
    else if (is_strained) {
        std::println("Your technique is burnt out and cannot be used yet");
        return;
    }
    else if (total_domain_uses >= domain_limit) {
        this->DamageBypass(50.0);
        this->SetStunState(true);
        std::println("You have overused your domain! You take 50 damage and are stunned for the next turn.");
        return;
    }
    domain_active = true;
    total_domain_uses++;
    std::println("\n********Domain Expansion********\n*******{}*******\n", this->GetDomain()->GetDomainName());
    if (technique) {
        technique->Set(Technique::Status::DomainBoost);
    }
}

int Sorcerer::GetDomainUses() const {
    return total_domain_uses;
}

void Sorcerer::DeactivateDomain() {
    domain_active = false;
    is_strained = true;
    active_domain_time = 0;
    if (technique) {
        technique->Set(Technique::Status::BurntOut);
        technique_burnout_time = 0;
    }
}

bool Sorcerer::CounterDomainActive() const {
    return counter_domain_active;
}

void Sorcerer::ActivateCounterDomain() {
    if (!counter_domain) {
        std::println("{} doesn't have a counter domain!", this->GetName());
        return;
    }
    if (!counter_domain_active) {
        counter_domain_active = true;
        std::println("{} activates {}!", this->GetName(), counter_domain->GetDomainName());
    }
}

void Sorcerer::DeactivateCounterDomain() {
    if (!counter_domain) {
        std::println("{} doesn't have a counter domain!", this->GetName());
        return;
    }
    if (counter_domain_active) {
        counter_domain_active = false;
        std::println("{} deactivated {}!", this->GetName(), counter_domain->GetDomainName());
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
        this->SpendCE(this->GetDomain()->GetUseCost());
    }
}

bool Sorcerer::CanBeHit() const {
    return true;
}

bool Sorcerer::DomainAmplificationActive() const {
    return domain_amplification_active;
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

void Sorcerer::RecoverBurnout() {
    if (is_strained) {
        burnout_time++;
        if (burnout_time >= max_technique_burnout_time) {
            is_strained = false;
            burnout_time = 0;
        }
    }
}

void Sorcerer::RecoverTechniqueBurnout(Technique* t) {
    if (t == nullptr) return;
    if (t->BurntOut()) {
        technique_burnout_time++;
        if (technique_burnout_time >= max_technique_burnout_time) {
            t->Set(Technique::Status::Usable);
            technique_burnout_time = 0;
            std::println("{}'s cursed technique has recovered from burnout!", this->GetName());
        }
    }
    if (technique_burnout_time != 0 && !t->BurntOut()) technique_burnout_time = 0;
}

void Sorcerer::CursedToolChoice(int choice) {
    if (choice == 0) {
        if (cursed_tool != nullptr) {
            std::println("{} put {} away.", this->GetName(), cursed_tool->GetName());
            inventory_curse.push_back(std::move(cursed_tool));
            cursed_tool = nullptr;
        }
        return;
    }

    int inv_index = choice - 1;
    if (inv_index >= 0 && inv_index < inventory_curse.size()) {
        if (cursed_tool != nullptr) {
            inventory_curse.push_back(std::move(cursed_tool));
        }
        cursed_tool = std::move(inventory_curse[inv_index]);
        inventory_curse.erase(inventory_curse.begin() + inv_index);

        std::println("{} equipped {}!", this->GetName(), cursed_tool->GetName());
    }
    else {
        std::println("Invalid tool choice.");
    }
}

void Sorcerer::EquipToolByName(const std::string& weaponName) {
    for (int i = 0; i < inventory_curse.size(); ++i) {
        if (inventory_curse[i]->GetName() == weaponName) {
            CursedToolChoice(i + 1); 
            return;
        }
    }
}


void Sorcerer::Taunt(Character* taunted) { // pure aura
    if (!taunted) return;
    const double healthy_threshold = 0.70;
    const double injured_threshold = 0.40;
    const double critical_threshold = 0.20;

    int taunt_type = GetRandomNumber(1, 4);

    if (this->GetCharacterHealth() > (this->GetCharacterMaxHealth() * healthy_threshold)) {
        switch (taunt_type) {
        case 1:
            std::println("I'm surprised you've even managed to scratch me this much {}!", taunted->GetName());
            break;
        case 2:
            std::println("Is that all you've got, {}? I expected more from you!", taunted->GetName());
            break;
        case 3:
            std::println("You're not even worth my time, {}!", taunted->GetName());
            break;
        default:
            std::println("You should just give up now, {}!", taunted->GetName());
        }
    }
    else if (this->GetCharacterHealth() > (this->GetCharacterMaxHealth() * injured_threshold)) {
        switch (taunt_type) {
        case 1:
            std::println("You're starting to annoy me, {}. Keep it up and I'll make you regret it!", taunted->GetName());
            break;
        case 2:
            std::println("You're not doing too bad, {}. But don't get too confident just yet!", taunted->GetName());
            break;
        case 3:
            std::println("Huh, you're actually putting up a fight, {}. I might have to take you more seriously!", taunted->GetName());
            break;
        default:
            std::println("You're not bad, {}. But I'm still better!", taunted->GetName());

        }
    }
    else if (this->GetCharacterHealth() > (this->GetCharacterMaxHealth() * critical_threshold)) {
        switch (taunt_type) {
        case 1:
            std::println("You're really starting to piss me off, {}. I'll make you regret your actions!", taunted->GetName());
            break;
        case 2:
            std::println("You're actually pretty strong, but it won't be enough to defeat me, {}!", taunted->GetName());
            break;
        case 3:
            std::println("You're really starting to get on my nerves, {}. I might have to end this quickly!", taunted->GetName());
            break;
        default:
            std::println("You think a few hits will stop me, {}? Im just getting warmed up!", taunted->GetName());
        }
    }
    else {
        switch (taunt_type) {
        case 1:
            std::println("You think this is over, {}? I'll drag you to the grave with me!", taunted->GetName());
            break;
        case 2:
            std::println("Blood for blood, {}! You won't leave this place alive!", taunted->GetName());
            break;
        case 3:
            std::println("You're really starting to piss me off, {}. I might have to end this quickly!", taunted->GetName());
            break;
        default:
            std::println("I'll make you wish you were never born {}!", taunted->GetName());
        }
    }
}