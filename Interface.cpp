#include "Interface.h"
#include <iostream>

Interface::Interface(const std::string& title) {

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, title, sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Încarcă imaginea
    if (!mapTexture.loadFromFile("images/harta.png")) {
        std::cerr << "Eroare: Nu se poate încărca harta.png\n";
    }

    mapSprite.setTexture(mapTexture);

    // Scale pentru Fullscreen
    float scaleX = static_cast<float>(desktop.width)  / mapTexture.getSize().x;
    float scaleY = static_cast<float>(desktop.height) / mapTexture.getSize().y;
    mapSprite.setScale(scaleX, scaleY);
}

void Interface::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void Interface::handleEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
            window.close();
    }
}

void Interface::render() {
    window.clear();
    window.draw(mapSprite);
    window.display();
}
