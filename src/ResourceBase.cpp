#include "../headers/ResourceBase.h"

int AbstractResource::globalCount = 0;

AbstractResource::AbstractResource(std::string name, int amount)
    : name(std::move(name)), amount(amount)
{
    if (this->amount < 0) {
        this->amount = 0;
    }
    ++globalCount;
}

AbstractResource::~AbstractResource() = default;

MaterialResource::MaterialResource(std::string name, int amount)
    : AbstractResource(std::move(name), amount) {}

void MaterialResource::applyDailyEffect(ResourceStockpile& stockpile) const {
    (void)stockpile;
}

std::unique_ptr<AbstractResource> MaterialResource::clone() const {
    return std::make_unique<MaterialResource>(*this);
}

// Implementare functii noi
std::string MaterialResource::getCategory() const {
    return "Raw Material";
}

bool MaterialResource::isStrategic() const {
    return true; // Materialele sunt strategice
}

void MaterialResource::printImpl(std::ostream& os) const {
    os << "MaterialResource(" << name << ", amount=" << amount << ")";
}



DailyOutputResource::DailyOutputResource(std::string name,
                                         int amount,
                                         int fuelPerUnitPerDay)
    : AbstractResource(std::move(name), amount),
      fuelPerUnitPerDay(fuelPerUnitPerDay)
{
    if (this->fuelPerUnitPerDay < 0) {
        this->fuelPerUnitPerDay = 0;
    }
}

void DailyOutputResource::applyDailyEffect(ResourceStockpile& stockpile) const {
    int producedFuel = amount * fuelPerUnitPerDay;
    stockpile.add(producedFuel, 0);
}

std::unique_ptr<AbstractResource> DailyOutputResource::clone() const {
    return std::make_unique<DailyOutputResource>(*this);
}

// Implementare functii noi
std::string DailyOutputResource::getCategory() const {
    return "Daily Output";
}
// isStrategic ramane false (default)

void DailyOutputResource::printImpl(std::ostream& os) const {
    os << "DailyOutputResource(" << name
       << ", amount=" << amount
       << ", fuelPerUnitPerDay=" << fuelPerUnitPerDay << ")";
}


ConstructionResource::ConstructionResource(std::string name,
                                           int amount,
                                           ConstructionType type)
    : AbstractResource(std::move(name), amount),
      type(type)
{
}

void ConstructionResource::applyDailyEffect(ResourceStockpile& stockpile) const {
    (void)stockpile;
}

std::unique_ptr<AbstractResource> ConstructionResource::clone() const {
    return std::make_unique<ConstructionResource>(*this);
}

// Implementare functii noi
std::string ConstructionResource::getCategory() const {
    return "Building";
}

bool ConstructionResource::isStrategic() const {
    return true;
}

void ConstructionResource::printImpl(std::ostream& os) const {
    os << "ConstructionResource(" << name
       << ", amount=" << amount
       << ", type=";
    switch (type) {
        case ConstructionType::Civ:       os << "Civ";       break;
        case ConstructionType::Mil:       os << "Mil";       break;
        case ConstructionType::Infra:     os << "Infra";     break;
        case ConstructionType::Dockyard:  os << "Dockyard";  break;
        case ConstructionType::Airfield:  os << "Airfield";  break;
        case ConstructionType::ArmyRF:    os << "ArmyRF";    break;
        case ConstructionType::NavalRF:   os << "NavalRF";   break;
        case ConstructionType::AerialRF:  os << "AerialRF";  break;
        case ConstructionType::NuclearRF: os << "NuclearRF"; break;
    }
    os << ")";
}