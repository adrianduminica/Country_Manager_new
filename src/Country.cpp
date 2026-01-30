#include "../headers/Country.h"
#include "../headers/GameExceptions.h"
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <iostream>

Country::Country(std::string n, std::string id, std::vector<Province> p, ResourceStockpile r)
    : name(std::move(n)), ideology(std::move(id)), provinces(std::move(p)), resources(r) {}

Country::Country(const Country& other)
    : name(other.name), ideology(other.ideology), provinces(other.provinces),
      resources(other.resources), equipment(other.equipment),
      milLines(other.milLines), constructions(other.constructions), focusTree(other.focusTree) {}

Country& Country::operator=(const Country& other) {
    if (this != &other) {
        name = other.name; ideology = other.ideology; provinces = other.provinces;
        resources = other.resources; equipment = other.equipment;
        milLines = other.milLines; constructions = other.constructions; focusTree = other.focusTree;
    }
    return *this;
}

int Country::totalCiv() const { int s = 0; for (const auto& p : provinces) s += p.getCiv(); return s; }
int Country::totalMil() const { int s = 0; for (const auto& p : provinces) s += p.getMil(); return s; }
int Country::totalOil() const { int s = 0; for (const auto& p : provinces) s += p.getOil(); return s; }
int Country::totalSteel() const { int s = 0; for (const auto& p : provinces) s += p.getSteel(); return s; }
int Country::totalTungsten() const { int s = 0; for (const auto& p : provinces) s += p.getTungsten(); return s; }
int Country::totalAluminum() const { int s = 0; for (const auto& p : provinces) s += p.getAluminum(); return s; }
int Country::totalChromium() const { int s = 0; for (const auto& p : provinces) s += p.getChromium(); return s; }

void Country::addProductionLine(EquipmentType t, int factories) {
    milLines.add(ProductionLine(t, factories, -1.0));
}

bool Country::startFocus(int index) {
    return focusTree.startFocus(index);
}

void Country::addConstruction(BuildingType type, int provinceIndex, int count) {
    if (count <= 0) return;
    if (provinceIndex < 0 || static_cast<std::size_t>(provinceIndex) >= provinces.size())
        throw InvalidProvinceIndexException("Index provincie invalid", provinceIndex);

    const Province& prov = provinces[provinceIndex];

    int queuedCiv = 0;
    int queuedMil = 0;
    int queuedInfra = 0;
    int queuedAir = 0;
    int queuedDock = 0;

    for (const auto& c : constructions) {
        if (c.getProvinceIndex() == provinceIndex) {
            switch (c.getType()) {
                case BuildingType::Civ:      queuedCiv++; break;
                case BuildingType::Mil:      queuedMil++; break;
                case BuildingType::Infra:    queuedInfra++; break;
                case BuildingType::Airfield: queuedAir++; break;
                case BuildingType::Dockyard: queuedDock++; break;
                default: break;
            }
        }
    }

    if (type == BuildingType::Infra) {
        if (prov.getInfra() + queuedInfra + count > 5) {
            throw GameException("Limita atinsa: Infrastructura maxima este 5!");
        }
    }
    else if (type == BuildingType::Airfield) {
        if (prov.getAirfields() + queuedAir + count > 10) {
            throw GameException("Limita atinsa: Aeroporturi maxime 10!");
        }
    }
    else if (type == BuildingType::Civ) {
        if (prov.getCiv() + queuedCiv + count > 6) {
            throw GameException("Limita atinsa: Maxim 6 Fabrici Civile!");
        }
    }
    else if (type == BuildingType::Mil) {
        if (prov.getMil() + queuedMil + count > 6) {
            throw GameException("Limita atinsa: Maxim 6 Fabrici Militare!");
        }
    }
    else if (type == BuildingType::Dockyard) {
        if (prov.getDockyards() + queuedDock + count > 6) {
            throw GameException("Limita atinsa: Maxim 6 Santier Navale!");
        }
    }

    double cost = 0;
    switch (type) {
        case BuildingType::Civ: cost = 100; break;
        case BuildingType::Mil: cost = 120; break;
        case BuildingType::Infra: cost = 80; break;
        case BuildingType::Dockyard: cost = 150; break;
        default: cost = 200; break;
    }

    for (int i = 0; i < count; ++i) {
        constructions.add(Construction(type, provinceIndex, cost));
    }

    std::cout << "Added construction successfully.\n";
}

void Country::simulateDay() {
    for (const auto& p : provinces) p.applyResourceEffects(resources);

    for (const auto& l : milLines) {
        long long units = static_cast<long long>(std::floor((l.getFactories() * MIL_OUTPUT) / l.getUnitCost()));
        switch (l.getType()) {
            case EquipmentType::Gun: equipment.addGuns(units); break;
            case EquipmentType::Artillery: equipment.addArtillery(units); break;
            case EquipmentType::AntiAir: equipment.addAntiAir(units); break;
            case EquipmentType::CAS: equipment.addCAS(units); break;
        }
    }

    double dailyBP = totalCiv() * CIV_OUTPUT_PER_DAY;
    if (!constructions.isEmpty()) {
        Construction& c = constructions.front();
        if (c.progress(dailyBP)) {
            int idx = c.getProvinceIndex();
            if (idx >= 0 && idx < static_cast<int>(provinces.size())) {
                try {
                    switch (c.getType()) {
                        case BuildingType::Civ: provinces[idx].addCiv(1); break;
                        case BuildingType::Mil: provinces[idx].addMil(1); break;
                        case BuildingType::Infra: provinces[idx].addInfra(1); break;
                        case BuildingType::Dockyard: provinces[idx].addDockyard(1); break;
                        default: provinces[idx].addAirfield(1); break;
                    }
                } catch (...) {}
            }
            constructions.removeFirst();
        }
    }

    int effRaw = focusTree.tickRaw();
    if (effRaw != -1 && !provinces.empty()) {
        int i = std::rand() % static_cast<int>(provinces.size());
        switch (static_cast<FocusEffectType>(effRaw)) {
            case FocusEffectType::AddCiv: provinces[i].addCiv(1); break;
            case FocusEffectType::AddMil: provinces[i].addMil(1); break;
            case FocusEffectType::AddInfra: provinces[i].addInfra(1); break;
            case FocusEffectType::AddDockyard: provinces[i].addDockyard(1); break;
        }
    }
}

std::string Country::toString() const {
    std::ostringstream ss;
    ss << "Country(" << name << ")\n";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Country& c) { return os << c.toString(); }