#include "../headers/ProductionLine.h"
#include <cmath>

ProductionLine::ProductionLine(EquipmentType t, int f, double eff)
    : type(t), factories(f), efficiency(eff) {
    switch (type) {
        case EquipmentType::Gun: unitCost = 0.5;
            break;
        case EquipmentType::Artillery: unitCost = 3.5;
            break;
        case EquipmentType::AntiAir: unitCost = 4.0;
            break;
        case EquipmentType::CAS: unitCost = 12.0;
            break;
    }
}

EquipmentType ProductionLine::getType() const { return type; }
int ProductionLine::getFactories() const { return factories; }
double ProductionLine::getEfficiency() const { return efficiency; }
double ProductionLine::getUnitCost() const { return unitCost; }

void ProductionLine::setFactories(int count) {
    factories = count;
}

long long ProductionLine::calculateDailyOutput() const {
    double baseOutput = 1000.0;
    return static_cast<long long>(std::floor((factories * baseOutput) / unitCost));
}
