#ifndef COUNTRY_H
#define COUNTRY_H

#include <string>
#include <vector>
#include <ostream>

#include "Province.h"
#include "ResourceStockpile.h"
#include "EquipmentStockpile.h"
#include "ProductionLine.h"
#include "Construction.h"
#include "FocusTree.h"
#include "ProductionQueue.h"

class Country {
    std::string name;
    std::string ideology;
    std::vector<Province> provinces;
    ResourceStockpile resources;
    EquipmentStockpile equipment;
    std::vector<ProductionLine> milLines;
    ProductionQueue<Construction> constructions;
    FocusTree focusTree;

    static constexpr int MIL_OUTPUT = 1000;
    static constexpr int OIL_TO_FUEL = 5;
    static constexpr double CIV_OUTPUT_PER_DAY = 5.0;

public:
    Country(std::string n, std::string id, std::vector<Province> p, ResourceStockpile r);

    Country(const Country &other);

    Country &operator=(const Country &other);

    ~Country() = default;

    int totalCiv() const;

    int totalMil() const;

    int totalOil() const;

    int totalSteel() const;

    int totalTungsten() const;

    int totalAluminum() const;

    int totalChromium() const;

    int getUsedMilFactories() const;

    int getFreeMilFactories() const;

    void modifyLineFactories(int index, int amount);

    long long getEquipmentCount(EquipmentType t) const;

    void addProductionLine(EquipmentType t);

    bool startFocus(int index);

    void addConstruction(BuildingType type, int provinceIndex, int count = 1);

    void simulateDay();

    std::string toString() const;

    const std::string &getName() const { return name; }
    const std::vector<Province> &getProvinces() const { return provinces; }
    const ResourceStockpile &getResourceStockpile() const { return resources; }
    const FocusTree &getFocusTree() const { return focusTree; }
    const ProductionQueue<Construction> &getConstructionQueue() const { return constructions; }
    const std::vector<ProductionLine> &getProductionLines() const { return milLines; }
};

std::ostream &operator<<(std::ostream &os, const Country &c);

#endif
