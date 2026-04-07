#pragma once
#include "Techniques.h"

class CurseUser;
class Character;

class Copy : public Technique {
private:
	std::vector<std::unique_ptr<Technique>> copied_techniques;
	static constexpr int max_copies = 5;
	size_t active_copy;
public:
	std::unique_ptr<Technique> Clone() const override;

	void CopyFrom(CurseUser* target);
	void SwitchCopy(size_t index);
	Technique* GetActive() const;
	void Set(Status s) override;

	void TechniqueMenu(CurseUser* user, Character* target, const std::vector<std::unique_ptr<Character>>& battlefield) override;
	void TechniqueSetting(CurseUser* user, const std::vector<std::unique_ptr<Character>>& battlefield) override;

	void Chant() override;
	std::string GetTechniqueName() const override;
	std::string GetTechniqueSimpleName() const override;
};