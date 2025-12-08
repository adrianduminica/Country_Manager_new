#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "Country.h"

class Engine {
public:
    Engine();

    void init();

    void simulateOneDay();

    int getDay() const;

    std::vector<Country>&       getCountries();
    const std::vector<Country>& getCountries() const;

private:
    std::vector<Country> countries;
    int day;
};

#endif // ENGINE_H
