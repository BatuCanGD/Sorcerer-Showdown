#include "Shikigami.h"
#include "Sorcerer.h"

import std;

// --- Base Shikigami  ---

Shikigami::Shikigami(double hp, double ce, double re) : Character(hp, ce, re) {
}

void Shikigami::PartiallyManifest() {
    shikigami_state = State::Partial;
}

void Shikigami::Manifest() {
    shikigami_state = State::Full;
}

void Shikigami::Withdraw() {
    shikigami_state = State::Shadow;
}

void Shikigami::ActiveTimeIncrementor() {
    if (!IsActive()) return;
    active_turn_amount++;
}

bool Shikigami::IsActive() const {
    return shikigami_state == State::Partial ||
            shikigami_state == State::Full;
}

int Shikigami::GetActiveTime() const {
    return active_turn_amount;
}

bool Shikigami::IsPartiallyActive() const {
    return shikigami_state == State::Partial;
}

bool Shikigami::IsActivePhysically() const {
    return shikigami_state == State::Full;
}

std::string Shikigami::GetShikigamiStatus() const {
    switch (shikigami_state) {
    case State::Shadow: return "Dormant";
    case State::Partial: return "Ability Active";
    case State::Full: return "Physically Manifested";
    default: return "Dormant";
    }
}

std::string Shikigami::GetName() const {
    return "Shikigami";
}

bool Shikigami::CanBeHit() const {
    return true;
}