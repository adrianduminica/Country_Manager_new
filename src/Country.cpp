/**
 * @file Country.cpp
 * @brief Implementarea clasei Country: gestiune provincii, productie, constructii si focus tree.
 *
 * Fisierul contine implementarea metodelor pentru:
 *  - agregarea statisticilor din provincii (fabrici, resurse)
 *  - managementul liniilor de productie militara
 *  - coada de constructii si progres zilnic
 *  - tick zilnic (simulateDay) incluzand focus tree effects
 */

#include "../headers/Country.h"
#include "../headers/GameExceptions.h"
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <iostream>

/**
 * @brief Constructor cu mutare pentru a initializa complet un Country.
 * @param n Numele tarii.
 * @param id Ideologia (string identificator).
 * @param p Vectorul de provincii.
 * @param r Stocul initial de resurse.
 */
Country::Country(std::string n, std::string id, std::vector<Province> p, ResourceStockpile r)
    : name(std::move(n)), ideology(std::move(id)), provinces(std::move(p)), resources(r) {
}

/**
 * @brief Copy-constructor.
 * @param other Obiectul sursa.
 */
Country::Country(const Country &other)
    : name(other.name), ideology(other.ideology), provinces(other.provinces),
      resources(other.resources), equipment(other.equipment),
      milLines(other.milLines), constructions(other.constructions), focusTree(other.focusTree) {
}

/**
 * @brief Operator de atribuire (copy assignment).
 * @param other Obiectul sursa.
 * @return Referinta la obiectul curent dupa copiere.
 */
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

/**
 * @brief Calculeaza totalul de fabrici civile din toate provinciile.
 * @return Numarul total de fabrici civile.
 */
int Country::totalCiv() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getCiv();
    return s;
}

/**
 * @brief Calculeaza totalul de fabrici militare din toate provinciile.
 * @return Numarul total de fabrici militare.
 */
int Country::totalMil() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getMil();
    return s;
}

/**
 * @brief Calculeaza totalul de petrol (oil) din toate provinciile.
 * @return Totalul de oil.
 */
int Country::totalOil() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getOil();
    return s;
}

/**
 * @brief Calculeaza totalul de otel (steel) din toate provinciile.
 * @return Totalul de steel.
 */
int Country::totalSteel() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getSteel();
    return s;
}

/**
 * @brief Calculeaza totalul de tungsten din toate provinciile.
 * @return Totalul de tungsten.
 */
int Country::totalTungsten() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getTungsten();
    return s;
}

/**
 * @brief Calculeaza totalul de aluminiu (aluminum) din toate provinciile.
 * @return Totalul de aluminum.
 */
int Country::totalAluminum() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getAluminum();
    return s;
}

/**
 * @brief Calculeaza totalul de crom (chromium) din toate provinciile.
 * @return Totalul de chromium.
 */
int Country::totalChromium() const {
    int s = 0;
    for (const auto &p: provinces) s += p.getChromium();
    return s;
}

/**
 * @brief Calculeaza cate fabrici militare sunt deja alocate pe liniile de productie.
 * @return Numarul de fabrici militare utilizate.
 */
int Country::getUsedMilFactories() const {
    int used = 0;
    for (const auto &line: milLines) {
        used += line.getFactories();
    }
    return used;
}

/**
 * @brief Calculeaza cate fabrici militare sunt disponibile (nealocate) pentru productie.
 *
 * Valoarea este diferenta dintre totalul de fabrici militare si fabricile deja alocate
 * pe liniile de productie.
 *
 * @return Numarul de fabrici militare libere.
 */
int Country::getFreeMilFactories() const {
    return totalMil() - getUsedMilFactories();
}

/**
 * @brief Adauga o noua linie de productie pentru un tip de echipament.
 *
 * Aloca implicit 1 fabrica daca exista fabrici militare libere; altfel aloca 0.
 *
 * @param t Tipul de echipament produs pe linie.
 */
void Country::addProductionLine(EquipmentType t) {
    int factoriesToAssign = 1;
    if (getFreeMilFactories() <= 0) {
        factoriesToAssign = 0;
    }
    milLines.push_back(ProductionLine(t, factoriesToAssign, 0.0));
}

/**
 * @brief Modifica numarul de fabrici alocate unei linii de productie.
 *
 * Daca amount > 0: incearca sa adauge fabrici (doar daca sunt suficiente libere).
 * Daca amount < 0: scoate fabrici, dar fara a scadea sub 0.
 * Daca index este invalid: nu face nimic.
 *
 * @param index Indexul liniei in vectorul de linii de productie.
 * @param amount Diferenta de fabrici (pozitiv / negativ).
 */
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

/**
 * @brief Obtine cantitatea curenta dintr-un tip de echipament.
 * @param t Tipul de echipament cerut.
 * @return Numarul de unitati din tipul respectiv.
 */
long long Country::getEquipmentCount(EquipmentType t) const {
    switch (t) {
        case EquipmentType::Gun: return equipment.getGuns();
        case EquipmentType::Artillery: return equipment.getArtillery();
        case EquipmentType::AntiAir: return equipment.getAntiAir();
        case EquipmentType::CAS: return equipment.getCAS();
    }
    return 0;
}

/**
 * @brief Porneste un focus din focus tree.
 * @param index Indexul focus-ului (conform structurii focusTree).
 * @return true daca focus-ul a pornit cu succes, false altfel.
 */
bool Country::startFocus(int index) {
    return focusTree.startFocus(index);
}

/**
 * @brief Adauga constructii in coada pentru o anumita provincie.
 *
 * Valideaza:
 *  - count > 0
 *  - index provincie valid
 *  - limite maxime (inclusiv ce este deja in coada pentru acea provincie)
 *
 * Costul per constructie depinde de tipul cladirii.
 *
 * @param type Tipul cladirii care se construieste.
 * @param provinceIndex Indexul provinciei tinta.
 * @param count Numarul de cladiri de adaugat in coada.
 *
 * @throws InvalidProvinceIndexException daca provinceIndex este invalid.
 * @throws GameException daca se depaseste o limita maxima pentru tipul selectat.
 */
void Country::addConstruction(BuildingType type, int provinceIndex, int count) {
    if (count <= 0) return;
    if (provinceIndex < 0 || static_cast<std::size_t>(provinceIndex) >= provinces.size())
        throw InvalidProvinceIndexException("Index provincie invalid", provinceIndex);

    const Province &prov = provinces[provinceIndex];

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
 * @brief Simuleaza o zi de joc pentru tara curenta.
 *
 * Include:
 *  - aplicarea efectelor de resurse la nivel de provincie
 *  - productia zilnica a liniilor militare (adauga echipament)
 *  - progresul pe prima constructie din coada (daca exista)
 *  - tick pentru focus tree si aplicarea unui efect random pe o provincie (daca exista efect)
 */
void Country::simulateDay() {
    for (const auto &p: provinces) p.applyResourceEffects(resources);

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

/**
 * @brief Returneaza o reprezentare text a tarii.
 * @return String cu informatii de baza despre tara.
 */
std::string Country::toString() const {
    std::ostringstream ss;
    ss << "Country(" << name << ")\n";
    return ss.str();
}

/**
 * @brief Operator de stream pentru afisarea unui Country.
 * @param os Stream-ul de iesire.
 * @param c Tara de afisat.
 * @return Referinta la stream-ul de iesire.
 */
std::ostream &operator<<(std::ostream &os, const Country &c) { return os << c.toString(); }
