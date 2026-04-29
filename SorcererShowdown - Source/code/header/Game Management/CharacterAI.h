#pragma once
#include <vector>
#include <memory>
class Character;
class CurseUser;
class Sorcerer;
struct Battlefield;

struct CharacterBrain {
    enum class AIType { Aggressive, Reactive, Randomized };
    void ExecuteTurn(Character* user, Battlefield& bf, AIType type);
protected:
    Character* GetTarget(Character* user, Battlefield& bf, AIType type);
    void UseRCT(Sorcerer* user, AIType type);
    void UseReinforcement(CurseUser* user, AIType type);
    void UseShikigami(CurseUser* user, AIType type);
    bool TryDomainActions(CurseUser* user, Battlefield& bf, Character* target, AIType type);
    bool TryTechniqueActions(CurseUser* user, Battlefield& bf, Character* target, AIType type);
    bool TryInventoryActions(Character* user, Character* target, AIType type);
};