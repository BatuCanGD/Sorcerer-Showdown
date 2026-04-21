#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Sorcerer.h"
#include "Domain.h"

class Domain;
class CurseUser;

class test_sorcerer : public Sorcerer {
public:
	test_sorcerer();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Character*, Battlefield& bf) override;
	std::string GetName() const override;
	bool CanBeHit() const override;
};

class KillEveryoneDomain : public Domain {
protected:
	static constexpr double surehit_killdamage = INT32_MAX;
	static constexpr double domain_cost = 5000000.0;
public:
	KillEveryoneDomain();
	void OnSureHit(CurseUser& user, Character& target) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};