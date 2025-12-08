#include "ProductionLine.h"
#include <algorithm>
#include <sstream>

double ProductionLine::defCost(EquipmentType t) {
    switch (t) {
        case EquipmentType::Gun:       return DEF_G;
        case EquipmentType::Artillery: return DEF_A;
        case EquipmentType::AntiAir:   return DEF_AA;
        case EquipmentType::CAS:       return DEF_CAS;
    }
    return DEF_G;
}

ProductionLine::ProductionLine(EquipmentType t, int f, double c)
    : type(t),
      factoriesAssigned(std::max(0, f)),
      unitCost(c > 0 ? c : defCost(t)) {}

EquipmentType ProductionLine::getType() const { return type; }
int ProductionLine::getFactories() const { return factoriesAssigned; }
double ProductionLine::getUnitCost() const { return unitCost; }

std::string ProductionLine::toString() const {
    const char* tn =
        (type == EquipmentType::Gun)       ? "Gun" :
        (type == EquipmentType::Artillery) ? "Artillery" :
        (type == EquipmentType::AntiAir)   ? "AA" : "CAS";

    std::ostringstream ss;
    ss << tn << " [factories=" << getFactories()
       << ", unitCost=" << getUnitCost() << "]";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const ProductionLine& p) {
    return os << p.toString();
}
