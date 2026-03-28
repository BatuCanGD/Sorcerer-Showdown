#include "Techniques.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

std::unique_ptr<Technique> Copy::Clone() const {
    auto new_copy = std::make_unique<Copy>();

    for (const auto& tech : copied_techniques) {
        new_copy->copied_techniques.push_back(tech->Clone());
    }

    new_copy->active_copy = this->active_copy;
    new_copy->state = this->state;
    new_copy->chant = this->chant;
    return new_copy;
}

void Copy::Set(Status s) {
    state = s;
    for (auto& tech : copied_techniques) {
        tech->Set(s);
    }
}

void Copy::CopyFrom(Sorcerer* target) {
    if (!target || !target->GetTechnique()) {
        std::println("Nothing to copy!");
        return;
    }
    if (target->IsHeavenlyRestricted()) {
        std::println("{} has no cursed technique to copy!", target->GetName());
        return;
    }
    if (copied_techniques.size() >= max_copies) {
        std::println("Copy limit reached ({})!", max_copies);
        return;
    }
    std::string ttname = target->GetTechnique()->GetTechniqueName();
    for (const auto& tech : copied_techniques) {
        if (tech->GetTechniqueName() == ttname) {
            std::println("You have already copied this technique!");
            return;
        }
    }
    auto cloned = target->GetTechnique()->Clone();
    cloned->Set(this->state);
    std::println("Copied {}'s {}!", target->GetName(), cloned->GetTechniqueName());
    copied_techniques.push_back(std::move(cloned));
}

void Copy::SwitchCopy(int index) {
    if (index < 0 || index >= copied_techniques.size()) {
        std::println("Invalid choice.");
        return;
    }
    active_copy = index;
    std::println("Switched to: {}", copied_techniques[active_copy]->GetTechniqueName());
}

Technique* Copy::GetActive() const {
    if (active_copy < 0 || active_copy >= copied_techniques.size())
        return nullptr;
    return copied_techniques[active_copy].get();
}

std::string Copy::GetTechniqueName() const {
    if (Technique* t = GetActive())
        return std::format("Copy [{}]", t->GetTechniqueName());
    return "Copy [None]";
}

void Copy::Chant() {
    if (Technique* t = GetActive()) t->Chant();
    else std::println("No technique active to chant for!");
}

void Copy::TechniqueMenu(Sorcerer* user, Character* target) {
    if (user->DomainAmplificationActive()) {
        std::println("You cannot use your innate technique due to domain amplification!");
        return;
    }
    Technique* t = GetActive();
    if (!t) {
        std::println("No technique used! Use Technique Settings to copy or switch to one first.");
        return;
    }
    t->TechniqueMenu(user, target);
}

void Copy::TechniqueSetting(Sorcerer* user, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    std::println("=== Copy Technique Settings ===");
    std::println("Active: {}", GetTechniqueName());
    std::println("Stored copies: {}", copied_techniques.size());

    for (size_t i = 0; i < copied_techniques.size(); i++) {
        std::println("  [{}] {}", i, copied_techniques[i]->GetTechniqueName());
    }

    std::println("1 - Copy from a target | 2 - Switch active copy | 3 - Return");
    std::print("=> ");
    int ch = GetValidInput();

    switch (ch) {
    case 1: {
        std::println("Choose a target to copy from:");

        for (size_t i = 0; i < battlefield.size(); ++i) {
            if (battlefield[i].get() == user || battlefield[i]->GetCharacterHealth() <= 0) continue;

            std::println("{} - {}", i, battlefield[i]->GetName());
        }

        std::print("=> ");
        size_t tdex = GetValidInput();
        if (tdex < battlefield.size() && battlefield[tdex].get() != user && battlefield[tdex]->GetCharacterHealth() > 0) {
            this->CopyFrom(battlefield[tdex].get());
        }
        else {
            std::println("Invalid target missed!");
        }
        break;
    }
    case 2: {
        if (copied_techniques.empty()) {
            std::println("No copies to switch to.");
            break;
        }
        std::println("Enter index: ");
        int dex = GetValidInput();
        SwitchCopy(dex);
        break;
    }
    case 3:
        break;
    default:
        std::println("Invalid Input!");
    }
}