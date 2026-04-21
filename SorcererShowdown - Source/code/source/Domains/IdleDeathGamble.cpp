#include "IdleDeathGamble.h"
#include "Character.h"
#include "CurseUser.h"
#include "Techniques.h"
#include "Utils.h"

import std;

IdleDeathGamble::IdleDeathGamble() : Domain(800.0, 150.0, 16.0) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitsEveryone;
}
void IdleDeathGamble::SetJackpot(bool t) {
    jackpot = t;
}
void IdleDeathGamble::OnSureHit(CurseUser& user, Character& target) {
    if (jackpot) return;
    target.SetStunState(true);
    if (GetRandomNumber(1, 239) <= luck) {
        std::println("\033[92m!!!!!!!!!!!JACKPOT!!!!!!!!!!\033[0m");
        jackpot = true;
        luck /= 6;
        this->CollapseDomain();
        user.DeactivateDomain();
        user.GetTechnique()->Set(Technique::Status::Usable);
    }
    else {
        luck = std::min(luck * 2, 239);
    }
    std::println("{} got stunned by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
    if (!info_dumped) {
        std::println("\033[92mIDLE DEATH GAMBLE: DOMAIN RULES\033[0m\n"
            "The SureHit: (Rule Transmission) When the domain is activated, the target is immediately stunned. This represents the mandatory information dump where the rules of the game are forced into the target's brain.\n"
            "The Jackpot Roll : Hakari rolls for a Jackpot with a base probability of 1 in 239.\n"
            "The Pity System: (Luck Boost) Every time Hakari fails a roll, his luck value doubles. This continues until the value hits 239, effectively guaranteeing a Jackpot eventually.\n"
            "The Jackpot Reward : Hitting a Jackpot grants Hakari two massive buffs for a limited time :\n"
            "Infinite Energy : Cursed Energy regeneration is boosted by 50x.\n"
            "Automatic RCT : His Reverse Cursed Technique proficiency is set to Absolute.\n"
            "Round Duration : The Jackpot state lasts for exactly 5 turns(ticks).\n"
            "The Reset : Once the 5 turns are up, the Jackpot ends, and Hakari's CE regen and RCT skills revert to their base levels.\n"
            "The Post - Jackpot Penalty : After a successful Jackpot, Hakari's luck is divided by 6, making the next immediate Jackpot significantly harder to hit.\n"
            "Standard Domain Constraints : Cost: Activating the domain costs 25.0 cursed energy.\n");
        info_dumped = true;
    }
}
bool IdleDeathGamble::HasHitJackpot() const {
    return jackpot;
}
std::string IdleDeathGamble::GetDomainName() const {
    return "\033[92mIdle Death Gamble\033[0m";
}
double IdleDeathGamble::GetUseCost() const {
    return domain_cost;
}