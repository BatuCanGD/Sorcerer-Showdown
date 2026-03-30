#pragma once
#include "Techniques.h"

class Copy : public Technique {
private:
	std::vector<std::unique_ptr<Technique>> copied_techniques;
	static constexpr int max_copies = 5;
	int active_copy = -1;
public:
	std::unique_ptr<Technique> Clone() const override;

	void CopyFrom(Sorcerer* target);
	void SwitchCopy(int index);
	Technique* GetActive() const;
	void Set(Status s) override;

	void TechniqueMenu(Sorcerer* user, Character* target, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) override;
	void TechniqueSetting(Sorcerer* user, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) override;
	void Chant() override;
	std::string GetTechniqueName() const override;
};