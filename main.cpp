#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>

// ============================================================================
//                               ResourceStockpile
// ============================================================================
class ResourceStockpile {
    int fuel;
    int manpower;
    static int clampNonNeg(int x) { return x < 0 ? 0 : x; }

public:
    ResourceStockpile() : fuel(0), manpower(0) {}
    ResourceStockpile(int fuel, int manpower) : fuel(fuel), manpower(manpower) {}

    int getFuel() const { return fuel; }
    int getManpower() const { return manpower; }
    void add(int dFuel, int dManpower) {
        fuel     = clampNonNeg(fuel     + dFuel);
        manpower = clampNonNeg(manpower + dManpower);
    }
    std::string toString() const {
        std::ostringstream ss;
        ss << "Fuel=" << fuel << ", Manpower=" << manpower;
        return ss.str();
    }
};
std::ostream& operator<<(std::ostream& os, const ResourceStockpile& r) {
    return os << r.toString();
}

// ============================================================================
//                               EquipmentStockpile
// ============================================================================
class EquipmentStockpile {
    long long guns = 0, artillery = 0, antiAir = 0, cas = 0;
public:
    void addGuns(long long n){ if(n>0) guns+=n; }
    void addArtillery(long long n){ if(n>0) artillery+=n; }
    void addAntiAir(long long n){ if(n>0) antiAir+=n; }
    void addCAS(long long n){ if(n>0) cas+=n; }
    long long getGuns()const{return guns;} long long getArtillery()const{return artillery;}
    long long getAntiAir()const{return antiAir;} long long getCAS()const{return cas;}
    std::string toString() const {
        std::ostringstream ss;
        ss << "Guns="<<guns<<", Artillery="<<artillery<<", AA="<<antiAir<<", CAS="<<cas;
        return ss.str();
    }
};
std::ostream& operator<<(std::ostream& os, const EquipmentStockpile& e){return os<<e.toString();}

// ============================================================================
//                                   Province
// ============================================================================
class Province {
    std::string name;
    int population{}, civFactories{}, milFactories{}, infrastructure{}, steelReserve{}, oilReserve{};
    int dockyards{}, airfields{};
    int armyRF{}, navalRF{}, aerialRF{}, nuclearRF{}; // counts (can be 0 or 1 per province)

public:
    Province()=default;
    Province(std::string name,int pop,int civ,int mil,int infra,int steel,int oil)
        : name(std::move(name)),population(pop),civFactories(civ),milFactories(mil),
          infrastructure(infra),steelReserve(steel),oilReserve(oil){}

    const std::string& getName()const{return name;}
    int getPopulation()const{return population;}
    int getCiv()const{return civFactories;} int getMil()const{return milFactories;}
    int getInfra()const{return infrastructure;} int getSteelReserve()const{return steelReserve;}
    int getOilReserve()const{return oilReserve;}
    int getDockyards()const{return dockyards;} int getAirfields()const{return airfields;}
    int getArmyRF()const{return armyRF;} int getNavalRF()const{return navalRF;}
    int getAerialRF()const{return aerialRF;} int getNuclearRF()const{return nuclearRF;}

    void addCiv(int x){ civFactories=std::max(0,civFactories+x);}
    void addMil(int x){ milFactories=std::max(0,milFactories+x);}
    void addInfra(int x){ infrastructure=std::min(10,std::max(0,infrastructure+x));}
    void addDockyard(int x){dockyards+=x;} void addAirfield(int x){airfields+=x;}
    void placeArmyRF(){armyRF=1;} void placeNavalRF(){navalRF=1;}
    void placeAerialRF(){aerialRF=1;} void placeNuclearRF(){nuclearRF=1;}

    std::string toString() const{
        std::ostringstream ss;
        ss<<"Province("<<name<<") pop="<<population<<", INFRA="<<infrastructure
          <<", CIV="<<civFactories<<", MIL="<<milFactories
          <<", STEEL="<<steelReserve<<", OIL="<<oilReserve
          <<", DOCKYARDS="<<dockyards<<", AIRFIELDS="<<airfields
          <<", FACILITIES[Army="<<armyRF<<",Naval="<<navalRF
          <<",Aerial="<<aerialRF<<",Nuclear="<<nuclearRF<<"]";
        return ss.str();
    }
};
std::ostream& operator<<(std::ostream& os,const Province&p){return os<<p.toString();}

// ============================================================================
//                                Construction
// ============================================================================
enum class BuildType{Infra,Civ,Mil,Refinery,Dockyard,Airfield,ArmyRF,NavalRF,AerialRF,NuclearRF};
class ConstructionTask{
    BuildType type; int provinceIndex; double remainingBP,baseCostBP;
public:
    ConstructionTask(BuildType t,int prov,double cost):type(t),provinceIndex(prov),remainingBP(cost),baseCostBP(cost){}
    BuildType getType()const{return type;} int getProvinceIndex()const{return provinceIndex;}
    double getRemainingBP()const{return remainingBP;} void setRemainingBP(double x){remainingBP=x;}
    std::string toString()const{
        const char*tn[]={"INFRA","CIV","MIL","REFINERY","DOCKYARD","AIRFIELD","ARMY_RF","NAVAL_RF","AERIAL_RF","NUCLEAR_RF"};
        std::ostringstream ss; ss<<tn[(int)type]<<"(prov="<<provinceIndex<<", left="<<std::fixed<<std::setprecision(1)
          <<remainingBP<<"/"<<baseCostBP<<")"; return ss.str();}
};
std::ostream& operator<<(std::ostream& os,const ConstructionTask&t){return os<<t.toString();}

// ============================================================================
//                            Military Production
// ============================================================================
enum class EquipmentType{Gun,Artillery,AntiAir,CAS};
class ProductionLine{
    EquipmentType type;int factoriesAssigned;double unitCost;
    static constexpr double DEF_G=10,DEF_A=50,DEF_AA=40,DEF_CAS=200;
    static double defCost(EquipmentType t){
        switch(t){case EquipmentType::Gun:return DEF_G;case EquipmentType::Artillery:return DEF_A;
            case EquipmentType::AntiAir:return DEF_AA;case EquipmentType::CAS:return DEF_CAS;}return DEF_G;}
public:
    ProductionLine(EquipmentType t,int f,double c):type(t),factoriesAssigned(std::max(0,f)),
        unitCost(c>0?c:defCost(t)){}
    EquipmentType getType()const{return type;}int getFactories()const{return factoriesAssigned;}
    double getUnitCost()const{return unitCost;}
    std::string toString()const{
        const char* tn=(type==EquipmentType::Gun?"Gun":type==EquipmentType::Artillery?"Artillery":
                        type==EquipmentType::AntiAir?"AA":"CAS");
        std::ostringstream ss; ss<<tn<<" [factories="<<factoriesAssigned<<", unitCost="<<unitCost<<"]"; return ss.str();}
};
std::ostream& operator<<(std::ostream& os,const ProductionLine&p){return os<<p.toString();}

// ============================================================================
//                                   Country
// ============================================================================
class Country{
    std::string name,ideology;std::vector<Province> provinces;ResourceStockpile resources;
    EquipmentStockpile equipment;std::vector<ConstructionTask> queue;std::vector<ProductionLine> milLines;
    int refineries=0;

    static constexpr int MIL_OUTPUT=1000,OIL_TO_FUEL=5,REFINERY_FUEL=10,AIRFIELD_MAX_PROV=5;
    static constexpr double CIV_OUT=1.0;
    static constexpr double COSTS[10]={5,25,25,20,30,15,40,40,40,60};
public:
    Country(std::string n,std::string id,std::vector<Province>p,ResourceStockpile r)
        :name(std::move(n)),ideology(std::move(id)),provinces(std::move(p)),resources(r){}

    // Totals
    int totalCiv()const{int s=0;for(auto&p:provinces)s+=p.getCiv();return s;}
    int totalMil()const{int s=0;for(auto&p:provinces)s+=p.getMil();return s;}
    int totalOil()const{int s=0;for(auto&p:provinces)s+=p.getOilReserve();return s;}
    int totalDockyards()const{int s=0;for(auto&p:provinces)s+=p.getDockyards();return s;}
    int totalAirfields()const{int s=0;for(auto&p:provinces)s+=p.getAirfields();return s;}
    int totalArmyRF()const{int s=0;for(auto&p:provinces)s+=p.getArmyRF();return s;}
    int totalNavalRF()const{int s=0;for(auto&p:provinces)s+=p.getNavalRF();return s;}
    int totalAerialRF()const{int s=0;for(auto&p:provinces)s+=p.getAerialRF();return s;}
    int totalNuclearRF()const{int s=0;for(auto&p:provinces)s+=p.getNuclearRF();return s;}

    // Constructions
    bool startConstruction(BuildType t,int prov=-1){
        bool needsProv=(t!=BuildType::Refinery);
        if(needsProv&&(prov<0||prov>=(int)provinces.size())){std::cerr<<"[!] Invalid province index.\n";return false;}
        if(needsProv){
            auto &p=provinces[prov];
            switch(t){
                case BuildType::Airfield:if(p.getAirfields()>=AIRFIELD_MAX_PROV){std::cerr<<"[!] Airfield cap reached in "<<p.getName()<<".\n";return false;}break;
                case BuildType::ArmyRF:if(p.getArmyRF()>=1){std::cerr<<"[!] ArmyRF exists in "<<p.getName()<<".\n";return false;}break;
                case BuildType::NavalRF:if(p.getNavalRF()>=1){std::cerr<<"[!] NavalRF exists in "<<p.getName()<<".\n";return false;}break;
                case BuildType::AerialRF:if(p.getAerialRF()>=1){std::cerr<<"[!] AerialRF exists in "<<p.getName()<<".\n";return false;}break;
                case BuildType::NuclearRF:if(p.getNuclearRF()>=1){std::cerr<<"[!] NuclearRF exists in "<<p.getName()<<".\n";return false;}break;
                default:break;
            }
        }else prov=-1;
        queue.emplace_back(t,prov,COSTS[(int)t]);return true;
    }

    bool addProductionLine(EquipmentType t,int f,double c=-1){
        int used=0;for(auto&l:milLines)used+=l.getFactories();
        if(used+f>totalMil()){std::cerr<<"[!] Not enough military factories.\n";return false;}
        milLines.emplace_back(t,f,c);return true;
    }

    void simulateDay(){
        int fuelGain=totalOil()*OIL_TO_FUEL+refineries*REFINERY_FUEL;resources.add(fuelGain,0);
        int civ=totalCiv();
        if(!queue.empty()&&civ>0){
            double throughput=civ*CIV_OUT,perTask=throughput/queue.size();
            for(auto&t:queue)t.setRemainingBP(std::max(0.0,t.getRemainingBP()-perTask));
            std::vector<ConstructionTask>next;
            for(auto&t:queue){
                if(t.getRemainingBP()>0.001){next.push_back(t);continue;}
                switch(t.getType()){
                    case BuildType::Infra:provinces[t.getProvinceIndex()].addInfra(1);break;
                    case BuildType::Civ:provinces[t.getProvinceIndex()].addCiv(1);break;
                    case BuildType::Mil:provinces[t.getProvinceIndex()].addMil(1);break;
                    case BuildType::Refinery:++refineries;break;
                    case BuildType::Dockyard:provinces[t.getProvinceIndex()].addDockyard(1);break;
                    case BuildType::Airfield:provinces[t.getProvinceIndex()].addAirfield(1);break;
                    case BuildType::ArmyRF:provinces[t.getProvinceIndex()].placeArmyRF();break;
                    case BuildType::NavalRF:provinces[t.getProvinceIndex()].placeNavalRF();break;
                    case BuildType::AerialRF:provinces[t.getProvinceIndex()].placeAerialRF();break;
                    case BuildType::NuclearRF:provinces[t.getProvinceIndex()].placeNuclearRF();break;
                }
            }queue.swap(next);
        }
        for(auto&l:milLines){
            int used=std::min(l.getFactories(),totalMil());
            long long units=(long long)std::floor((used*MIL_OUTPUT)/l.getUnitCost());
            switch(l.getType()){
                case EquipmentType::Gun:equipment.addGuns(units);break;
                case EquipmentType::Artillery:equipment.addArtillery(units);break;
                case EquipmentType::AntiAir:equipment.addAntiAir(units);break;
                case EquipmentType::CAS:equipment.addCAS(units);break;
            }
        }
    }

    std::string toString()const{
        std::ostringstream ss;
        ss<<"Country("<<name<<", "<<ideology<<")\n";
        ss<<"  Factories: CIV="<<totalCiv()<<" | MIL="<<totalMil()<<" | REFINERIES="<<refineries<<"\n";
        ss<<"  Buildings: Dockyards="<<totalDockyards()<<" | Airfields="<<totalAirfields()<<" (max/prov="<<AIRFIELD_MAX_PROV<<")\n";
        ss<<"  Facilities: ArmyRF="<<totalArmyRF()<<", NavalRF="<<totalNavalRF()
          <<", AerialRF="<<totalAerialRF()<<", NuclearRF="<<totalNuclearRF()<<"\n";
        ss<<"  Resources: "<<resources<<"\n  Equipment: "<<equipment<<"\n";
        if(!queue.empty()){ss<<"  Construction Queue:\n";for(auto&t:queue)ss<<"    - "<<t<<"\n";}
        else ss<<"  Construction Queue: (empty)\n";
        ss<<"  Provinces:\n";for(auto&p:provinces)ss<<"    - "<<p<<"\n";return ss.str();
    }
};
std::ostream& operator<<(std::ostream& os,const Country&c){return os<<c.toString();}

// ============================================================================
// MAIN
// ============================================================================
int main(){
    Province p1("Wallachia",1800,3,3,6,2,3);
    Province p2("Moldavia",1500,2,2,5,5,1);
    Province p3("Transylvania",1600,2,1,7,8,2);
    ResourceStockpile roRes(0,100);
    Country Romania("Romania","Democratic",{p1,p2,p3},roRes);

    Romania.startConstruction(BuildType::Infra,0);
    Romania.startConstruction(BuildType::Civ,2);
    Romania.startConstruction(BuildType::Dockyard,0);
    Romania.startConstruction(BuildType::Airfield,1);
    Romania.startConstruction(BuildType::ArmyRF,2);
    Romania.startConstruction(BuildType::Refinery);

    Romania.addProductionLine(EquipmentType::Gun,3);
    Romania.addProductionLine(EquipmentType::Artillery,2);

    Province h1("Alfold",1400,2,2,6,4,2);
    Province h2("Transdanubia",1200,2,1,6,3,1);
    ResourceStockpile huRes(0,80);
    Country Hungary("Hungary","Authoritarian",{h1,h2},huRes);

    Hungary.startConstruction(BuildType::Mil,1);
    Hungary.startConstruction(BuildType::Dockyard,0);
    Hungary.startConstruction(BuildType::Airfield,0);
    Hungary.startConstruction(BuildType::AerialRF,1);

    Hungary.addProductionLine(EquipmentType::Gun,2);
    Hungary.addProductionLine(EquipmentType::AntiAir,1);

    std::cout<<"=== INITIAL STATE ===\n"<<Romania<<"\n"<<Hungary<<"\n";
    for(int d=1;d<=30;++d){Romania.simulateDay();Hungary.simulateDay();}
    std::cout<<"\n=== AFTER 30 DAYS ===\n"<<Romania<<"\n"<<Hungary<<"\n";
}
