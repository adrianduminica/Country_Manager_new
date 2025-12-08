#include "Engine.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Country.h"
#include "Province.h"
#include "ResourceStockpile.h"
#include "ProductionLine.h"
#include "Construction.h"

int Engine::run() {
    std::srand((unsigned)std::time(nullptr));

    // --- Romania ---
    Province p1("Wallachia",     1800, 3, 3, 6, 5, 3, 4, 1, 3);
    Province p2("Moldavia",      1500, 2, 2, 5, 4, 2, 3, 1, 2);
    Province p3("Transylvania",  1600, 2, 1, 7, 8, 5, 6, 3, 1);

    Country Romania(
        "Romania",
        "Democratic",
        { p1, p2, p3 },
        ResourceStockpile(0, 100)
    );
    Romania.addProductionLine(EquipmentType::Gun, 2);
    Romania.startFocus(0);
    Romania.addConstruction(BuildingType::Mil, 1);

    // --- Hungary ---
    Province h1("Alfold",        1400, 2, 2, 6, 4, 2, 3, 1, 2);
    Province h2("Transdanubia",  1200, 2, 1, 6, 3, 2, 2, 1, 1);

    Country Hungary(
        "Hungary",
        "Authoritarian",
        { h1, h2 },
        ResourceStockpile(0, 80)
    );
    Hungary.addProductionLine(EquipmentType::Artillery, 1);
    Hungary.startFocus(1);
    Hungary.addConstruction(BuildingType::Civ, 0);

    std::cout << "=== INITIAL STATE ===\n"
              << Romania << "\n"
              << Hungary << "\n";

    for (int d = 1; d <= 40; ++d) {
        Romania.simulateDay();
        Hungary.simulateDay();
    }

    std::cout << "\n=== AFTER 40 DAYS ===\n"
              << Romania << "\n"
              << Hungary << "\n";

    return 0;
}
