#include "CharacterAI.h"
#include "Character.h"
#include "Sorcerer.h"
#include "CurseUser.h"
#include "PhysicallyGifted.h"
#include "Techniques.h"
#include "Limitless.h"
#include "Shikigami.h"
#include "Domain.h"
#include "Specials.h"
#include "Specials.h"
#include "CursedTool.h"
#include "BattlefieldHeader.h"
#include "Utils.h"

import std;

void CharacterBrain::ExecuteTurn(Character* user, Battlefield& bf, AIType type) {
    Character* target = GetTarget(user, bf, type);
    if (!target) return;

    if (user->IsaCurseUser()) {
        auto* cu = static_cast<CurseUser*>(user);
        if (cu->IsaSorcerer()) {
            UseRCT(static_cast<Sorcerer*>(cu), type);
        }
        UseReinforcement(cu, type); 
        UseShikigami(cu, type);
        if (TryDomainActions(cu, bf, target, type)) return;
        if (TryTechniqueActions(cu, bf, target, type)) return;
    }
    if (TryInventoryActions(user, target, type)) return;

    user->Attack(target);
}

Character* CharacterBrain::GetTarget(Character* user, Battlefield& bf, AIType type) {
    Character* target = nullptr;
    double best_score = -1.0;

    switch (type) {
    case AIType::Aggressive: {
        for (const auto& ch : bf.battlefield) {
            if (ch.get() == user) continue;

            double score = ch->GetCharacterHealth() / ch->GetCharacterMaxHealth();

            if (ch->IsaCurseUser()) {
                auto target_cu = static_cast<CurseUser*>(ch.get());
                if (target_cu->DomainActive()) score += 1.0;
                if (auto* tech = target_cu->GetTechnique()) {
                    if (tech->IsShrine()) score += 0.55;
                    if (tech->IsLimitless()) score += 0.60;
                }
            }
            else if (ch->IsPhysicallyGifted()) {
                score += 0.5;
            }

            score += GetRandomNumber(-5, 5) * 0.025;
            if (score > best_score) {
                best_score = score;
                target = ch.get();
            }
        }
        break;
    }
    case AIType::Reactive: {
        for (const auto& ch : bf.battlefield) {
            if (ch.get() == user) continue;

            double score = 1.0 - (ch->GetCharacterHealth() / ch->GetCharacterMaxHealth());

            if (ch->IsaCurseUser()) {
                auto target_cu = static_cast<CurseUser*>(ch.get());
                if (target_cu->DomainActive()) score -= 0.5; 
                if (auto* tech = target_cu->GetTechnique()) {
                    if (tech->IsShrine()) score -= 0.5;
                    if (tech->IsLimitless()) score -= 0.2;
                }
            }
            else if (ch->IsPhysicallyGifted()) {
                score += 0.8; 
            }

            score += GetRandomNumber(-5, 5) * 0.025;
            if (score > best_score) {
                best_score = score;
                target = ch.get();
            }
        }
        break;
    }
    case AIType::Randomized: {
        std::vector<Character*> valid_targets;
        for (const auto& ch : bf.battlefield) {
            if (ch.get() != user) {
                valid_targets.push_back(ch.get());
            }
        }
        if (!valid_targets.empty()) {
            target = valid_targets[size_t(GetRandomNumber(0, static_cast<int>(valid_targets.size() - 1)))];
        }
        break;
    }
    }
    return target;
}

void CharacterBrain::UseRCT(Sorcerer* user, AIType type) {
    bool low_hp = !user->HPMoreThanMax(type == AIType::Aggressive ? 0.25 : 0.35);
    bool mid_hp = !user->HPMoreThanMax(type == AIType::Aggressive ? 0.60 : 0.65);

    if (low_hp && user->CEMoreThanMax(type == AIType::Aggressive ? 0.20 : 0.15)) user->BoostRCT();
    else if (mid_hp && user->CEMoreThanMax(0.10)) user->EnableRCT();
    else user->DisableRCT();
}

void CharacterBrain::UseReinforcement(CurseUser* user, AIType type) {
    if (type == AIType::Aggressive) {
        if (user->CEMoreThanMax(0.50)) user->SetCurrentReinforcement(200.0);
        else if (user->CEMoreThanMax(0.30)) user->SetCurrentReinforcement(100.0);
        else if (user->CEMoreThanMax(0.20)) user->SetCurrentReinforcement(50.0);
        else user->SetCurrentReinforcement(0.0);
    }
    else {
        if (!user->HPMoreThanMax(0.35)) user->SetCurrentReinforcement(200.0);
        else if (user->CEMoreThanMax(0.40)) user->SetCurrentReinforcement(100.0);
        else user->SetCurrentReinforcement(0.0);
    }
}

void CharacterBrain::UseShikigami(CurseUser* user, AIType type) {
    for (const auto& shiki : user->GetShikigami()) {
        if (!shiki->IsActive() && user->CEMoreThanMax(0.30)) {
            shiki->Manifest();
        }
        else if (shiki->IsActivePhysically() && !user->CEMoreThanMax(0.15)) {
            shiki->Withdraw();
        }
    }
}

bool CharacterBrain::TryDomainActions(CurseUser* user, Battlefield& bf, Character* target, AIType type) {
    std::vector<CurseUser*> domain_users;

    for (const auto& ch : bf.battlefield) {
        if (ch.get() == user) continue;
        if (!ch->IsaCurseUser()) continue;

        auto* crs = static_cast<CurseUser*>(ch.get());

        if (crs->GetDomain() && crs->DomainActive()) {
            domain_users.push_back(crs);
        }
    }

    if (!domain_users.empty()) {
        if (type == AIType::Aggressive &&
            user->GetDomain() &&
            user->GetCharacterCE() >= user->GetDomain()->GetUseCost() &&
            !user->DomainActive() &&
            !user->IsStrained() &&
            user->GetDomainUses() < 5 &&
            (!user->GetTechnique() || !user->GetTechnique()->BurntOut())) {

            if (domain_users.size() == 1) {
                user->ActivateDomain();
                return true;
            }

            if (GetRandomNumber(1, 100) <= 5) {
                user->ActivateDomain();
                return true;
            }
        }

        if (user->GetCounterDomain() && !user->CounterDomainActive()) {
            user->ActivateCounterDomain();
            if (user->CounterDomainActive()) return true;
        }
    }
    else {
        if (user->CounterDomainActive() && GetRandomNumber(1, 10) >= 6) {
            user->DeactivateCounterDomain();
            return true;
        }

        bool trigger_domain = (type == AIType::Aggressive) ? (GetRandomNumber(1, 100) <= 25) : (!user->HPMoreThanMax(0.40));

        if (trigger_domain &&
            user->GetDomain() &&
            !user->DomainActive() &&
            !user->IsStrained() &&
            user->GetDomainUses() < 5 &&
            (!user->GetTechnique() || !user->GetTechnique()->BurntOut())) {

            user->ActivateDomain();
            return true;
        }
    }

    return false;
}

bool CharacterBrain::TryTechniqueActions(CurseUser* user, Battlefield& bf, Character* target, AIType type) {
    bool target_infinity = false;
    if (target->IsaCurseUser()) {
        auto tr = static_cast<CurseUser*>(target);
        if (auto* tech = tr->GetTechnique()) {
            if (tech->IsLimitless() && tech->IsInfinityActive()) target_infinity = true;
        }
    }

    if (target_infinity) user->SetAmplification(true);
    else if (user->DomainAmplificationActive()) user->SetAmplification(false);

    if (user->GetTechnique() && !user->GetTechnique()->BurntOut()
        && (user->GetTechnique()->Usable() || user->GetTechnique()->Boosted())
        && !user->DomainAmplificationActive()) {

        bool use_tech = (type == AIType::Aggressive) ? user->CEMoreThanMax(0.20) : (!user->HPMoreThanMax(0.50) || user->GetTechnique()->Boosted());

        if (use_tech) {
            user->GetTechnique()->AutoTechniqueUse(user, target, bf);
            return true;
        }
    }
    if (user->GetSpecial() && GetRandomNumber(1, 100) <= 20) {
        user->GetSpecial()->PerformSpecial(user);
    }
    return false;
}

bool CharacterBrain::TryInventoryActions(Character* user, Character* target, AIType type) {
    const auto& inv = user->GetCursedTools();
    auto* tool = user->GetTool();

    if (inv.empty() && !tool) {
        return false;
    }

    bool target_infinity = false;
    if (target->IsaCurseUser()) {
        auto tr = static_cast<CurseUser*>(target);
        if (auto* tech = tr->GetTechnique()) {
            if (tech->IsLimitless() && tech->IsInfinityActive()) target_infinity = true;
        }
    }

    if (target_infinity) {
        if (tool && tool->IsAntiTechniqueWeapon()) return false;

        for (size_t i = 0; i < inv.size(); ++i) {
            if (inv[i]->IsAntiTechniqueWeapon()) {
                user->CursedToolChoice(i + 1);
                return true;
            }
        }
    }
    else if (!inv.empty() && !tool) {
        if (GetRandomNumber(1, 100) <= 50) {
            user->CursedToolChoice(static_cast<size_t>(GetRandomNumber(1, static_cast<int>(inv.size()))));
            return true;
        }
    }
    else if (tool && !inv.empty()) {
        int roll = GetRandomNumber(1, 100);
        if (roll <= 25) {
            user->CursedToolChoice(static_cast<size_t>(GetRandomNumber(1, static_cast<int>(inv.size()))));
            return true;
        }
    }
    return false;
}