#include "ResourceStockpile.h"
#include <sstream>

ResourceStockpile::ResourceStockpile()
    : fuel(0), manpower(0) {}

ResourceStockpile::ResourceStockpile(int fuel, int manpower)
    : fuel(fuel), manpower(manpower) {}

int ResourceStockpile::getFuel() const {
    return fuel;
}

int ResourceStockpile::getManpower() const {
    return manpower;
}

void ResourceStockpile::add(int dFuel, int dManpower) {
    fuel     = clampNonNeg(fuel     + dFuel);
    manpower = clampNonNeg(manpower + dManpower);
}

std::string ResourceStockpile::toString() const {
    std::ostringstream ss;
    ss << "Fuel=" << getFuel() << ", Manpower=" << getManpower();
    return ss.str();
}


std::ostream& operator<<(std::ostream& os, const ResourceStockpile& r) {
    return os << r.toString();
}
