#ifndef COUNTRY_BUILDER_H
#define COUNTRY_BUILDER_H

#include "Country.h"
#include <string>
#include <vector>


class CountryBuilder {
    std::string name;
    std::string ideology;
    std::vector<Province> provinces;
    ResourceStockpile resources;

public:
    CountryBuilder() : resources(0, 0) {
    }

    CountryBuilder &setName(const std::string &n) {
        name = n;
        return *this;
    }

    CountryBuilder &setIdeology(const std::string &id) {
        ideology = id;
        return *this;
    }

    CountryBuilder &setStockpile(int fuel, int manpower) {
        resources = ResourceStockpile(fuel, manpower);
        return *this;
    }

    CountryBuilder &addProvince(const Province &p) {
        provinces.push_back(p);
        return *this;
    }


    Country build() {
        return Country(name, ideology, provinces, resources);
    }
};

#endif // COUNTRY_BUILDER_H
