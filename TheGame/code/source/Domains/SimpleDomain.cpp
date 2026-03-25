#include "Domain.h"
#include "Character.h"
#include "Sorcerer.h"

import std;

SimpleDomain::SimpleDomain() : Domain(std::numeric_limits<double>::infinity(), 3, 3) {
    is_neutralizer = true;
}

void SimpleDomain::OnSureHit(Character&) {}

std::string SimpleDomain::GetDomainName() const {
    return "Simple Domain";
}

double SimpleDomain::GetUseCost() const {
    return domain_cost;
}
