#pragma once
#include "Specials.h"

class UnlimitedPurple : public Specials {
public:
	UnlimitedPurple();
	void PerformSpecial(CurseUser* user) override;
};