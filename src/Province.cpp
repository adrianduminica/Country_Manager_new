/**
 * @file Province.cpp
 * @brief Implementarea clasei Province: populatie, cladiri (civ/mil/infra/dock/air), resurse si efecte zilnice.
 *
 * Model:
 *  - date brute: populatie, fabrici, infrastructura, resurse numerice (steel/tungsten/aluminum/chromium/oil)
 *  - vectorul `resources` contine obiecte polymorfice (Material/DailyOutput/ConstructionResource etc.)
 *    create prin ResourceFactory, folosite pentru aplicarea efectelor zilnice asupra unui ResourceStockpile.
 *
 * Notite:
 *  - initResources() reconstruieste lista `resources` din valorile numerice curente.
 *  - metodele add* modifica un atribut si apoi refac `resources` pentru consistenta.
 */

#include "../headers/Province.h"
#include "../headers/ResourceFactory.h"
#include "../headers/Utils.h"
#include <sstream>
#include <algorithm>

/**
 * @brief Constructor: initializeaza o provincie cu valori brute pentru populatie, cladiri si resurse.
 *
 * Valori negative sunt clamp-uite la 0, iar infrastructura este limitata in [0, 10].
 * Dupa validare, initResources() construieste lista de resurse (objects) pe baza valorilor numerice.
 *
 * @param name Numele provinciei.
 * @param pop Populatia (valori < 0 devin 0).
 * @param civ Numarul de fabrici civile (valori < 0 devin 0).
 * @param mil Numarul de fabrici militare (valori < 0 devin 0).
 * @param infra Nivelul infrastructurii (clamp in [0, 10]).
 * @param steel Cantitatea de steel (valori < 0 devin 0).
 * @param tungsten Cantitatea de tungsten (valori < 0 devin 0).
 * @param aluminum Cantitatea de aluminum (valori < 0 devin 0).
 * @param chromium Cantitatea de chromium (valori < 0 devin 0).
 * @param oil Cantitatea de oil (valori < 0 devin 0).
 */
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
      oil(oil) {
    if (population < 0) population = 0;
    if (civFactories < 0) civFactories = 0;
    if (milFactories < 0) milFactories = 0;

    infrastructure = GameUtils::ensureRange<int>(infrastructure, 0, 10);

    if (steel < 0) this->steel = 0;
    if (tungsten < 0) this->tungsten = 0;
    if (aluminum < 0) this->aluminum = 0;
    if (chromium < 0) this->chromium = 0;
    if (oil < 0) this->oil = 0;

    initResources();
}

/**
 * @brief Copy-constructor.
 *
 * Face deep-copy pentru vectorul de `resources` (clone pe fiecare resursa),
 * astfel incat provinciile sa nu partajeze aceleasi obiecte polymorfice.
 *
 * @param other Provincia sursa.
 */
Province::Province(const Province &other)
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
      oil(other.oil) {
    resources.clear();
    resources.reserve(other.resources.size());
    for (const auto &resPtr: other.resources) {
        resources.push_back(resPtr->clone());
    }
}

/**
 * @brief Operator de atribuire (copy assignment) implementat prin copy-and-swap.
 *
 * Primeste parametrul prin valoare (copiaza), apoi face swap, obtinand siguranta la exceptii.
 *
 * @param other Copie a provinciei sursa.
 * @return Referinta la obiectul curent.
 */
Province &Province::operator=(Province other) {
    swap(*this, other);
    return *this;
}

/**
 * @brief Reconstruieste lista de resurse (objects) pe baza valorilor numerice curente.
 *
 * Curata `resources` si adauga:
 *  - materiale (Steel/Aluminum/Tungsten/Chromium) daca valorile sunt > 0
 *  - productie zilnica (Oil) daca oil > 0 (cu output zilnic fix: 5)
 *  - resurse de constructie pentru civ/mil/infra/dockyards/airfields daca sunt > 0
 *
 * Aceasta metoda este chemata dupa orice modificare a valorilor relevante (addCiv/addMil/etc.)
 * pentru a mentine consistenta intre campurile numerice si reprezentarea polymorfica.
 */
void Province::initResources() {
    resources.clear();

    if (steel > 0)
        resources.push_back(ResourceFactory::createMaterial("Steel", steel));
    if (aluminum > 0)
        resources.push_back(ResourceFactory::createMaterial("Aluminum", aluminum));
    if (tungsten > 0)
        resources.push_back(ResourceFactory::createMaterial("Tungsten", tungsten));
    if (chromium > 0)
        resources.push_back(ResourceFactory::createMaterial("Chromium", chromium));

    if (oil > 0)
        resources.push_back(ResourceFactory::createDailyOutput("Oil", oil, 5));

    if (civFactories > 0)
        resources.push_back(ResourceFactory::createConstruction("Civ factories", civFactories, ConstructionType::Civ));
    if (milFactories > 0)
        resources.push_back(ResourceFactory::createConstruction("Mil factories", milFactories, ConstructionType::Mil));
    if (infrastructure > 0)
        resources.push_back(
            ResourceFactory::createConstruction("Infrastructure", infrastructure, ConstructionType::Infra));

    if (dockyards > 0)
        resources.push_back(ResourceFactory::createConstruction("Dockyards", dockyards, ConstructionType::Dockyard));
    if (airfields > 0)
        resources.push_back(ResourceFactory::createConstruction("Airfields", airfields, ConstructionType::Airfield));
}

/**
 * @brief Adauga (sau scade) fabrici civile si mentine valoarea in [0, +inf).
 * @param x Diferenta aplicata numarului de fabrici civile (poate fi negativa).
 */
void Province::addCiv(int x) {
    civFactories = std::max(0, civFactories + x);
    initResources();
}

/**
 * @brief Adauga (sau scade) fabrici militare si mentine valoarea in [0, +inf).
 * @param x Diferenta aplicata numarului de fabrici militare (poate fi negativa).
 */
void Province::addMil(int x) {
    milFactories = std::max(0, milFactories + x);
    initResources();
}

/**
 * @brief Modifica infrastructura si o limiteaza in intervalul [0, 10].
 * @param x Diferenta aplicata infrastructurii (poate fi negativa).
 */
void Province::addInfra(int x) {
    infrastructure = GameUtils::ensureRange<int>(infrastructure + x, 0, 10);
    initResources();
}

/**
 * @brief Adauga (sau scade) dockyards si mentine valoarea in [0, +inf).
 * @param x Diferenta aplicata numarului de dockyards (poate fi negativa).
 */
void Province::addDockyard(int x) {
    dockyards = std::max(0, dockyards + x);
    initResources();
}

/**
 * @brief Adauga (sau scade) airfields si mentine valoarea in [0, +inf).
 * @param x Diferenta aplicata numarului de airfields (poate fi negativa).
 */
void Province::addAirfield(int x) {
    airfields = std::max(0, airfields + x);
    initResources();
}

/**
 * @brief Aplica efectele zilnice ale resurselor provinciei asupra stockpile-ului.
 *
 * Itereaza peste `resources` si apeleaza applyDailyEffect() pe fiecare resursa polymorfica.
 *
 * @param stockpile Stockpile-ul global al tarii care este modificat de efectele provinciei.
 */
void Province::applyResourceEffects(ResourceStockpile &stockpile) const {
    for (const auto &res: resources) {
        res->applyDailyEffect(stockpile);
    }
}

/**
 * @brief Calculeaza numarul total de sloturi de constructie (din resurse), excluzand infrastructura.
 *
 * Interpreteaza doar resursele de tip ConstructionResource si insumeaza amount pentru toate
 * tipurile diferite de Infra (Infra este exclusa explicit).
 *
 * @return Totalul sloturilor de constructie non-infra provenite din `resources`.
 */
int Province::totalConstructionSlotsFromResources() const {
    int total = 0;
    for (const auto &res: resources) {
        if (auto *cr = dynamic_cast<const ConstructionResource *>(res.get())) {
            if (cr->getType() != ConstructionType::Infra) {
                total += cr->getAmount();
            }
        }
    }
    return total;
}

/**
 * @brief Construieste o reprezentare text detaliata a provinciei.
 *
 * Include datele principale (populatie, cladiri, resurse numerice) + lista de obiecte resursa
 * (RES_OBJS) prin apelul virtual print().
 *
 * @return String descriptiv pentru debug/logging.
 */
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

/**
 * @brief Operator de stream pentru afisarea unei Province.
 * @param os Stream-ul de iesire.
 * @param p Provincia de afisat.
 * @return Referinta la stream-ul de iesire.
 */
std::ostream &operator<<(std::ostream &os, const Province &p) {
    return os << p.toString();
}
