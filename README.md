# Sorcerer Showdown

A turn-based, tactical, fan-made fighting game written in modern C++23, inspired by the combat mechanics and power systems of *Jujutsu Kaisen*. 

This project simulates complex sorcery battles, featuring an extensible AI system, resource management (Cursed Energy), and intricate mechanics like Domain Expansions and Shikigami.

## Features

* **Modern C++23:** Utilizes the latest C++ standards, including C++20/23 modules (`import std;`), smart pointers (`std::unique_ptr`), and modern memory management.
* **Extensible Framework:** Built with a highly modular Object-Oriented architecture. The core `Sorcerer` and `Character` base classes make it incredibly easy to inherit and implement new fighters, cursed techniques, and shikigami.
* **Not so Basic Turn-Based AI:** Characters evaluate the battlefield dynamically. AI opponents will calculate health thresholds to trigger RCT, count active domains to avoid multi-clash burnout, and strategically target the strongest threats.
* **Authentic Combat Mechanics:** * Cursed Energy (CE) resource management.
  * Domain Expansions, Simple Domains, and Domain Amplification.
  * Unique character kits (e.g., Six Eyes efficiency, copying techniques, manifesting Shikigami with turn limits).

## Architecture 

The game is structured to allow rapid expansion. Key components include:

* `Sorcerer.h` / `Character.h`: The foundation for all entities, handling health, CE, and basic attacks.
* `Techniques.h`: Modular abilities that can be attached to any sorcerer.
* `Domain.h`: Handles the logic for environmental overrides, clashes, and burnout timers.
* `Shikigami.h`: Independent entities that have their own turn logic and manifestation cooldowns.

## Building & Requirements
Compiler: A C++23 compliant compiler (MSVC 19.34+, GCC 13+, or Clang 16+).

Modules: Ensure your build system (like CMake or VS) is configured to handle import std;.

Standard: Set your language standard to c++23 or /std:c++latest.

## Adding New Characters

```h
// First, create a header file with any name you want
// like MyNewSorcerer.h
// And then using classes Create your custom sorcerer class inheriting from sorcerer

#include "Sorcerer.h" // this is required to compile properly and inherit from the sorcerer class
#include <string> // std::string     |
#include <vector> // std::vector     |   these 3 are needed for the OnSorcererTurn and GetName functions
#include <memory> // std::unique_ptr |

class MyNewSorcerer : public Sorcerer {
public:
    MyNewSorcerer(); // constructor needed to set base stats
    std::string GetName() const override; // required for the name displayed ingame
    void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) override; // required for AI turn logic but can be left empty
};
```

```cpp
  #include "MyNewSorcerer.h"
  // In the CPP file you created for your character
  import std; // you will need this
  // Initialize stats (HP, CE, Regen)


  MyNewSorcerer::MyNewSorcerer() : Sorcerer(1000.0, 5000.0, 100.0) {
      technique = std::make_unique<CustomTechnique>(); // you can enter any of the existing domains or techniques
      domain = std::make_unique<CustomDomain>(); //       make an op or extremely weak character
      black_flash_chance = 10; //                         or add on top of whatever you'd like
  }

  // Define the Character's Name
  std::string MyNewSorcerer::GetName() const { return "Custom Fighter"; }

  // Define the AI Strategy
  void MyNewSorcerer::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
      // AI logic goes here (Targeting, RCT usage, Domain activation)
      // Similar to the logic found in Sukuna.cpp or Gojo.cpp
      // You can leave it empty if the character is only controlled by the player
  }

```

