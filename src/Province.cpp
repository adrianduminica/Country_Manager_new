#include "Province.h"
#include <sstream>
#include <algorithm>

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
      steel(steel),
      tungsten(tungsten),
      aluminum(aluminum),
      chromium(chromium),
      oil(oil)
{
    if (population < 0)    population = 0;
    if (civFactories < 0)  civFactories = 0;
    if (milFactories < 0)  milFactories = 0;
    if (infrastructure < 0) infrastructure = 0;
    if (infrastructure > 10) infrastructure = 10;

    if (steel < 0)    this->steel    = 0;
    if (tungsten < 0) this->tungsten = 0;
    if (aluminum < 0) this->aluminum = 0;
    if (chromium < 0) this->chromium = 0;
    if (oil < 0)      this->oil      = 0;

    initResources();
}

Province::Province(const Province& other)
    : name(other.name),
      population(other.population),
      civFactories(other.civFactories),
      milFactories(other.milFactories),
      infrastructure(other.infrastructure),
      dockyards(other.dockyards),
      airfields(other.airfields),
      armyRF(other.armyRF),
      navalRF(other.navalRF),
      aerialRF(other.aerialRF),
      nuclearRF(other.nuclearRF),
      steel(other.steel),
      tungsten(other.tungsten),
      aluminum(other.aluminum),
      chromium(other.chromium),
      oil(other.oil)
{
    resources.clear();
    resources.reserve(other.resources.size());
    for (const auto& resPtr : other.resources) {
        resources.push_back(resPtr->clone());
    }
}

Province& Province::operator=(Province other) {
    swap(*this, other);
    return *this;
}

void Province::initResources() {
    resources.clear();

    if (steel > 0)
        resources.push_back(std::make_unique<MaterialResource>("Steel", steel));
    if (aluminum > 0)
        resources.push_back(std::make_unique<MaterialResource>("Aluminum", aluminum));
    if (tungsten > 0)
        resources.push_back(std::make_unique<MaterialResource>("Tungsten", tungsten));
    if (chromium > 0)
        resources.push_back(std::make_unique<MaterialResource>("Chromium", chromium));

    if (oil > 0)
        resources.push_back(std::make_unique<DailyOutputResource>("Oil", oil, 5));

    if (civFactories > 0)
        resources.push_back(std::make_unique<ConstructionResource>("Civ factories", civFactories, ConstructionType::Civ));
    if (milFactories > 0)
        resources.push_back(std::make_unique<ConstructionResource>("Mil factories", milFactories, ConstructionType::Mil));
    if (infrastructure > 0)
        resources.push_back(std::make_unique<ConstructionResource>("Infrastructure", infrastructure, ConstructionType::Infra));

    if (dockyards > 0)
        resources.push_back(std::make_unique<ConstructionResource>("Dockyards", dockyards, ConstructionType::Dockyard));
    if (airfields > 0)
        resources.push_back(std::make_unique<ConstructionResource>("Airfields", airfields, ConstructionType::Airfield));

    if (armyRF > 0)
        resources.push_back(std::make_unique<ConstructionResource>("Army RF", armyRF, ConstructionType::ArmyRF));
    if (navalRF > 0)
        resources.push_back(std::make_unique<ConstructionResource>("Naval RF", navalRF, ConstructionType::NavalRF));
    if (aerialRF > 0)
        resources.push_back(std::make_unique<ConstructionResource>("Aerial RF", aerialRF, ConstructionType::AerialRF));
    if (nuclearRF > 0)
        resources.push_back(std::make_unique<ConstructionResource>("Nuclear RF", nuclearRF, ConstructionType::NuclearRF));
}

void Province::addCiv(int x) {
    civFactories = std::max(0, civFactories + x);
    initResources();
}

void Province::addMil(int x) {
    milFactories = std::max(0, milFactories + x);
    initResources();
}

void Province::addInfra(int x) {
    infrastructure = std::min(10, std::max(0, infrastructure + x));
    initResources();
}

void Province::addDockyard(int x) {
    dockyards = std::max(0, dockyards + x);
    initResources();
}

void Province::addAirfield(int x) {
    airfields = std::max(0, airfields + x);
    initResources();
}

void Province::applyResourceEffects(ResourceStockpile& stockpile) const {
    for (const auto& res : resources) {
        res->applyDailyEffect(stockpile);
    }
}

int Province::totalConstructionSlotsFromResources() const {
    int total = 0;
    for (const auto& res : resources) {
        if (auto* cr = dynamic_cast<const ConstructionResource*>(res.get())) {
            total += cr->getAmount();
        }
    }
    return total;
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

    ss << ", RES_OBJS={";
    for (std::size_t i = 0; i < resources.size(); ++i) {
        resources[i]->print(ss);
        if (i + 1 < resources.size()) ss << "; ";
    }
    ss << "}";

    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Province& p) {
    return os << p.toString();
}
