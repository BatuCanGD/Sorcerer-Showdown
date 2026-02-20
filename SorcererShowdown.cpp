import std;
using namespace std;

class Shikigami;
class Sorcerer;
class Sukuna;
class Gojo;
class Mahoraga;
class Agito;

struct CombatContext;

class Character { // base classes
protected:
	double health;
	double cursed_energy;
	bool is_stunned = false;
	bool is_heavenly_restricted = false;
public:
	Character(double hp, double ce) {
		health = hp; cursed_energy = ce;
	}
	virtual ~Character() = default;

	void Damage(double h) {
		health -= h;
	}
	void Regen(double h) {
		health += h;
	}

	double GetCharacterCE() const {
		return cursed_energy;
	}
	void SpendCE(double c) {
		cursed_energy -= c;
	}
	void RegenCE(double c) {
		cursed_energy += c;
	}

	void SetStunState(bool s) {
		is_stunned = s;
	}
	double GetCharacterHealth() const {
		return health;
	}
	bool IsCharacterStunned() const {
		return is_stunned;
	}
};

class Domain{
protected:
	double domain_health = 200.0;
	const double base_range = 15;
	double current_range = 15;
	bool clashing = false;
public:
	virtual ~Domain() = default;

	bool Clashing() const {
		return clashing;
	}
	void SetClashState(bool a) {
		clashing = a;
	}
	
	double DomainRangeMult()const {
		return current_range / base_range;
	}

	virtual void OnSureHit(Character& target) = 0;
};

class Technique {
public:
	enum class Status {Usable,DomainBoost,BurntOut};
protected:
	const double base_output = 10.0;
	Status state = Status::Usable;
public:
	virtual ~Technique() = default;

	void Set(Status s) { state = s; }
	Status GetStatus() const { return state; }
};

class Limitless : public Technique {
protected:
	enum class TechniqueChants {
		None,
		FirstChant,
		SecondChant,
		ThirdChant,
		FourthChant
	};
};
class Shrine : public Technique {
protected:
	bool world_cutting_slash_allowed = false;
public:
	void SetWCS(bool s) {
		world_cutting_slash_allowed = s;
	}
};

class Sorcerer : public Character{ 
protected:
	unique_ptr<Domain> domain = nullptr;
	unique_ptr<Technique> technique = nullptr;
	vector<unique_ptr<Shikigami>> shikigami;

	bool domain_active = false;
	int domain_limit = 5;

	bool domain_amplification_active = false;
	enum class ReverseCT {
		Disabled,
		Active,
		Overdrive
	};
	ReverseCT rct_state = ReverseCT::Disabled;
public:
	Sorcerer(double hp, double ce) 
		: Character(hp, ce) {}
	
	bool DomainActive() const {
		return domain_active;
	}
	Domain* GetDomain() {
		return domain.get();
	}
	Technique* GetTechnique() {
		return technique.get();
	}
	const vector<unique_ptr<Shikigami>>& GetShikigami() const {
		return shikigami;
	}

	void DisableRCT() {
		rct_state = ReverseCT::Disabled;
	}
	void EnableRCT() {
		rct_state = ReverseCT::Active;
	}
	void BoostRCT() {
		rct_state = ReverseCT::Overdrive;
	}

	void DeactivateDomain() {
		domain_active = false;
	}
	void ActivateDomain() {
		domain_active = true;
	}

	virtual ~Sorcerer() = default;
};


class InfiniteVoid : public Domain { // domains
protected:
	static constexpr double surehit_braindamage = 30.0;
public:
	void OnSureHit(Character& target) {
		if (clashing) return;
		target.Damage(surehit_braindamage * DomainRangeMult());
		target.SetStunState(true);
	}
};
class MalevolentShrine : public Domain {
protected:
	static constexpr double surehit_slashdamage = 75.0;
public:
	void OnSureHit(Character& target) {
		if (clashing) return;
		target.Damage(surehit_slashdamage * DomainRangeMult());
	}
};

class Shikigami : public Character {
protected:
	int active_turn_amount = 0;

	enum class ShikigamiStatus {
		Shadow,
		PartialManifestation,
		Manifested
	};
	ShikigamiStatus shikigami_stats = ShikigamiStatus::Shadow;

public:
	Shikigami(double hp, double ce) : Character(hp, ce) {}

	void PartiallyManifest() {
		shikigami_stats = ShikigamiStatus::PartialManifestation;
	}
	void Manifest() {
		shikigami_stats = ShikigamiStatus::Manifested;
	}
	void Withdraw() {
		shikigami_stats = ShikigamiStatus::Shadow;
	}

	void ActiveTimeIncrementor() {
		if (!IsActive()) return;
		active_turn_amount++;
	}

	bool IsActive() const {
		return shikigami_stats == ShikigamiStatus::PartialManifestation ||
				shikigami_stats == ShikigamiStatus::Manifested;
	}
};

class Mahoraga : public Shikigami {
public:
	Mahoraga() : Shikigami(500.0, 1000.0) {}
protected:
	enum class InfinityAdaptation {
		None,
		FirstSpin,
		SecondSpin,
		ThirdSpin,
		FourthSpin
	};
	InfinityAdaptation InfStage = InfinityAdaptation::None;
public:

	void Adapt() {
		if (!IsActive()) return;

		if (active_turn_amount >= 50) {
			InfStage = InfinityAdaptation::FourthSpin;
		}
		else if(active_turn_amount >= 40){
			InfStage = InfinityAdaptation::ThirdSpin;
		}
		else if (active_turn_amount >= 25) {
			InfStage = InfinityAdaptation::SecondSpin;
		}
		else if (active_turn_amount >= 10) {
			InfStage = InfinityAdaptation::FirstSpin;
		}
		else {
			InfStage = InfinityAdaptation::None;
		}
	}

	bool FullyAdaptedToInfinity()const{
		return InfStage == InfinityAdaptation::FourthSpin;
	}

};

class Agito : public Shikigami {
public:
	Agito() : Shikigami(200.0, 1000.0) {}
protected:
	const double passive_heal_amount = 10.0;
	const double passive_regen_amount = 20.0;
public:
	void PassiveSupport() {
		
	}
};

class Gojo : public Sorcerer { // fighters
public:
	Gojo() : Sorcerer(400.0, 4000.0) {
		domain = make_unique<InfiniteVoid>();
		technique = make_unique<Limitless>();
	}
};

class Sukuna : public Sorcerer { 
protected:
	enum class Understanding {
		None,
		DomainAmplification,
		DomainAmpAndMahoraga
	};
	Understanding techknow = Understanding::None;
public:
	Sukuna() : Sorcerer(800.0, 12000.0) {
		domain = make_unique<MalevolentShrine>();
		technique = make_unique<Shrine>();
		shikigami.push_back(make_unique<Mahoraga>());
		shikigami.push_back(make_unique<Agito>());
	}
};

struct CombatContext {
	void WorldCuttingSlashReady(Mahoraga& m, Sorcerer* user) {
		const auto& shikigami_list = user->GetShikigami();
		for (const auto& s : shikigami_list) {
			Mahoraga* m = dynamic_cast<Mahoraga*>(s.get());
			if (m != nullptr && m->FullyAdaptedToInfinity()) {
				Shrine* shrinePtr = dynamic_cast<Shrine*>(user->GetTechnique());
				if (shrinePtr != nullptr) {
					shrinePtr->SetWCS(true);
					println("The blueprint is complete. World Cutting Slash enabled!");
				}
				return;
			}
		}
	}
};



int main() { // main
	Gojo gojo;
	Sukuna sukuna;
	return 0;
}