#include "../headers/Country.h"
#include "../headers/GameExceptions.h"
#include <sstream>
#include <cmath>
#include <cstdlib>

Country::Country(std::string n, std::string id, std::vector<Province> p, ResourceStockpile r)
    : name(std::move(n)), ideology(std::move(id)), provinces(std::move(p)), resources(r) {
}

Country::Country(const Country &other)
    : name(other.name), ideology(other.ideology), provinces(other.provinces),
      resources(other.resources), equipment(other.equipment),
      milLines(other.milLines), constructions(other.constructions), focusTree(other.focusTree) {
}

Country &Country::operator=(const Country &other) {
    if (this != &other) {
        name = other.name;
        ideology = other.ideology;
        provinces = other.provinces;
        resources = other.resources;
        equipment = other.equipment;
        milLines = other.milLines;
        constructions = other.constructions;
        focusTree = other.focusTree;
    }
    return *this;
}

int Country::totalCiv() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getCiv();
    return s;
}

int Country::totalMil() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getMil();
    return s;
}

int Country::totalOil() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getOil();
    return s;
}

int Country::totalSteel() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getSteel();
    return s;
}

int Country::totalTungsten() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getTungsten();
    return s;
}

int Country::totalAluminum() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getAluminum();
    return s;
}

int Country::totalChromium() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getChromium();
    return s;
}

void Country::addProductionLine(EquipmentType t, int factories) {
    milLines.add(ProductionLine(t, factories, -1.0));
}

// --- MODIFICARE: Returneaza bool ---
bool Country::startFocus(int index) {
    return focusTree.startFocus(index);
}

void Country::addConstruction(BuildingType type, int provinceIndex, int count) {
    if (count < 0) throw InvalidConstructionCountException("Numar invalid constructii", count);
    if (count == 0) return;
    if (provinceIndex < 0 || static_cast<std::size_t>(provinceIndex) >= provinces.size())
        throw InvalidProvinceIndexException("Index provincie invalid", provinceIndex);

    double cost = 0;
    switch (type) {
        case BuildingType::Civ: cost = 100;
            break;
        case BuildingType::Mil: cost = 120;
            break;
        case BuildingType::Infra: cost = 80;
            break;
        case BuildingType::Dockyard: cost = 150;
            break;
        default: cost = 200;
            break;
    }

    for (int i = 0; i < count; ++i) {
        constructions.add(Construction(type, provinceIndex, cost));
    }
}

void Country::simulateDay() {
    for (const auto &p: provinces) p.applyResourceEffects(resources);

    for (const auto &l: milLines) {
        long long units = static_cast<long long>(std::floor((l.getFactories() * MIL_OUTPUT) / l.getUnitCost()));
        switch (l.getType()) {
            case EquipmentType::Gun: equipment.addGuns(units);
                break;
            case EquipmentType::Artillery: equipment.addArtillery(units);
                break;
            case EquipmentType::AntiAir: equipment.addAntiAir(units);
                break;
            case EquipmentType::CAS: equipment.addCAS(units);
                break;
        }
    }

    double dailyBP = totalCiv() * CIV_OUTPUT_PER_DAY;
    if (!constructions.isEmpty()) {
        Construction &c = constructions.front();
        if (c.progress(dailyBP)) {
            int idx = c.getProvinceIndex();
            if (idx >= 0 && idx < static_cast<int>(provinces.size())) {
                switch (c.getType()) {
                    case BuildingType::Civ: provinces[idx].addCiv(1);
                        break;
                    case BuildingType::Mil: provinces[idx].addMil(1);
                        break;
                    case BuildingType::Infra: provinces[idx].addInfra(1);
                        break;
                    case BuildingType::Dockyard: provinces[idx].addDockyard(1);
                        break;
                    default: provinces[idx].addAirfield(1);
                        break;
                }
            }
            constructions.removeFirst();
        }
    }

    int effRaw = focusTree.tickRaw();
    if (effRaw != -1 && !provinces.empty()) {
        int i = std::rand() % static_cast<int>(provinces.size());
        switch (static_cast<FocusEffectType>(effRaw)) {
            case FocusEffectType::AddCiv: provinces[i].addCiv(1);
                break;
            case FocusEffectType::AddMil: provinces[i].addMil(1);
                break;
            case FocusEffectType::AddInfra: provinces[i].addInfra(1);
                break;
            case FocusEffectType::AddDockyard: provinces[i].addDockyard(1);
                break;
        }
    }
}

std::string Country::toString() const {
    std::ostringstream ss;
    ss << "Country(" << name << ")\n";
    // ... restul logicii de afisare ...
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const Country &c) { return os << c.toString(); }
