#ifndef RESOURCE_STOCKPILE_H
#define RESOURCE_STOCKPILE_H

#include <string>
#include <ostream>

class ResourceStockpile {
    int fuel;
    int manpower;

    static int clampNonNeg(int x) { return x < 0 ? 0 : x; }

public:
    ResourceStockpile();
    ResourceStockpile(int fuel, int manpower);

    int  getFuel() const;
    int  getManpower() const;

    void add(int dFuel, int dManpower);
    std::string toString() const;
};

// operator global de inserare în stream
std::ostream& operator<<(std::ostream& os, const ResourceStockpile& r);

#endif // RESOURCE_STOCKPILE_H
