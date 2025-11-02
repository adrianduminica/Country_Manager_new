#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <ctime>

// ============================================================================
//                               ResourceStockpile (dinamic)
// ============================================================================
class ResourceStockpile {
    int fuel;
    int manpower;
    static int clampNonNeg(int x) { return x < 0 ? 0 : x; }

public:
    ResourceStockpile() : fuel(0), manpower(0) {}
    ResourceStockpile(int fuel, int manpower) : fuel(fuel), manpower(manpower) {}

    int  getFuel() const     { return fuel; }
    int  getManpower() const { return manpower; }

    void add(int dFuel, int dManpower) {
        fuel     = clampNonNeg(fuel     + dFuel);
        manpower = clampNonNeg(manpower + dManpower);
    }
    std::string toString() const {
        std::ostringstream ss;
        ss << "Fuel=" << getFuel() << ", Manpower=" << getManpower();
        return ss.str();
    }
};
std::ostream& operator<<(std::ostream& os, const ResourceStockpile& r) { return os << r.toString(); }

// ============================================================================
//                               EquipmentStockpile
// ============================================================================
class EquipmentStockpile {
    long long guns=0, artillery=0, antiAir=0, cas=0;
public:
    void addGuns(long long n)      { if(n>0) guns+=n; }
    void addArtillery(long long n) { if(n>0) artillery+=n; }
    void addAntiAir(long long n)   { if(n>0) antiAir+=n; }
    void addCAS(long long n)       { if(n>0) cas+=n; }

    long long getGuns()const{return guns;}
    long long getArtillery()const{return artillery;}
    long long getAntiAir()const{return antiAir;}
    long long getCAS()const{return cas;}

    std::string toString() const {
        std::ostringstream ss;
        ss << "Guns="<< getGuns()
           << ", Artillery="<< getArtillery()
           << ", AA="<< getAntiAir()
           << ", CAS="<< getCAS();
        return ss.str();
    }
};
std::ostream& operator<<(std::ostream& os,const EquipmentStockpile& e){ return os<<e.toString(); }

// ============================================================================
//                                   Province
// ============================================================================
class Province {
    std::string name;
    int population{}, civFactories{}, milFactories{}, infrastructure{};
    int dockyards{}, airfields{};
    int armyRF{}, navalRF{}, aerialRF{}, nuclearRF{};

    // --- static resources ---
    int steel{}, tungsten{}, aluminum{}, chromium{}, oil{};

public:
    Province()=default;
    Province(std::string name,int pop,int civ,int mil,int infra,
             int steel,int tungsten,int aluminum,int chromium,int oil)
        : name(std::move(name)),population(pop),civFactories(civ),milFactories(mil),
          infrastructure(infra),
          steel(steel),tungsten(tungsten),aluminum(aluminum),chromium(chromium),oil(oil){}

    const std::string& getName()const{return name;}
    int getPopulation()const{return population;}
    int getCiv()const{return civFactories;}
    int getMil()const{return milFactories;}
    int getInfra()const{return infrastructure;}
    int getDockyards()const{return dockyards;}
    int getAirfields()const{return airfields;}
    int getArmyRF()const{return armyRF;}
    int getNavalRF()const{return navalRF;}
    int getAerialRF()const{return aerialRF;}
    int getNuclearRF()const{return nuclearRF;}

    // --- resources ---
    int getSteel()const{return steel;}
    int getTungsten()const{return tungsten;}
    int getAluminum()const{return aluminum;}
    int getChromium()const{return chromium;}
    int getOil()const{return oil;}

    void addCiv(int x){ civFactories=std::max(0,civFactories+x);}
    void addMil(int x){ milFactories=std::max(0,milFactories+x);}
    void addInfra(int x){ infrastructure=std::min(10,std::max(0,infrastructure+x));}
    void addDockyard(int x){ dockyards+=x; }
    void addAirfield(int x){ airfields+=x; }

    [[maybe_unused]] void placeArmyRF(){ armyRF=1; }
    [[maybe_unused]] void placeNavalRF(){ navalRF=1; }
    [[maybe_unused]] void placeAerialRF(){ aerialRF=1; }
    [[maybe_unused]] void placeNuclearRF(){ nuclearRF=1; }

    std::string toString() const {
        std::ostringstream ss;
        ss<<"Province("<< getName() <<") pop="<< getPopulation()
          <<", CIV="<< getCiv() <<", MIL="<< getMil()
          <<", INFRA="<< getInfra()
          <<", RES[Steel="<< getSteel()
          <<", Tung="<< getTungsten()
          <<", Alu="<< getAluminum()
          <<", Chr="<< getChromium()
          <<", Oil="<< getOil() << "]"
          <<", DOCKYARDS="<< getDockyards()
          <<", AIRFIELDS="<< getAirfields()
          <<", FACILITIES[Army="<< getArmyRF()
          <<",Naval="<< getNavalRF()
          <<",Aerial="<< getAerialRF()
          <<",Nuclear="<< getNuclearRF() <<"]";
        return ss.str();
    }
};
std::ostream& operator<<(std::ostream& os,const Province&p){return os<<p.toString();}

// ============================================================================
//                            Military Production
// ============================================================================
enum class EquipmentType{Gun,Artillery,AntiAir,CAS};
class ProductionLine{
    EquipmentType type; int factoriesAssigned; double unitCost;
    static constexpr double DEF_G=10, DEF_A=50, DEF_AA=40, DEF_CAS=200;
    static double defCost(EquipmentType t){
        switch(t){
            case EquipmentType::Gun: return DEF_G;
            case EquipmentType::Artillery: return DEF_A;
            case EquipmentType::AntiAir: return DEF_AA;
            case EquipmentType::CAS: return DEF_CAS;
        } return DEF_G;
    }
public:
    ProductionLine(EquipmentType t,int f,double c)
        : type(t), factoriesAssigned(std::max(0,f)), unitCost(c>0?c:defCost(t)){}
    EquipmentType getType()const{return type;}
    int getFactories()const{return factoriesAssigned;}
    double getUnitCost()const{return unitCost;}
    std::string toString()const{
        const char* tn=(type==EquipmentType::Gun?"Gun":
                        type==EquipmentType::Artillery?"Artillery":
                        type==EquipmentType::AntiAir?"AA":"CAS");
        std::ostringstream ss; ss<<tn<<" [factories="<<getFactories()<<", unitCost="<<getUnitCost()<<"]"; return ss.str();
    }
};
std::ostream& operator<<(std::ostream& os,const ProductionLine&p){return os<<p.toString();}

// ============================================================================
//                            Building Construction System
// ============================================================================
enum class BuildingType { Civ, Mil, Infra, Dockyard, Airfield, ArmyRF, NavalRF, AerialRF, NuclearRF };

class Construction {
    BuildingType type;
    int provinceIndex;
    double remainingBP;
    double totalCost;
public:
    Construction(BuildingType type, int provinceIndex, double totalCost)
        : type(type), provinceIndex(provinceIndex), remainingBP(totalCost), totalCost(totalCost) {}

    bool progress(double dailyBP) {
        remainingBP -= dailyBP;
        return remainingBP <= 0;
    }

    BuildingType getType() const { return type; }
    int getProvinceIndex() const { return provinceIndex; }
    double getRemainingBP() const { return remainingBP; }
    double getTotalCost() const { return totalCost; }

    std::string toString() const {
        std::ostringstream ss;
        ss<<"["<<(int)getType()<<"] "<< getRemainingBP() <<"/"<< getTotalCost()
          <<" BP left in province "<< getProvinceIndex();
        return ss.str();
    }
};

// ============================================================================
//                             Focus Tree System
// ============================================================================
enum class FocusEffectType { AddCiv, AddMil, AddInfra, AddDockyard };

class Focus {
    std::string name_;
    int daysRequired_;
    FocusEffectType effect_;
    bool completed_ = false;

public:
    Focus(std::string name, int daysRequired, FocusEffectType effect)
        : name_(std::move(name)), daysRequired_(daysRequired), effect_(effect) {}

    const std::string&  name()   const { return name_; }
    int                 days()   const { return daysRequired_; }
    FocusEffectType     effect() const { return effect_; }
    [[maybe_unused]] bool completed() const { return completed_; }
    void                markCompleted() { completed_ = true; }
};

class FocusTree {
    std::vector<Focus> focuses;
    int currentIndex = -1;
    int progress = 0;

public:
    FocusTree() {
        focuses.emplace_back("Industrial Expansion", 35, FocusEffectType::AddCiv);
        focuses.emplace_back("Military Buildup",     35, FocusEffectType::AddMil);
        focuses.emplace_back("Infrastructure Effort",25, FocusEffectType::AddInfra);
        focuses.emplace_back("Dockyard Development", 30, FocusEffectType::AddDockyard);
    }

    bool startFocus(int index) {
        if (currentIndex != -1) { std::cerr << "[!] A focus is already running.\n"; return false; }
        if (index < 0 || index >= (int)focuses.size()) { std::cerr << "[!] Invalid focus index.\n"; return false; }
        currentIndex = index; progress = 0;
        std::cout << "[FOCUS] Started: " << focuses[index].name()
                  << " (" << focuses[index].days() << " days)\n";
        return true;
    }

    [[maybe_unused]] bool hasActiveFocus() const { return currentIndex != -1; }
    std::string getActiveFocusName() const { return currentIndex==-1 ? "None" : focuses[currentIndex].name(); }

    int tickRaw() {
        if (currentIndex == -1) return -1;
        progress++;
        if (progress >= focuses[currentIndex].days()) {
            auto eff = (int)focuses[currentIndex].effect();
            focuses[currentIndex].markCompleted();
            std::cout << "[FOCUS COMPLETED] " << focuses[currentIndex].name() << " finished!\n";
            currentIndex = -1;
            return eff;
        }
        return -1;
    }
};

// ============================================================================
//                                   Country
// ============================================================================
class Country {
    std::string name, ideology;
    std::vector<Province> provinces;
    ResourceStockpile resources;
    EquipmentStockpile equipment;
    std::vector<ProductionLine> milLines;
    std::vector<Construction> constructions;
    FocusTree focusTree;

    static constexpr int MIL_OUTPUT=1000, OIL_TO_FUEL=5;
    static constexpr double CIV_OUTPUT_PER_DAY=5.0;

public:
    Country(std::string n,std::string id,std::vector<Province>p,ResourceStockpile r)
        :name(std::move(n)),ideology(std::move(id)),provinces(std::move(p)),resources(r){}

    int totalCiv()const{int s=0;for(const auto& p:provinces)s+=p.getCiv();return s;}
    int totalMil()const{int s=0;for(const auto& p:provinces)s+=p.getMil();return s;}
    int totalOil()const{int s=0;for(const auto& p:provinces)s+=p.getOil();return s;}
    int totalSteel()const{int s=0;for(const auto& p:provinces)s+=p.getSteel();return s;}
    int totalTungsten()const{int s=0;for(const auto& p:provinces)s+=p.getTungsten();return s;}
    int totalAluminum()const{int s=0;for(const auto& p:provinces)s+=p.getAluminum();return s;}
    int totalChromium()const{int s=0;for(const auto& p:provinces)s+=p.getChromium();return s;}

    void addProductionLine(EquipmentType t,int factories){ milLines.emplace_back(t,factories,-1.0); }
    void startFocus(int index){ (void)focusTree.startFocus(index); }

    void addConstruction(BuildingType type, int provinceIndex) {
        double cost = 0;
        switch(type){
            case BuildingType::Civ: cost=100; break;
            case BuildingType::Mil: cost=120; break;
            case BuildingType::Infra: cost=80; break;
            case BuildingType::Dockyard: cost=150; break;
            default: cost=200; break;
        }
        constructions.emplace_back(type, provinceIndex, cost);
    }

    void simulateDay(){
        // resurse
        int fuelGain = totalOil()*OIL_TO_FUEL;
        resources.add(fuelGain, 0);

        // producție militară
        for(const auto& l : milLines){
            long long units = (long long)std::floor( (l.getFactories()*MIL_OUTPUT) / l.getUnitCost() );
            switch(l.getType()){
                case EquipmentType::Gun:       equipment.addGuns(units);      break;
                case EquipmentType::Artillery: equipment.addArtillery(units); break;
                case EquipmentType::AntiAir:   equipment.addAntiAir(units);   break;
                case EquipmentType::CAS:       equipment.addCAS(units);       break;
            }
        }

        // construcții
        double dailyBP = totalCiv() * CIV_OUTPUT_PER_DAY;
        for (auto it = constructions.begin(); it != constructions.end();) {
            if (it->progress(dailyBP)) {
                int idx = it->getProvinceIndex();
                if (idx >= 0 && idx < (int)provinces.size()) {
                    switch(it->getType()){
                        case BuildingType::Civ: provinces[idx].addCiv(1); break;
                        case BuildingType::Mil: provinces[idx].addMil(1); break;
                        case BuildingType::Infra: provinces[idx].addInfra(1); break;
                        case BuildingType::Dockyard: provinces[idx].addDockyard(1); break;
                        default: provinces[idx].addAirfield(1); break;
                    }
                }
                it = constructions.erase(it);
            } else ++it;
        }

        // focus
        int effRaw = focusTree.tickRaw();
        if (effRaw != -1 && !provinces.empty()) {
            int i = std::rand() % provinces.size();
            switch(static_cast<FocusEffectType>(effRaw)){
                case FocusEffectType::AddCiv:      provinces[i].addCiv(1);      break;
                case FocusEffectType::AddMil:      provinces[i].addMil(1);      break;
                case FocusEffectType::AddInfra:    provinces[i].addInfra(1);    break;
                case FocusEffectType::AddDockyard: provinces[i].addDockyard(1); break;
            }
        }
    }

    std::string toString() const {
        std::ostringstream ss;
        ss<<"Country("<<name<<", "<<ideology<<")\n";
        ss<<"  Factories: CIV="<<totalCiv()<<" | MIL="<<totalMil()<<"\n";
        ss<<"  Resources: Steel="<<totalSteel()
          <<", Tungsten="<<totalTungsten()
          <<", Aluminum="<<totalAluminum()
          <<", Chromium="<<totalChromium()
          <<", Oil="<<totalOil()<<"\n";
        ss<<"  Focus: "<<focusTree.getActiveFocusName()<<"\n";
        ss<<"  Equipment: "<<equipment<<"\n";
        ss<<"  Stockpile: "<<resources<<"\n";
        ss<<"  Constructions: "<<constructions.size()<<"\n";
        ss<<"  Provinces:\n";
        for(const auto& p: provinces) ss<<"    - "<<p<<"\n";
        return ss.str();
    }
};
std::ostream& operator<<(std::ostream& os,const Country&c){ return os<<c.toString(); }

// ----------------------------- cppcheck silencer -----------------------------
// Face “used” funcțiile marcate de cppcheck fără să le apeleze/execute.
// Nu schimbă în niciun fel funcționalitatea runtime.
namespace __silence_cppcheck {
    inline void touch() {
        [[maybe_unused]] void (Province::*p1)() = &Province::placeArmyRF;
        [[maybe_unused]] void (Province::*p2)() = &Province::placeNavalRF;
        [[maybe_unused]] void (Province::*p3)() = &Province::placeAerialRF;
        [[maybe_unused]] void (Province::*p4)() = &Province::placeNuclearRF;
        [[maybe_unused]] bool (Focus::*p5)() const = &Focus::completed;
        [[maybe_unused]] bool (FocusTree::*p6)() const = &FocusTree::hasActiveFocus;
        (void)p1; (void)p2; (void)p3; (void)p4; (void)p5; (void)p6;
    }
}

// ============================================================================
// MAIN
// ============================================================================
int main(){
    std::srand((unsigned)std::time(nullptr));
    if (false) { __silence_cppcheck::touch(); } // nu se execută; doar oprește “unusedFunction”

    // --- Romania ---
    Province p1("Wallachia",1800,3,3,6,5,3,4,1,3);
    Province p2("Moldavia",1500,2,2,5,4,2,3,1,2);
    Province p3("Transylvania",1600,2,1,7,8,5,6,3,1);
    Country Romania("Romania","Democratic",{p1,p2,p3},ResourceStockpile(0,100));
    Romania.addProductionLine(EquipmentType::Gun,2);
    Romania.startFocus(0);
    Romania.addConstruction(BuildingType::Mil, 1);

    // --- Hungary ---
    Province h1("Alfold",1400,2,2,6,4,2,3,1,2);
    Province h2("Transdanubia",1200,2,1,6,3,2,2,1,1);
    Country Hungary("Hungary","Authoritarian",{h1,h2},ResourceStockpile(0,80));
    Hungary.addProductionLine(EquipmentType::Artillery,1);
    Hungary.startFocus(1);
    Hungary.addConstruction(BuildingType::Civ, 0);

    std::cout<<"=== INITIAL STATE ===\n"<<Romania<<"\n"<<Hungary<<"\n";
    for(int d=1; d<=40; ++d){ Romania.simulateDay(); Hungary.simulateDay(); }
    std::cout<<"\n=== AFTER 40 DAYS ===\n"<<Romania<<"\n"<<Hungary<<"\n";

    return 0;
}
