#ifndef RESOURCE_FACTORY_H
#define RESOURCE_FACTORY_H

#include <memory>
#include <string>
#include "ResourceBase.h"

// --- DESIGN PATTERN: FACTORY METHOD (Cerință: Șablon de proiectare 1) ---
// Centralizează crearea resurselor, eliminând dependența directă de "new" în Province.
class ResourceFactory {
public:
    static std::unique_ptr<AbstractResource> createMaterial(const std::string &name, int amount) {
        return std::make_unique<MaterialResource>(name, amount);
    }

    static std::unique_ptr<AbstractResource> createDailyOutput(const std::string &name, int amount, int fuelRatio) {
        return std::make_unique<DailyOutputResource>(name, amount, fuelRatio);
    }

    static std::unique_ptr<AbstractResource> createConstruction(const std::string &name, int amount,
                                                                ConstructionType type) {
        return std::make_unique<ConstructionResource>(name, amount, type);
    }
};

#endif // RESOURCE_FACTORY_H
