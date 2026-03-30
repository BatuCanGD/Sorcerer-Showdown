#pragma once
#include <vector>
#include <memory>

class Sorcerer;

class UserInterface {
public:
	void ShowBattleEntry(const std::vector<std::unique_ptr<Sorcerer>>& battlefield);
	void DisplaySorcererStatus(Sorcerer* s);
	static void ContinuePrompt();
	static void ClearScreen();
};