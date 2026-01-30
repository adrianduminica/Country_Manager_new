#include "../headers/FocusTree.h"
#include <iostream>

Focus::Focus(std::string name, int daysRequired, FocusEffectType effect)
    : name_(std::move(name)), daysRequired_(daysRequired), effect_(effect) {
}

const std::string &Focus::name() const { return name_; }
int Focus::days() const { return daysRequired_; }
FocusEffectType Focus::effect() const { return effect_; }
void Focus::markCompleted() { completed_ = true; }

FocusTree::FocusTree() {
    focuses.emplace_back("Industrial Expansion", 35, FocusEffectType::AddCiv);
    focuses.emplace_back("Military Buildup", 35, FocusEffectType::AddMil);
    focuses.emplace_back("Infrastructure Effort", 25, FocusEffectType::AddInfra);
    focuses.emplace_back("Dockyard Development", 30, FocusEffectType::AddDockyard);
}

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

std::string FocusTree::getActiveFocusName() const {
    return currentIndex == -1 ? "None" : focuses[currentIndex].name();
}

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
