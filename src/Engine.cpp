#include "Engine.h"
#include "ResourceBase.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Engine::Engine() {
    std::srand((unsigned)std::time(nullptr));
    init();
}

void Engine::init() {
    Province p1("Wallachia",    1800, 3, 3, 6, 5, 3, 4, 1, 3);
    Province p2("Moldavia",     1500, 2, 2, 5, 4, 2, 3, 1, 2);
    Province p3("Transylvania", 1600, 2, 1, 7, 8, 5, 6, 3, 1);

    Country Romania(
        "Romania",
        "Democratic",
        { p1, p2, p3 },
        ResourceStockpile(0, 100)
    );
    Romania.addProductionLine(EquipmentType::Gun, 2);
    Romania.startFocus(0);

    Romania.addConstruction(BuildingType::Mil, 1, 4);
    Romania.addConstruction(BuildingType::Civ, 0, 3);

    Province h1("Alfold",       1400, 2, 2, 6, 4, 2, 3, 1, 2);
    Province h2("Transdanubia", 1200, 2, 1, 6, 3, 2, 2, 1, 1);

    Country Hungary(
        "Hungary",
        "Authoritarian",
        { h1, h2 },
        ResourceStockpile(0, 80)
    );
    Hungary.addProductionLine(EquipmentType::Artillery, 1);
    Hungary.startFocus(1);

    Hungary.addConstruction(BuildingType::Civ, 0, 2);
    Hungary.addConstruction(BuildingType::Infra, 1, 1);

    countries.clear();
    countries.push_back(Romania);
    countries.push_back(Hungary);

    std::cout << "=== INITIAL STATE ===\n";
    for (const auto& c : countries) {
        std::cout << c << "\n";
    }

    std::cout << "Total resource objects: "
              << AbstractResource::getGlobalCount()
              << "\n";
}

void Engine::simulateOneDay() {
    ++day;
    for (auto& c : countries) {
        c.simulateDay();
    }
}
