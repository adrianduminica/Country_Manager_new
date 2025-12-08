#ifndef PRODUCTION_LINE_H
#define PRODUCTION_LINE_H

#include <string>
#include <ostream>

enum class EquipmentType { Gun, Artillery, AntiAir, CAS };

class ProductionLine {
    EquipmentType type;
    int factoriesAssigned;
    double unitCost;

    static constexpr double DEF_G   = 10;
    static constexpr double DEF_A   = 50;
    static constexpr double DEF_AA  = 40;
    static constexpr double DEF_CAS = 200;

    static double defCost(EquipmentType t);

public:
    ProductionLine(EquipmentType t, int f, double c);

    EquipmentType getType() const;
    int getFactories() const;
    double getUnitCost() const;

    std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const ProductionLine& p);

#endif // PRODUCTION_LINE_H
