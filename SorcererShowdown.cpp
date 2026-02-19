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
	bool is_stunned = false;
public:
	Character(double hp) {
		health = hp;
	}
	virtual ~Character() = default;

	void Damage(double h) {
		health -= h;
	}
	void Regen(double h) {
		health += h;
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

class Sorcerer : public Character{ 
protected:
	double cursed_energy;
	
	unique_ptr<Domain> domain;
	unique_ptr<Technique> technique;

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
	Sorcerer(double hp, double ce) : Character(hp) {
		cursed_energy = ce;
	}
	
	bool DomainActive() const {
		return domain_active;
	}
	Domain* GetDomain() {
		return domain.get();
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


	double GetSorcererCE() const {
		return cursed_energy;
	}
	void SpendCE(double c) {
		cursed_energy -= c;
	}
	void RegenCE(double c) {
		cursed_energy += c;
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
	enum class ShikigamiStatus {
		Shadow,
		PartialManifestation,
		Manifested
	};
	ShikigamiStatus shikigami_stats = ShikigamiStatus::Shadow;
public:
	Shikigami(double hp) : Character(hp) {}

	void PartiallyManifest() {
		shikigami_stats = ShikigamiStatus::PartialManifestation;
	}
	void Manifest() {
		shikigami_stats = ShikigamiStatus::Manifested;
	}
	void Withdraw() {
		shikigami_stats = ShikigamiStatus::Shadow;
	}

	bool IsActive() const {
		return shikigami_stats == ShikigamiStatus::PartialManifestation ||
				shikigami_stats == ShikigamiStatus::Manifested;
	}
};

class Mahoraga : public Shikigami {
public:
	Mahoraga() : Shikigami(500.0) {}
};
class Agito : public Shikigami {
public:
	Agito() : Shikigami(200.0) {}
};



class Gojo : public Sorcerer { // fighters
public:
	Gojo() : Sorcerer(400.0, 4000.0) {
		domain = make_unique<InfiniteVoid>();
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
	}
};

int main() { // main
	Gojo gojo;
	Sukuna sukuna;
	Mahoraga mahoraga;
	Agito agito;
	return 0;
}