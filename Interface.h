#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Engine.h"

enum class StatKind {
    Steel, Tungsten, Aluminum, Chromium, Oil,
    Civ, Mil, Infra, Dockyard, Airfield
};

struct ResourceIconUI {
    StatKind   kind;
    sf::Sprite icon;
    sf::Text   value;
};

struct ProvinceUI {
    std::vector<ResourceIconUI> resourceIcons;
};

class Interface {
public:
    Interface(Engine& engine, const std::string& title = "Country Manager UI");
    void run();

private:
    Engine& engine;

    sf::RenderWindow window;

    sf::Texture mapTexture;
    sf::Sprite  mapSprite;

    sf::Font font;

    sf::Texture steelTex;
    sf::Texture tungstenTex;
    sf::Texture aluminumTex;
    sf::Texture chromiumTex;
    sf::Texture oilTex;
    sf::Texture civTex;
    sf::Texture milTex;
    sf::Texture infraTex;
    sf::Texture dockyardTex;
    sf::Texture airfieldTex;

    std::vector<std::vector<ProvinceUI>> provinceUI;

    sf::Text dayText;

    void handleEvents();
    void updateUI();
    void render();

    void setupProvinceUI();

    sf::Vector2f getIconPositionFor(const std::string& countryName,
                                    const std::string& provinceName,
                                    StatKind kind);
};

#endif // INTERFACE_H
