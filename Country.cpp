#include "Country.h"
#include <sstream>
#include <cmath>
#include <cstdlib>

Country::Country(std::string n,
                 std::string id,
                 std::vector<Province> p,
                 ResourceStockpile r)
    : name(std::move(n)),
      ideology(std::move(id)),
      provinces(std::move(p)),
      resources(r) {}

Country::Country(const Country& other)
    : name(other.name),
      ideology(other.ideology),
      provinces(other.provinces),
      resources(other.resources),
      equipment(other.equipment),
      milLines(other.milLines),
      constructions(other.constructions),
      focusTree(other.focusTree) {}

Country& Country::operator=(const Country& other) {
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
    for (const auto& p : provinces) s += p.getCiv();
    return s;
}

int Country::totalMil() const {
    int s = 0;
    for (const auto& p : provinces) s += p.getMil();
    return s;
}

int Country::totalOil() const {
    int s = 0;
    for (const auto& p : provinces) s += p.getOil();
    return s;
}

int Country::totalSteel() const {
    int s = 0;
    for (const auto& p : provinces) s += p.getSteel();
    return s;
}

int Country::totalTungsten() const {
    int s = 0;
    for (const auto& p : provinces) s += p.getTungsten();
    return s;
}

int Country::totalAluminum() const {
    int s = 0;
    for (const auto& p : provinces) s += p.getAluminum();
    return s;
}

int Country::totalChromium() const {
    int s = 0;
    for (const auto& p : provinces) s += p.getChromium();
    return s;
}

void Country::addProductionLine(EquipmentType t, int factories) {
    milLines.emplace_back(t, factories, -1.0);
}

void Country::startFocus(int index) {
    (void)focusTree.startFocus(index);
}

void Country::addConstruction(BuildingType type, int provinceIndex) {
    double cost = 0;
    switch (type) {
        case BuildingType::Civ:      cost = 100; break;
        case BuildingType::Mil:      cost = 120; break;
        case BuildingType::Infra:    cost = 80;  break;
        case BuildingType::Dockyard: cost = 150; break;
        default:                     cost = 200; break;
    }
    constructions.emplace_back(type, provinceIndex, cost);
}

void Country::simulateDay() {
    // resurse
    int fuelGain = totalOil() * OIL_TO_FUEL;
    resources.add(fuelGain, 0);

    // producție militară
    for (const auto& l : milLines) {
        long long units = static_cast<long long>(
            std::floor((l.getFactories() * MIL_OUTPUT) / l.getUnitCost())
        );
        switch (l.getType()) {
            case EquipmentType::Gun:
                equipment.addGuns(units);      break;
            case EquipmentType::Artillery:
                equipment.addArtillery(units); break;
            case EquipmentType::AntiAir:
                equipment.addAntiAir(units);   break;
            case EquipmentType::CAS:
                equipment.addCAS(units);       break;
        }
    }

    // construcții
    double dailyBP = totalCiv() * CIV_OUTPUT_PER_DAY;
    for (auto it = constructions.begin(); it != constructions.end(); ) {
        if (it->progress(dailyBP)) {
            int idx = it->getProvinceIndex();
            if (idx >= 0 && idx < static_cast<int>(provinces.size())) {
                switch (it->getType()) {
                    case BuildingType::Civ:      provinces[idx].addCiv(1);      break;
                    case BuildingType::Mil:      provinces[idx].addMil(1);      break;
                    case BuildingType::Infra:    provinces[idx].addInfra(1);    break;
                    case BuildingType::Dockyard: provinces[idx].addDockyard(1); break;
                    default:                     provinces[idx].addAirfield(1); break;
                }
            }
            it = constructions.erase(it);
        } else {
            ++it;
        }
    }

    // focus
    int effRaw = focusTree.tickRaw();
    if (effRaw != -1 && !provinces.empty()) {
        int i = std::rand() % provinces.size();
        switch (static_cast<FocusEffectType>(effRaw)) {
            case FocusEffectType::AddCiv:
                provinces[i].addCiv(1);      break;
            case FocusEffectType::AddMil:
                provinces[i].addMil(1);      break;
            case FocusEffectType::AddInfra:
                provinces[i].addInfra(1);    break;
            case FocusEffectType::AddDockyard:
                provinces[i].addDockyard(1); break;
        }
    }
}

std::string Country::toString() const {
    std::ostringstream ss;
    ss << "Country(" << name << ", " << ideology << ")\n";
    ss << "  Factories: CIV=" << totalCiv()
       << " | MIL=" << totalMil() << "\n";
    ss << "  Resources: Steel=" << totalSteel()
       << ", Tungsten=" << totalTungsten()
       << ", Aluminum=" << totalAluminum()
       << ", Chromium=" << totalChromium()
       << ", Oil=" << totalOil() << "\n";
    ss << "  Focus: " << focusTree.getActiveFocusName() << "\n";
    ss << "  Equipment: " << equipment << "\n";
    ss << "  Stockpile: " << resources << "\n";
    ss << "  Constructions: " << constructions.size() << "\n";
    ss << "  Provinces:\n";
    for (const auto& p : provinces) {
        ss << "    - " << p << "\n";
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Country& c) {
    return os << c.toString();
}
