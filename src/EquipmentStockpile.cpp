#include "EquipmentStockpile.h"
#include <sstream>

void EquipmentStockpile::addGuns(long long n) {
    if (n > 0) guns += n;
}

void EquipmentStockpile::addArtillery(long long n) {
    if (n > 0) artillery += n;
}

void EquipmentStockpile::addAntiAir(long long n) {
    if (n > 0) antiAir += n;
}

void EquipmentStockpile::addCAS(long long n) {
    if (n > 0) cas += n;
}

long long EquipmentStockpile::getGuns() const {
    return guns;
}

long long EquipmentStockpile::getArtillery() const {
    return artillery;
}

long long EquipmentStockpile::getAntiAir() const {
    return antiAir;
}

long long EquipmentStockpile::getCAS() const {
    return cas;
}

std::string EquipmentStockpile::toString() const {
    std::ostringstream ss;
    ss << "Guns=" << getGuns()
       << ", Artillery=" << getArtillery()
       << ", AA=" << getAntiAir()
       << ", CAS=" << getCAS();
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const EquipmentStockpile& e) {
    return os << e.toString();
}
