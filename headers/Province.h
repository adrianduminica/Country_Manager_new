#ifndef PROVINCE_H
#define PROVINCE_H

#include <string>
#include <ostream>
#include <vector>
#include <memory>

#include "ResourceStockpile.h"
#include "ResourceBase.h"

class Province {
    std::string name;
    int population{};
    int civFactories{};
    int milFactories{};
    int infrastructure{};
    int dockyards{};
    int airfields{};
    int armyRF{};
    int navalRF{};
    int aerialRF{};
    int nuclearRF{};

    int steel{};
    int tungsten{};
    int aluminum{};
    int chromium{};
    int oil{};

    std::vector<std::unique_ptr<AbstractResource> > resources;

    void initResources();

public:
    Province() = default;

    Province(std::string name,
             int pop,
             int civ,
             int mil,
             int infra,
             int steel,
             int tungsten,
             int aluminum,
             int chromium,
             int oil);

    Province(const Province &other);

    Province &operator=(Province other);

    friend void swap(Province &a, Province &b) noexcept {
        using std::swap;
        swap(a.name, b.name);
        swap(a.population, b.population);
        swap(a.civFactories, b.civFactories);
        swap(a.milFactories, b.milFactories);
        swap(a.infrastructure, b.infrastructure);
        swap(a.dockyards, b.dockyards);
        swap(a.airfields, b.airfields);
        swap(a.armyRF, b.armyRF);
        swap(a.navalRF, b.navalRF);
        swap(a.aerialRF, b.aerialRF);
        swap(a.nuclearRF, b.nuclearRF);
        swap(a.steel, b.steel);
        swap(a.tungsten, b.tungsten);
        swap(a.aluminum, b.aluminum);
        swap(a.chromium, b.chromium);
        swap(a.oil, b.oil);
        swap(a.resources, b.resources);
    }

    const std::string &getName() const { return name; }
    int getPopulation() const { return population; }
    int getCiv() const { return civFactories; }
    int getMil() const { return milFactories; }
    int getInfra() const { return infrastructure; }
    int getDockyards() const { return dockyards; }
    int getAirfields() const { return airfields; }
    int getArmyRF() const { return armyRF; }
    int getNavalRF() const { return navalRF; }
    int getAerialRF() const { return aerialRF; }
    int getNuclearRF() const { return nuclearRF; }

    int getSteel() const { return steel; }
    int getTungsten() const { return tungsten; }
    int getAluminum() const { return aluminum; }
    int getChromium() const { return chromium; }
    int getOil() const { return oil; }

    void addCiv(int x);

    void addMil(int x);

    void addInfra(int x);

    void addDockyard(int x);

    void addAirfield(int x);

    void applyResourceEffects(ResourceStockpile &stockpile) const;

    int totalConstructionSlotsFromResources() const;

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const Province &p);

#endif
