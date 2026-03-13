#include "Sorcerer.h"
#include "Technique.h"
#include <print>


bool Sorcerer::DomainActive() const {
    return domain_active;
}

Domain* Sorcerer::GetDomain() {
    return domain.get();
}

Technique* Sorcerer::GetTechnique() {
    return technique.get();
}

CombatContext* Sorcerer::GetSpecial() {
    return special.get();
}

const std::vector<std::unique_ptr<Shikigami>>& Sorcerer::GetShikigami() const {
    return shikigami;
}

void Sorcerer::SetAmplification(bool t) {
    domain_amplification_active = t;
}

void Sorcerer::CheckSpecial(Sorcerer* sorcerer) {
    if (GetSpecial() == nullptr) return;

    auto wcs = dynamic_cast<WorldCuttingSlash*>(special.get());


    if (auto* limitless = dynamic_cast<Limitless*>(technique.get())) {
        
        std::println("no limitless special yet");
    }
    else if (auto* shrine = dynamic_cast<Shrine*>(technique.get())) {
        if (wcs) {
            wcs->WorldCuttingSlashReady(sorcerer);
        }
    }
}

void Sorcerer::SetSixEyes(bool t) {
    six_eyes = t;
}

bool Sorcerer::HasSixEyes() const {
    return six_eyes;
}

void Sorcerer::SpendCE(double ce) {
    if (HasSixEyes()) {
        cursed_energy -= ce * 0.10;
    }
    else {
        cursed_energy -= ce;
    }
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

void Sorcerer::DeactivateDomain() {
    domain_active = false;
    if (technique) {
        technique->Set(Technique::Status::BurntOut);
    }
}

void Sorcerer::ActivateDomain(Character* user) {
    if (total_domain_uses >= domain_limit) {
        user->Damage(50.0);
        user->SetStunState(true);
        std::println("You have overused your domain! You take 50 damage and are stunned for the next turn.");
        return;
    }
    domain_active = true;
    if (technique) {
        technique->Set(Technique::Status::DomainBoost);
    }
    total_domain_uses++;
}

std::string Sorcerer::GetName() const {
    return "Sorcerer";
}

void Sorcerer::OnSorcererTurn() {
    std::println("override this");
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

// ---------------- Gojo -------------------

Gojo::Gojo() : Sorcerer(800.0, 4000.0, 50.0) {
    domain = std::make_unique<InfiniteVoid>();
    technique = std::make_unique<Limitless>();
    SetSixEyes(true);
}

std::string Gojo::GetName() const {
    return "Gojo";
}

void Gojo::OnSorcererTurn() {
    std::println("go/jo");
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
}

std::string Sukuna::GetName() const {
    return "Sukuna";
}

void Sukuna::OnSorcererTurn() {
    std::println("fraudkuna");
}

bool Sukuna::CanBeHit() const {
    return true;
}

/// for testing stuff, use this class

test::test() : Sorcerer(9999999.9, 999999.9, 1000.0) {
}

void test::OnSorcererTurn() {
    std::println("I'm the strongest dude");
}
std::string test::GetName() const {
    return "John Zenin";
}

bool test::CanBeHit() const {
    return true;
}