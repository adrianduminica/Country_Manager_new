#ifndef FOCUS_TREE_H
#define FOCUS_TREE_H

#include <string>
#include <vector>

enum class FocusEffectType { AddCiv, AddMil, AddInfra, AddDockyard };

class Focus {
    std::string name_;
    int daysRequired_;
    FocusEffectType effect_;
    bool completed_ = false;

public:
    Focus(std::string name, int daysRequired, FocusEffectType effect);

    const std::string &name() const;

    int days() const;

    FocusEffectType effect() const;

    void markCompleted();

    // --- MODIFICARE: Verificare status ---
    bool isCompleted() const { return completed_; }
};

class FocusTree {
    std::vector<Focus> focuses;
    int currentIndex = -1;
    int progress = 0;

public:
    FocusTree();

    bool startFocus(int index);

    std::string getActiveFocusName() const;

    int tickRaw();

    // --- MODIFICARE: Getteri UI ---
    int getActiveFocusIndex() const { return currentIndex; }

    bool isFocusCompleted(int index) const {
        if (index >= 0 && index < static_cast<int>(focuses.size())) {
            return focuses[index].isCompleted();
        }
        return false;
    }
};

#endif
