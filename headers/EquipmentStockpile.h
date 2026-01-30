#ifndef EQUIPMENT_STOCKPILE_H
#define EQUIPMENT_STOCKPILE_H

#include <string>
#include <ostream>

class EquipmentStockpile {
    long long guns = 0;
    long long artillery = 0;
    long long antiAir = 0;
    long long cas = 0;

public:
    void addGuns(long long n);

    void addArtillery(long long n);

    void addAntiAir(long long n);

    void addCAS(long long n);

    long long getGuns() const;

    long long getArtillery() const;

    long long getAntiAir() const;

    long long getCAS() const;

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const EquipmentStockpile &e);

#endif // EQUIPMENT_STOCKPILE_H
