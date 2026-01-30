#ifndef PRODUCTIONLINE_H
#define PRODUCTIONLINE_H

enum class EquipmentType { Gun, Artillery, AntiAir, CAS };

class ProductionLine {
    EquipmentType type;
    int factories;
    double efficiency;
    double unitCost;

public:
    ProductionLine(EquipmentType t, int f, double eff);

    EquipmentType getType() const;

    int getFactories() const;

    double getEfficiency() const;

    double getUnitCost() const;

    void setFactories(int count);

    long long calculateDailyOutput() const;
};

#endif
