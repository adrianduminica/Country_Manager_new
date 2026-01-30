/**
 * @file Country.cpp
 * @brief Implementarea logicii principale pentru o țară.
 */

#include "../headers/Country.h"
#include "../headers/GameExceptions.h"
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <iostream>

/**
 * @brief Constructorul principal.
 * Inițializează țara cu provinciile și resursele sale.
 */
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

int Country::getUsedMilFactories() const {
    int used = 0;
    for (const auto &line: milLines) {
        used += line.getFactories();
    }
    return used;
}

int Country::getFreeMilFactories() const {
    return totalMil() - getUsedMilFactories();
}

void Country::addProductionLine(EquipmentType t) {
    int factoriesToAssign = 1;
    if (getFreeMilFactories() <= 0) {
        factoriesToAssign = 0;
    }
    milLines.push_back(ProductionLine(t, factoriesToAssign, 0.0));
}

void Country::modifyLineFactories(int index, int amount) {
    if (index < 0 || index >= static_cast<int>(milLines.size())) return;
    ProductionLine &line = milLines[index];
    int current = line.getFactories();

    if (amount > 0) {
        if (getFreeMilFactories() >= amount) {
            line.setFactories(current + amount);
        }
    } else {
        if (current + amount >= 0) {
            line.setFactories(current + amount);
        }
    }
}

long long Country::getEquipmentCount(EquipmentType t) const {
    switch (t) {
        case EquipmentType::Gun: return equipment.getGuns();
        case EquipmentType::Artillery: return equipment.getArtillery();
        case EquipmentType::AntiAir: return equipment.getAntiAir();
        case EquipmentType::CAS: return equipment.getCAS();
    }
    return 0;
}

bool Country::startFocus(int index) {
    return focusTree.startFocus(index);
}

/**
 * @brief Adaugă o construcție în coada de așteptare, verificând regulile jocului.
 *
 * Această funcție este critică pentru mecanica jocului deoarece:
 * 1. Validează indexul provinciei.
 * 2. Parcurge întreaga coadă curentă de construcții pentru a vedea ce este deja planificat (queued).
 * 3. Aplică regulile stricte de limitare a clădirilor per provincie (Hard Limits):
 * - Infrastructură: Maxim 5.
 * - Aeroporturi: Maxim 10.
 * - Industrie (Civ/Mil/Dock): Maxim 6 sloturi industriale.
 * 4. Determină costul de producție (IC) în funcție de tipul clădirii.
 *
 * @param type Tipul clădirii de construit.
 * @param provinceIndex Indexul provinciei țintă.
 * @param count Numărul de clădiri solicitate.
 * @throws GameException Dacă limita maximă a provinciei ar fi depășită.
 */
void Country::addConstruction(BuildingType type, int provinceIndex, int count) {
    if (count <= 0) return;
    if (provinceIndex < 0 || static_cast<std::size_t>(provinceIndex) >= provinces.size())
        throw InvalidProvinceIndexException("Index provincie invalid", provinceIndex);

    const Province &prov = provinces[provinceIndex];

    // Calculăm clădirile deja aflate în coada de așteptare
    int queuedCiv = 0;
    int queuedMil = 0;
    int queuedInfra = 0;
    int queuedAir = 0;
    int queuedDock = 0;
    for (const auto &c: constructions) {
        if (c.getProvinceIndex() == provinceIndex) {
            switch (c.getType()) {
                case BuildingType::Civ: queuedCiv++;
                    break;
                case BuildingType::Mil: queuedMil++;
                    break;
                case BuildingType::Infra: queuedInfra++;
                    break;
                case BuildingType::Airfield: queuedAir++;
                    break;
                case BuildingType::Dockyard: queuedDock++;
                    break;
                default: break;
            }
        }
    }

    // Verificăm limitele jocului (Clădiri existente + Clădiri în coadă + Cererea curentă)
    if (type == BuildingType::Infra) {
        if (prov.getInfra() + queuedInfra + count > 5) throw GameException(
            "Limita atinsa: Infrastructura maxima este 5!");
    } else if (type == BuildingType::Airfield) {
        if (prov.getAirfields() + queuedAir + count > 10) throw GameException("Limita atinsa: Aeroporturi maxime 10!");
    } else if (type == BuildingType::Civ) {
        if (prov.getCiv() + queuedCiv + count > 6) throw GameException("Limita atinsa: Maxim 6 Fabrici Civile!");
    } else if (type == BuildingType::Mil) {
        if (prov.getMil() + queuedMil + count > 6) throw GameException("Limita atinsa: Maxim 6 Fabrici Militare!");
    } else if (type == BuildingType::Dockyard) {
        if (prov.getDockyards() + queuedDock + count > 6) throw GameException("Limita atinsa: Maxim 6 Santier Navale!");
    }

    double cost = 200;
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

/**
 * @brief Nucleul logic al jocului: avansează simularea cu o zi.
 *
 * Această funcție gestionează toate aspectele economice și militare zilnice:
 * 1. **Resurse:** Actualizează bonusurile resurselor locale în fiecare provincie.
 * 2. **Producție Militară:** Iterează prin toate liniile de producție, calculează output-ul zilnic
 * (Daily Output) și adaugă echipamentul finit în stocul național.
 * 3. **Construcții:** Calculează puterea totală de construcție (Total Civs * 5).
 * Aceasta este aplicată primei construcții din coadă. Dacă se finalizează, clădirea
 * este adăugată efectiv provinciei.
 * 4. **Focus Național:** Verifică progresul focusului politic. Dacă se finalizează un pas (tickRaw),
 * aplică efectul aleatoriu (RNG) asupra unei provincii random.
 */
void Country::simulateDay() {
    // 1. Efecte resurse
    for (const auto &p: provinces) p.applyResourceEffects(resources);

    // 2. Producție militară
    for (const auto &l: milLines) {
        long long units = l.calculateDailyOutput();
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

    // 3. Construcții
    double dailyBP = totalCiv() * CIV_OUTPUT_PER_DAY;
    if (!constructions.isEmpty()) {
        Construction &c = constructions.front();
        if (c.progress(dailyBP)) {
            int idx = c.getProvinceIndex();
            if (idx >= 0 && idx < static_cast<int>(provinces.size())) {
                try {
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
                } catch (...) {
                    // Ignorăm erorile la finalizare
                }
            }
            constructions.removeFirst();
        }
    }

    // 4. Focus Tree
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
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const Country &c) { return os << c.toString(); }