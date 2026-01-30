#include "../headers/Engine.h"
#include "../headers/CountryBuilder.h"
#include "../headers/ResourceBase.h"
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

    CountryBuilder roBuilder;
    Country Romania = roBuilder
                        .setName("Romania")
                        .setIdeology("Democratic")
                        .setStockpile(0, 100)
                        .addProvince(p1)
                        .addProvince(p2)
                        .addProvince(p3)
                        .build();

    Romania.addProductionLine(EquipmentType::Gun, 2);

    Province h1("Alfold",       1400, 2, 2, 6, 4, 2, 3, 1, 2);
    Province h2("Transdanubia", 1200, 2, 1, 6, 3, 2, 2, 1, 1);

    CountryBuilder huBuilder;
    Country Hungary = huBuilder
                        .setName("Hungary")
                        .setIdeology("Authoritarian")
                        .setStockpile(0, 80)
                        .addProvince(h1)
                        .addProvince(h2)
                        .build();

    Hungary.addProductionLine(EquipmentType::Artillery, 1);

    countries.clear();
    countries.push_back(Romania);
    countries.push_back(Hungary);

    std::cout << "=== INITIAL STATE ===\n";
}

void Engine::simulateOneDay() {
    ++day;
    for (auto& c : countries) {
        c.simulateDay();
    }
}