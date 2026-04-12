#include "CurseUser.h"
#include "Shikigami.h"
#include "Domain.h"
#include "Techniques.h"
#include "Specials.h"
#include "Utils.h"

import std;


CurseUser::CurseUser(double hp, double ce, double re) : Character(hp, ce, re) {
    current_ce_reinforcement = 50.0;
    max_ce_reinforcement = 200.0;
}
CurseUser::~CurseUser() = default;

bool CurseUser::DomainActive() const {
    return domain_active;
}

Shikigami* CurseUser::ChooseShikigami(size_t index)  const {
    if (index >= 0 && index < shikigami.size()) {
        return shikigami[index].get();
    }
    return nullptr;
}

Domain* CurseUser::GetCounterDomain() const {
    return counter_domain.get();
}

Domain* CurseUser::GetDomain() const {
    return domain.get();
}

Technique* CurseUser::GetTechnique() const {
    return technique.get();
}

Specials* CurseUser::GetSpecial() const {
    return special.get();
}

const std::vector<std::unique_ptr<Shikigami>>& CurseUser::GetShikigami() const {
    return shikigami;
}

void CurseUser::SetAmplification(bool t) {
    domain_amplification_active = t;
}

std::string CurseUser::GetDomainStatus()const {
    if (domain_active) return "\033[35mActive\033[0m";
    else return "\033[2;90mInactive\033[0m";
}
void CurseUser::TickShikigami() {
    std::vector<std::unique_ptr<Character>> empty;
    for (const auto& s : shikigami) {
        s->OnCharacterTurn(this, empty);
    }
}

std::string CurseUser::GetDAstatus() const {
    if (domain_amplification_active) return std::format("{}Active{}", Color::Cyan, Color::Clear);
    return std::format("{}Inactive{}", Color::Red, Color::Clear);
}
std::string CurseUser::GetCounterStatus() const {
    if (counter_domain_active) return std::format("{}Active{}", Color::Purple, Color::Clear);
    return std::format("{}Inactive{}", Color::Red, Color::Clear);
}

std::string CurseUser::GetReinforcementStatus() const {
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
    return std::format("{}{:.1f}/{:.1f}{}", currentcolor, current_ce_reinforcement, max_ce_reinforcement, clear);
}

void CurseUser::SpendCE(double ce) {
    cursed_energy = std::max(cursed_energy - ce, 0.0);
}

void CurseUser::SpendCEdirect(double ce) {
    cursed_energy = std::max(cursed_energy - ce, 0.0);
}

void CurseUser::TickZone() {
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

void CurseUser::TickDomain() {
    if (this->DomainActive()) {
        active_domain_time++;
        if (active_domain_time == max_domain_time) {
            std::println("{}'s domain will end soon", this->GetNameWithID());
        }
        else if (active_domain_time > max_domain_time) {
            std::println("{}'s domain has been deactivated after reaching its timed limit!", this->GetNameWithID());
            this->DeactivateDomain();
            this->GetDomain()->CollapseDomain();
            active_domain_time = 0;
        }
    }
    else if (this->CounterDomainActive()) {
        active_counter_time++;
        if (active_counter_time == max_counter_time) {
            std::println("{}'s {}{}{} is about to shatter", this->GetNameWithID(), Color::Cyan, this->GetCounterDomain()->GetDomainName(), Color::Clear);
        }
        else if (active_counter_time > max_counter_time) {
            std::println("{}'s {}{}{} has been {}shattered{}!", this->GetNameWithID(), Color::Cyan, this->GetCounterDomain()->GetDomainName(), Color::Clear, Color::Red, Color::Clear);
            this->DeactivateCounterDomain();
            counter_on_cooldown = true;
            active_counter_time = 0;
        }
    }
    if (!this->CounterDomainActive() && counter_on_cooldown) {
        counter_recover_time++;
        if (counter_recover_time >= counter_domain_cooldown) {
            counter_on_cooldown = false;
            counter_recover_time = 0;
        }
    }
    else if (!this->CounterDomainActive() && active_counter_time > 0) active_counter_time = 0;
}

bool CurseUser::IsaCurseUser() const {
    return true;
}

void CurseUser::ActivateDomain() {
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
    std::println("\n********{}Domain Expansion{}********\n" "*******{}*******\n", Color::Purple, Color::Clear, this->GetDomain()->GetDomainName());
    if (technique) {
        technique->Set(Technique::Status::DomainBoost);
    }
}

int CurseUser::GetDomainUses() const {
    return total_domain_uses;
}

void CurseUser::DeactivateDomain() {
    domain_active = false;
    is_strained = true;
    active_domain_time = 0;
    if (technique) {
        technique->Set(Technique::Status::BurntOut);
        technique_burnout_time = 0;
    }
}

bool CurseUser::CounterDomainActive() const {
    return counter_domain_active;
}

void CurseUser::ActivateCounterDomain() {
    if (!counter_domain) {
        std::println("{} doesn't have a counter domain!", this->GetNameWithID());
        return;
    }
    if (counter_on_cooldown) {
        std::println("your counter domain is on cooldown, wait for it to recover!");
        return;
    }
    if (!counter_domain_active) {
        counter_domain_active = true;
        std::println("{} activates {}!", this->GetNameWithID(), counter_domain->GetDomainName());
    }
}

void CurseUser::DeactivateCounterDomain() {
    if (!counter_domain) {
        std::println("{} doesn't have a counter domain!", this->GetNameWithID());
        return;
    }
    if (counter_domain_active) {
        counter_domain_active = false;
        std::println("{} deactivated {}!", this->GetNameWithID(), counter_domain->GetDomainName());
    }
}

void CurseUser::DomainDrain() {
    if (DomainActive()) {
        this->SpendCE(this->GetDomain()->GetUseCost());
    }
}
bool CurseUser::DomainAmplificationActive() const {
    return domain_amplification_active;
}
void CurseUser::CleanupShikigami() {
    auto [removed_begin, removed_end] = std::ranges::remove_if(shikigami, [](const auto& s) {
        if (s->GetCharacterHealth() <= 0.0) {
            std::println("{} has been destroyed!", s->GetName());
            return true;
        }
        return false;
        });
    shikigami.erase(removed_begin, removed_end);
}

void CurseUser::RecoverBurnout() {
    if (is_strained) {
        burnout_time++;
        if (burnout_time >= max_technique_burnout_time) {
            is_strained = false;
            burnout_time = 0;
        }
    }
}

void CurseUser::RecoverTechniqueBurnout(Technique* t) {
    if (t == nullptr) return;
    if (t->BurntOut()) {
        technique_burnout_time++;
        if (technique_burnout_time >= max_technique_burnout_time) {
            t->Set(Technique::Status::Usable);
            technique_burnout_time = 0;
            std::println("{}'s {}cursed technique{} has{} recovered from burnout{}!", this->GetNameWithID(), Color::Cyan, Color::Clear, Color::Green, Color::Clear);
        }
    }
    if (technique_burnout_time != 0 && !t->BurntOut()) technique_burnout_time = 0;
}

std::string CurseUser::GetSimpleName() const {
    return "Curse User";
}
std::string CurseUser::GetName() const {
    return "\033[34mCurse User\033[0m";
}
bool CurseUser::CanBeHit() const {
    return true;
}