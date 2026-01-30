/**
 * @file FocusTree.cpp
 * @brief Implementarea sistemului de National Focus: definirea focus-urilor, pornire, progres zilnic si efect la final.
 *
 * Contine:
 *  - clasa Focus: un focus individual (nume, durata, efect, stare completat)
 *  - clasa FocusTree: colectia de focus-uri, focus-ul activ (index) si progresul acestuia
 *
 * Flux:
 *  - startFocus(index): porneste un focus daca nu exista deja unul activ si daca indexul e valid/necompletat
 *  - tickRaw(): avanseaza progresul cu 1 zi; cand se finalizeaza, marcheaza focus-ul completat si
 *               returneaza efectul (int) pentru a fi aplicat de logica jocului (ex: Country::simulateDay)
 */

#include "../headers/FocusTree.h"
#include <iostream>

/**
 * @brief Construieste un focus individual.
 * @param name Numele focus-ului.
 * @param daysRequired Numarul de zile necesare pentru completare.
 * @param effect Efectul aplicat la completare (FocusEffectType).
 */
Focus::Focus(std::string name, int daysRequired, FocusEffectType effect)
    : name_(std::move(name)), daysRequired_(daysRequired), effect_(effect) {
}

/**
 * @brief Returneaza numele focus-ului.
 * @return Referinta const la string-ul numelui.
 */
const std::string &Focus::name() const { return name_; }

/**
 * @brief Returneaza numarul de zile necesare pentru completare.
 * @return Zilele necesare.
 */
int Focus::days() const { return daysRequired_; }

/**
 * @brief Returneaza efectul focus-ului.
 * @return Tipul efectului asociat focus-ului.
 */
FocusEffectType Focus::effect() const { return effect_; }

/**
 * @brief Marcheaza focus-ul ca fiind completat.
 */
void Focus::markCompleted() { completed_ = true; }

/**
 * @brief Constructor FocusTree: initializeaza lista de focus-uri disponibile.
 *
 * Defineste 4 focus-uri, fiecare cu durata (zile) si efect diferit.
 */
FocusTree::FocusTree() {
    focuses.emplace_back("Industrial Expansion", 35, FocusEffectType::AddCiv);
    focuses.emplace_back("Military Buildup", 35, FocusEffectType::AddMil);
    focuses.emplace_back("Infrastructure Effort", 25, FocusEffectType::AddInfra);
    focuses.emplace_back("Dockyard Development", 30, FocusEffectType::AddDockyard);
}

/**
 * @brief Porneste un focus dupa index.
 *
 * Reguli:
 *  - nu poti porni daca exista deja un focus activ (currentIndex != -1)
 *  - index trebuie sa fie in [0, focuses.size())
 *  - focus-ul nu trebuie sa fie deja completat
 *
 * Daca porneste, reseteaza progresul la 0 si seteaza currentIndex.
 *
 * @param index Indexul focus-ului din vectorul intern.
 * @return true daca a pornit cu succes, false altfel.
 */
bool FocusTree::startFocus(int index) {
    if (currentIndex != -1) {
        std::cerr << "[!] A focus is already running.\n";
        return false;
    }
    if (index < 0 || index >= static_cast<int>(focuses.size())) {
        std::cerr << "[!] Invalid focus index.\n";
        return false;
    }
    // Daca e deja completat, nu il mai putem face
    if (focuses[index].isCompleted()) {
        std::cerr << "[!] Focus already completed.\n";
        return false;
    }

    currentIndex = index;
    progress = 0;
    std::cout << "[FOCUS] Started: " << focuses[index].name() << "\n";
    return true;
}

/**
 * @brief Returneaza numele focus-ului activ (sau "None" daca nu exista unul activ).
 * @return String cu numele focus-ului activ sau "None".
 */
std::string FocusTree::getActiveFocusName() const {
    return currentIndex == -1 ? "None" : focuses[currentIndex].name();
}

/**
 * @brief Avanseaza cu 1 zi progresul focus-ului activ si intoarce efectul brut daca se finalizeaza.
 *
 * Daca nu exista focus activ, returneaza -1.
 * Daca focus-ul se finalizeaza in aceasta zi:
 *  - marcheaza focus-ul completat
 *  - reseteaza currentIndex la -1 (nu mai e activ)
 *  - returneaza efectul ca int (pentru a fi aplicat in alta parte)
 *
 * @return -1 daca nu s-a completat nimic; altfel codul efectului (int).
 */
int FocusTree::tickRaw() {
    if (currentIndex == -1) return -1;
    progress++;
    if (progress >= focuses[currentIndex].days()) {
        auto eff = static_cast<int>(focuses[currentIndex].effect());
        focuses[currentIndex].markCompleted();
        std::cout << "[FOCUS COMPLETED] " << focuses[currentIndex].name() << "\n";
        currentIndex = -1;
        return eff;
    }
    return -1;
}
