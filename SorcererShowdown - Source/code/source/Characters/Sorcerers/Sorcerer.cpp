#include "Sorcerer.h"
#include "Limitless.h"
#include "Techniques.h"
#include "Shikigami.h"
#include "Domain.h"
#include "CursedTool.h"
#include "Utils.h"
#include "Specials.h"

import std;

Sorcerer::~Sorcerer() = default;

Sorcerer::Sorcerer(double hp, double ce, double re) : Character(hp, ce, re) {
    global_id_counter++;
    unique_id = global_id_counter;
}

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

const std::vector<std::unique_ptr<Shikigami>>& Sorcerer::GetShikigami() const {
    return shikigami;
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

std::string Sorcerer::GetDomainStatus()const{
    if (domain_active) return "\033[35mActive\033[0m";
    else return "\033[2;90mInactive\033[0m";
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
    case ReverseCT::Disabled: return "\033[31mDisabled\033[0m";
    case ReverseCT::Active: return "\033[33mActive\033[0m";
    case ReverseCT::Overdrive: return "\033[32mOverdrive\033[0m";
    default: return "\033[2;90mDisabled\033[0m";
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
    std::vector<std::unique_ptr<Character>> empty;
    for (const auto& s : shikigami) {
        s->OnCharacterTurn(this, empty);
    }
}

std::string Sorcerer::GetDAstatus() const {
    if (domain_amplification_active) return std::format("{}Active{}", Color::Cyan, Color::Clear);
    return std::format("{}Inactive{}", Color::Red, Color::Clear); 
}
std::string Sorcerer::GetCounterStatus() const {
    if (counter_domain_active) return std::format("{}Active{}", Color::Purple, Color::Clear);
    return std::format("{}Inactive{}", Color::Red, Color::Clear);
}

std::string Sorcerer::GetReinforcementStatus() const {
    std::string currentcolor = Color::Yellow;
    std::string clear = Color::Clear;

    if (current_ce_reinforcement < 50.0) {
        currentcolor = Color::Red;
    }
    else if (current_ce_reinforcement < 100.0) {
        currentcolor = Color::Yellow;
    }
    else if (current_ce_reinforcement < 150.0) {
        currentcolor = Color::Green;
    }
    else if (current_ce_reinforcement < 200.0) {
        currentcolor = Color::Blue;
    }
    else {
        currentcolor = Color::Purple;
    }
    return std::format("{}{:.1f}/{:.1f}{}",currentcolor,current_ce_reinforcement,max_ce_reinforcement, clear);
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
                std::println("{}'s attack was blocked by {}'s {}Infinity{}!", this->GetNameWithID(), target_sorcerer->GetNameWithID(),Color::Cyan,Color::Clear);
                return;
            }
        }
    }
    if (domain_amplification_active) {
        double ce_addon = std::sqrt(std::max(0.0, this->GetCharacterCE())) * 0.633;
        double amp_damage = base_attack_damage + ce_addon;

        target->DamageBypass(amp_damage);
        std::println("{} landed a strike on {} using {}domain amplification{}!", this->GetNameWithID(), target->GetNameWithID(),Color::Yellow,Color::Clear);
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
        std::println("\n*** {}BLACK FLASH!{} ***", Color::Red, Color::Clear);
        std::println("{} landed a {}BlackFlash{} on {}!", this->GetNameWithID(), Color::Red, Color::Clear, target->GetNameWithID());
    }
    else {
        std::println("{} landed a {}heavy strike{} on {}!", this->GetNameWithID(),Color::BrightRed,Color::Clear, target->GetNameWithID());
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
            std::println("{}'s {}{}{} is about to shatter", this->GetName(),Color::Cyan, this->GetCounterDomain()->GetDomainName(),Color::Clear);
        }
        else if (active_counter_time > max_counter_time) {
            std::println("{}'s {}{}{} has been {}shattered{}!", this->GetName(), Color::Cyan, this->GetCounterDomain()->GetDomainName(),Color::Clear, Color::Red, Color::Clear);
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
        std::println("Your technique is {}burnt out{} and cannot be used yet", Color::Red, Color::Clear);
        return;
    }
    else if (total_domain_uses >= domain_limit) {
        this->DamageBypass(50.0);
        this->SetStunState(true);
        std::println("{}You have overused your domain! You take 50 damage and are stunned for the next turn.{}", Color::Red, Color::Clear);
        return;
    }
    domain_active = true;
    total_domain_uses++;
    std::println("\n********{}Domain Expansion{}********\n" "*******{}*******\n", Color::Purple, Color::Clear,this->GetDomain()->GetDomainName());
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

bool Sorcerer::IsaSorcerer()const {
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
            std::println("{}'s {}cursed technique{} has{} recovered from burnout{}!", this->GetName(),Color::Cyan,Color::Clear, Color::Green, Color::Clear);
        }
    }
    if (technique_burnout_time != 0 && !t->BurntOut()) technique_burnout_time = 0;
}

std::string Sorcerer::GetSimpleName() const {
    return "Sorcerer";
}

std::unique_ptr<Character> Sorcerer::Clone() const {
    return nullptr;
}