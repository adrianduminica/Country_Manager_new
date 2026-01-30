#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include <string>

enum class BuildingType {
    Civ,
    Mil,
    Infra,
    Dockyard,
    Airfield,
    ArmyRF,
    NavalRF,
    AerialRF,
    NuclearRF
};

class Construction {
    BuildingType type;
    int provinceIndex;
    double remainingBP;
    double totalCost;

public:
    Construction(BuildingType type, int provinceIndex, double totalCost);

    bool progress(double dailyBP);

    BuildingType getType() const;

    int getProvinceIndex() const;

    double getRemainingBP() const;

    double getTotalCost() const;

    std::string toString() const;
};

#endif // CONSTRUCTION_H
