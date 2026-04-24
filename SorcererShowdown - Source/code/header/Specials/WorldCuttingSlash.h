#pragma once
#include "Specials.h"

class WorldCuttingSlash : public Specials {
public:
	WorldCuttingSlash();
	void PerformSpecial(CurseUser* user) override;
};