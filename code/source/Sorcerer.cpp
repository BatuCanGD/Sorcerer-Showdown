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

bool Sorcerer::IsDomainActive() const {
    return domain_active;
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

bool Sorcerer::CanBeHit() const {
    return true;
}

// ---------------- Gojo -------------------

Gojo::Gojo() : Sorcerer(800.0, 4000.0, 50.0) {
    domain = std::make_unique<InfiniteVoid>();
    technique = std::make_unique<Limitless>();
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

Sukuna::Sukuna() : Sorcerer(1000.0, 12000.0, 100.0) {
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

test::test() : Sorcerer(154.1, 154.1, 2.5) {
}

void test::OnSorcererTurn() {
    std::println("test");
}
std::string test::GetName() const {
    return "test";
}

bool test::CanBeHit() const {
    return true;
}