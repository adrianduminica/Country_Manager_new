#ifndef PROVINCE_H
#define PROVINCE_H

#include <string>
#include <ostream>

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

    const std::string& getName() const;
    int getPopulation() const;
    int getCiv() const;
    int getMil() const;
    int getInfra() const;
    int getDockyards() const;
    int getAirfields() const;
    int getArmyRF() const;
    int getNavalRF() const;
    int getAerialRF() const;
    int getNuclearRF() const;

    int getSteel() const;
    int getTungsten() const;
    int getAluminum() const;
    int getChromium() const;
    int getOil() const;

    void addCiv(int x);
    void addMil(int x);
    void addInfra(int x);
    void addDockyard(int x);
    void addAirfield(int x);

    std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const Province& p);

#endif // PROVINCE_H
