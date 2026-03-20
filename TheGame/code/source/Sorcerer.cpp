#include "Sorcerer.h"
#include "Techniques.h"
#include "Utils.h"

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

Specials* Sorcerer::GetSpecial() {
    return special.get();
}

CursedTool* Sorcerer::GetTool() {
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

std::string Sorcerer::GetDAstatus() const {
    if (domain_amplification_active) return "Active";
    else return "Inactive";
}

void Sorcerer::UseRCT() {
    if (this->GetCharacterHealth() >= this->GetCharacterMaxHealth()) {
        return;
    }
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

void Sorcerer::Attack(Character* target) {
    if (cursed_tool) {
        cursed_tool->UseTool(this, target);
        return;
    }
    else if (domain_amplification_active) {
        target->DamageBypass(base_attack_damage);
        std::println("{} landed a strike on {} using domain amplification!", this->GetName(), target->GetName());
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
    active_domain_time = 0;
    if (technique) {
        technique->Set(Technique::Status::BurntOut);
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

void Sorcerer::CursedToolChoice(int c) {
    auto choice = static_cast<CurrentWeapon>(c);
    ChangeCursedTool(choice);
}

void Sorcerer::ChangeCursedTool(CurrentWeapon wep) {
    if (wep == CurrentWeapon::None) {
        if (cursed_tool != nullptr) {
            inventory_curse.push_back(std::move(cursed_tool));
            cursed_tool = nullptr;
            std::println("{} put his weapon away.", this->GetName());
        }
        return;
    }
    for (auto it = inventory_curse.begin(); it != inventory_curse.end(); ++it) {
        bool match = false;

        if (wep == CurrentWeapon::ISOH && dynamic_cast<InvertedSpearofHeaven*>(it->get())) match = true;
        else if (wep == CurrentWeapon::PLCLD && dynamic_cast<PlayfulCloud*>(it->get())) match = true;

        if (match) {
            if (cursed_tool != nullptr) {
                inventory_curse.push_back(std::move(cursed_tool));
            }
            cursed_tool = std::move(*it);
            inventory_curse.erase(it);

            std::println("{} equipped {}!", this->GetName(), cursed_tool->GetName());
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



// ---------------- Gojo -------------------

Gojo::Gojo() : Sorcerer(800.0, 4000.0, 50.0) {
    domain = std::make_unique<InfiniteVoid>();
    technique = std::make_unique<Limitless>();
    SetSixEyes(true);
    black_flash_chance = 15;
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
    else {
        this->DisableRCT();
    }
    double strongesthealth = -1.0;
    Sorcerer* strongest = nullptr;

    for (const auto& target : battlefield) {
        if (target.get() == this) continue;
        if (target->GetCharacterHealth() > strongesthealth) {
            strongesthealth = target->GetCharacterHealth();
            strongest = target.get();
        }
    }
    for (const auto& t : battlefield) {
        if (t->DomainActive() && !this->DomainActive() && !this->GetTechnique()->BurntOut() && this->GetDomainUses() < 5) {
            this->ActivateDomain();
            return;
        }
    }
    auto* limitless = dynamic_cast<Limitless*>(this->GetTechnique());

    if (limitless && strongest && (limitless->Usable() || limitless->Boosted())) {
        int roll = GetRandomNumber(1, 100);

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
    black_flash_chance = 10;
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
    else {
        this->DisableRCT();
    }
    
    double weakesthealth = INT32_MAX;
    Sorcerer* weakest = nullptr;
    std::vector<Sorcerer*> domain_users;
    for (const auto& target : battlefield) {
        if (target.get() == this) continue;
        if (target->DomainActive()) {
            domain_users.push_back(target.get());
        }
        if (target->GetCharacterHealth() < weakesthealth) {
            weakesthealth = target->GetCharacterHealth();
            weakest = target.get();
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
        if (this->GetCharacterHealth() < GetCharacterMaxHealth() * 0.60) {
            if (!makora->IsActivePhysically()) { 
                makora->Manifest();
            }
        }
        else if (!makora->IsActive()) {
            makora->PartiallyManifest();
        }
    }

    if (agito && this->GetCharacterHealth() < this->GetCharacterMaxHealth() * 0.35) {
        if (!agito->IsActive()) {
            agito->Manifest();
        }
    }

    for (const auto& s : shikigami) {
        s->OnShikigamiTurn(this);
    }

    Shrine* shrine = dynamic_cast<Shrine*> (this->GetTechnique());
    int roll = GetRandomNumber(1, 100);

    this->GetSpecial()->PerformSpecial(this);

    if (shrine->WorldCuttingSlashUnlocked()) {
        shrine->WorldCuttingSlashToTarget(this, weakest);
        return;
    }
    if (domain_users.size() < 2 && !this->DomainActive() && !this->GetTechnique()->BurntOut() && this->GetDomainUses() < 5) {
        this->ActivateDomain();
        return;
    }
    if (auto* limitless = dynamic_cast<Limitless*>(weakest->GetTechnique())) {
        if (limitless->CheckInfinity()) {
            this->SetAmplification(true);
            this->Attack(weakest);
            return;
        }
    }
    else {
        this->SetAmplification(false);
    }

    if (weakest->GetCharacterHealth() < GetCharacterMaxHealth() * 0.25 && roll <= 15) {
        shrine->UseShrineTechnique(Shrine::ShrineType::Cleave, this, weakest);
        return;
    }
    else if (weakest->CanBeHit()) {
        shrine->UseShrineTechnique(Shrine::ShrineType::Dismantle, this, weakest);
        return;
    }
}

bool Sukuna::CanBeHit() const {
    return true;
}


// ---------------------- Toji ------------------


Toji::Toji() : Sorcerer(1250.0, -1, -1) {
    inventory_curse.push_back(std::make_unique<InvertedSpearofHeaven>());
    inventory_curse.push_back(std::make_unique<PlayfulCloud>());
    black_flash_chance = 0;
}

std::string Toji::GetName() const {
    return "Toji Fushiguro";
}

void Toji::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    Sorcerer* ignored = nullptr;
    for (const auto& t : battlefield) {
        if (t.get() == this || t->GetCharacterHealth() <= 0.0) continue;

        if (!ignored || t->GetCharacterHealth() == t->GetCharacterPreviousHealth()) {
            ignored = t.get();
        }
    }

    if (this->GetTool() == nullptr) {
        if (auto* limitless = dynamic_cast<Limitless*>(ignored->GetTechnique())) {
            this->CursedToolChoice(INVERTED_SPEAR_OF_HEAVEN);
        }
        else {
            this->CursedToolChoice(PLAYFUL_CLOUD);
        }
    }

    this->GetTool()->UseTool(this, ignored);
}

bool Toji::CanBeHit() const {
    return true;
}


/// for testing stuff, use this class

test_sorcerer::test_sorcerer() : Sorcerer(30000000.0, 30000000.0, 1000.0) {
    technique = std::make_unique<Limitless>();
	domain = std::make_unique<KillEveryoneDomain>();
    black_flash_chance = 100;
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