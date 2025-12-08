#include "Province.h"
#include <algorithm>
#include <sstream>

Province::Province(std::string name,
                   int pop,
                   int civ,
                   int mil,
                   int infra,
                   int steel,
                   int tungsten,
                   int aluminum,
                   int chromium,
                   int oil)
    : name(std::move(name)),
      population(pop),
      civFactories(civ),
      milFactories(mil),
      infrastructure(infra),
      dockyards(0),
      airfields(0),
      armyRF(0),
      navalRF(0),
      aerialRF(0),
      nuclearRF(0),
      steel(steel),
      tungsten(tungsten),
      aluminum(aluminum),
      chromium(chromium),
      oil(oil) {}

const std::string& Province::getName() const { return name; }
int Province::getPopulation() const { return population; }
int Province::getCiv() const { return civFactories; }
int Province::getMil() const { return milFactories; }
int Province::getInfra() const { return infrastructure; }
int Province::getDockyards() const { return dockyards; }
int Province::getAirfields() const { return airfields; }
int Province::getArmyRF() const { return armyRF; }
int Province::getNavalRF() const { return navalRF; }
int Province::getAerialRF() const { return aerialRF; }
int Province::getNuclearRF() const { return nuclearRF; }

int Province::getSteel() const { return steel; }
int Province::getTungsten() const { return tungsten; }
int Province::getAluminum() const { return aluminum; }
int Province::getChromium() const { return chromium; }
int Province::getOil() const { return oil; }

void Province::addCiv(int x) {
    civFactories = std::max(0, civFactories + x);
}

void Province::addMil(int x) {
    milFactories = std::max(0, milFactories + x);
}

void Province::addInfra(int x) {
    infrastructure = std::min(10, std::max(0, infrastructure + x));
}

void Province::addDockyard(int x) {
    dockyards += x;
}

void Province::addAirfield(int x) {
    airfields += x;
}

std::string Province::toString() const {
    std::ostringstream ss;
    ss << "Province(" << getName() << ") pop=" << getPopulation()
       << ", CIV=" << getCiv()
       << ", MIL=" << getMil()
       << ", INFRA=" << getInfra()
       << ", RES[Steel=" << getSteel()
       << ", Tung=" << getTungsten()
       << ", Alu=" << getAluminum()
       << ", Chr=" << getChromium()
       << ", Oil=" << getOil() << "]"
       << ", DOCKYARDS=" << getDockyards()
       << ", AIRFIELDS=" << getAirfields()
       << ", FACILITIES[Army=" << getArmyRF()
       << ",Naval=" << getNavalRF()
       << ",Aerial=" << getAerialRF()
       << ",Nuclear=" << getNuclearRF() << "]";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Province& p) {
    return os << p.toString();
}
