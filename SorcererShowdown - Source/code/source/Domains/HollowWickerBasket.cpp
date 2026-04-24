#include "HollowWickerBasket.h"
#include "Character.h"

import std;

HollowWickerBasket::HollowWickerBasket() : Domain(150.0, 2, 2) {
    is_neutralizer = true;
    domain_name = "Hollow Wicker Basket";
    domain_color = "\033[95m";
}
double HollowWickerBasket::GetUseCost() const {
    return domain_cost;
}
void HollowWickerBasket::OnSureHit(CurseUser&, Character&) {}