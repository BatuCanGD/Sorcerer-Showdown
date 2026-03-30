#include "Limitless.h"
#include "Sorcerer.h"
#include "Techniques.h"
#include "Specials.h"
#include "UnlimitedPurple.h"
#include "Utils.h"

import std;

void UnlimitedPurple::PerformSpecial(Sorcerer* user) {
	if (Limitless* limitless = dynamic_cast<Limitless*>(user->GetTechnique())) {
		if (limitless->GetUsedBlueAmount() >= 10 && limitless->GetUsedRedAmount() >= 10 && limitless->GetUsedPurpleAmount() >= 3) {
			if (!limitless->UnlimitedHollowAllowed()) {
				std::println("\n{}=== UNLIMITED HOLLOW PURPLE IS NOW POSSIBLE! ==={}",Color::Purple,Color::Clear);
				limitless->SetUnlimitedHollow(true);
			}
		}
		else {
			std::println("\n{}Unlimited Hollow Purple fails to manifest, the technique of the user needs to be used more!{}",Color::DimGray,Color::Clear);
		}
	}
}

std::string UnlimitedPurple::GetSpecialSimplifiedName() const {
	return "\033[35mUnlimited Purple\033[0m";
}