#include "Domain.h"
#include "Character.h"
#include "Sorcerer.h"

import std;

HollowWickerBasket::HollowWickerBasket() : Domain(150.0, 2, 2) {
    is_neutralizer = true;
}

double HollowWickerBasket::GetUseCost() const {
    return domain_cost;
}

void HollowWickerBasket::OnSureHit(Character&) {}

std::string HollowWickerBasket::GetDomainName() const {
    return "Hollow Wicker Basket";
}