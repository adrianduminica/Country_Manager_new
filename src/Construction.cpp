#include "../headers/Construction.h"
#include "../headers/Utils.h"
#include <sstream>

Construction::Construction(BuildingType type, int provinceIndex, double totalCost)
    : type(type),
      provinceIndex(provinceIndex),
      remainingBP(totalCost),
      totalCost(totalCost) {
}

bool Construction::progress(double dailyBP) {
    remainingBP -= dailyBP;

    remainingBP = GameUtils::ensureRange<double>(remainingBP, 0.0, totalCost);

    return remainingBP <= 0;
}

BuildingType Construction::getType() const { return type; }
int Construction::getProvinceIndex() const { return provinceIndex; }
double Construction::getRemainingBP() const { return remainingBP; }
double Construction::getTotalCost() const { return totalCost; }

std::string Construction::toString() const {
    std::ostringstream ss;
    ss << "[" << static_cast<int>(getType()) << "] "
            << getRemainingBP() << "/" << getTotalCost()
            << " BP left in province " << getProvinceIndex();
    return ss.str();
}
