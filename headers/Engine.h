#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "Country.h"

class Engine {
    std::vector<Country> countries;
    int day = 0;

public:
    Engine();

    void simulateOneDay();

    const std::vector<Country>& getCountries() const { return countries; }
    int getDay() const { return day; }

private:
    void init();
};

#endif
