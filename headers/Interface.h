#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Engine.h"

enum class StatKind {
    Steel, Tungsten, Aluminum, Chromium, Oil,
    Civ, Mil, Infra, Dockyard, Airfield
};

struct ResourceIconUI {
    StatKind kind;
    sf::Sprite icon;
    sf::Text value;
    int lastValue;
};

struct ProvinceUI {
    std::vector<ResourceIconUI> resourceIcons;
};

struct ClickZone {
    sf::FloatRect bounds;
    int countryIndex;
};

class Interface {
    Engine& engine;
    sf::RenderWindow window;

    sf::Texture mapTexture;
    sf::Sprite mapSprite;
    sf::Font font;
    sf::Text dayText;
    sf::Text infoText;
    sf::Text alertText;

    sf::Texture steelTex, tungstenTex, aluminumTex, chromiumTex, oilTex;
    sf::Texture civTex, milTex, infraTex, dockyardTex, airfieldTex;
    sf::Texture manpowerTex, fuelStockTex;

    sf::Sprite roFuelSprite, roManpowerSprite;
    sf::Sprite huFuelSprite, huManpowerSprite;
    sf::Text roFuelText, roManpowerText;
    sf::Text huFuelText, huManpowerText;

    std::vector<std::vector<ProvinceUI>> provinceUI;
    std::vector<ClickZone> clickZones;
    int selectedCountryIndex = -1;

    bool showFocusTree = false;
    sf::Texture focusBgTex;
    sf::Sprite focusBgSprite;
    sf::Texture focusIconTex[4];
    sf::Sprite focusIconSprites[4];
    sf::Text focusNameTexts[4];

    bool showConstruction = false;
    sf::RectangleShape constructionPanel;
    sf::Sprite buildIcons[5];
    sf::Text buildIconTexts[5];
    int selectedBuildingType = -1;
    sf::Text queueTitleText;

    void handleEvents();
    void render();
    void updateUI();
    void setupProvinceUI();
    void setupFocusUI();
    void setupConstructionUI();
    sf::Vector2f getIconPositionFor(const std::string& countryName, const std::string& provinceName, StatKind kind);

public:
    explicit Interface(Engine& eng, const std::string& title = "Country Manager UI");
    void run();
};

#endif