#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics.hpp>
#include <string>

class Interface {
public:
    Interface(const std::string& title = "Country Manager UI");
    void run();

private:
    sf::RenderWindow window;

    sf::Texture mapTexture;
    sf::Sprite mapSprite;

    void handleEvents();
    void render();
};

#endif // INTERFACE_H
