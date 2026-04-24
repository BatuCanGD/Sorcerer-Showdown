#include "SimpleDomain.h"
#include "Character.h"

import std;

SimpleDomain::SimpleDomain() : Domain(INT32_MAX, 3, 3) {
    is_neutralizer = true;
    domain_name = "Simple Domain";
    domain_color = "\033[36m";
}
void SimpleDomain::OnSureHit(CurseUser&, Character&) {}
double SimpleDomain::GetUseCost() const {
    return domain_cost;
}
