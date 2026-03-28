#include "SimpleDomain.h"
#include "Character.h"

import std;

SimpleDomain::SimpleDomain() : Domain(INT32_MAX, 3, 3) {
    is_neutralizer = true;
}

void SimpleDomain::OnSureHit(Character&) {}

std::string SimpleDomain::GetDomainName() const {
    return "Simple Domain";
}

double SimpleDomain::GetUseCost() const {
    return domain_cost;
}
