#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>

class Character;

struct Battlefield {
	std::vector<std::unique_ptr<Character>> battlefield;
	std::vector<std::unique_ptr<Character>> spawn_queue;
	std::map<std::string, int> fighter_counts;
};