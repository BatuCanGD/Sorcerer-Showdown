#include "Sorcerer.h"
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

const vector<unique_ptr<Shikigami>>& Sorcerer::GetShikigami() const {
    return shikigami;
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
        println("You have overused your domain! You take 50 damage and are stunned for the next turn.");
        return;
    }
    domain_active = true;
    if (technique) {
        technique->Set(Technique::Status::DomainBoost);
    }
    total_domain_uses++;
}

string Sorcerer::GetName() const {
    return "Sorcerer";
}

void Sorcerer::OnSorcererTurn() {
    println("override this");
}

// ---------------- Gojo -------------------

Gojo::Gojo() : Sorcerer(800.0, 4000.0) {
    domain = make_unique<InfiniteVoid>();
    technique = make_unique<Limitless>();
}

string Gojo::GetName() const {
    return "Gojo";
}

void Gojo::OnSorcererTurn() {
    println("go/jo");
}

// ---------------- Sukuna -------------------

Sukuna::Sukuna() : Sorcerer(1000.0, 3000.0) {
    domain = make_unique<MalevolentShrine>();
    technique = make_unique<Shrine>();
	shikigami.push_back(make_unique<Mahoraga>());
	shikigami.push_back(make_unique<Agito>());
}

string Sukuna::GetName() const {
    return "Sukuna";
}

void Sukuna::OnSorcererTurn() {
    println("fraudkuna");
}