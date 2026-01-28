#ifndef RESOURCE_BASE_H
#define RESOURCE_BASE_H

#include <string>
#include <memory>
#include <ostream>

#include "ResourceStockpile.h"

class AbstractResource {
protected:
    std::string name;
    int amount;
    static int globalCount;

    AbstractResource(std::string name, int amount);
    virtual void printImpl(std::ostream& os) const = 0;

public:
    virtual ~AbstractResource();

    const std::string& getName() const { return name; }
    int getAmount() const { return amount; }

    virtual void applyDailyEffect(ResourceStockpile& stockpile) const = 0;
    virtual std::unique_ptr<AbstractResource> clone() const = 0;

    void print(std::ostream& os) const { printImpl(os); }

    static int getGlobalCount() { return globalCount; }
};

class MaterialResource : public AbstractResource {
public:
    MaterialResource(std::string name, int amount);

    void applyDailyEffect(ResourceStockpile& stockpile) const override;
    std::unique_ptr<AbstractResource> clone() const override;

protected:
    void printImpl(std::ostream& os) const override;
};

class DailyOutputResource : public AbstractResource {
    int fuelPerUnitPerDay;

public:
    DailyOutputResource(std::string name, int amount, int fuelPerUnitPerDay);

    void applyDailyEffect(ResourceStockpile& stockpile) const override;
    std::unique_ptr<AbstractResource> clone() const override;

protected:
    void printImpl(std::ostream& os) const override;
};

enum class ConstructionType {
    Civ,
    Mil,
    Infra,
    Dockyard,
    Airfield,
    ArmyRF,
    NavalRF,
    AerialRF,
    NuclearRF
};

class ConstructionResource : public AbstractResource {
    ConstructionType type;

public:
    ConstructionResource(std::string name, int amount, ConstructionType type);

    ConstructionType getType() const { return type; }

    void applyDailyEffect(ResourceStockpile& stockpile) const override;
    std::unique_ptr<AbstractResource> clone() const override;

protected:
    void printImpl(std::ostream& os) const override;
};

inline std::ostream& operator<<(std::ostream& os, const AbstractResource& r) {
    r.print(os);
    return os;
}

#endif
