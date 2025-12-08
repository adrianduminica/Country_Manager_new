#include "Interface.h"
#include <iostream>

Interface::Interface(Engine& eng, const std::string& title)
    : engine(eng)
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, title, sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    if (!mapTexture.loadFromFile("images/harta.png")) {
        std::cerr << "Eroare: Nu se poate incarca images/harta.png\n";
    }
    mapSprite.setTexture(mapTexture);
    float scaleX = static_cast<float>(desktop.width)  / mapTexture.getSize().x;
    float scaleY = static_cast<float>(desktop.height) / mapTexture.getSize().y;
    mapSprite.setScale(scaleX, scaleY);

    if (!font.loadFromFile("fonts/arial.ttf")) {
        std::cerr << "Eroare: Nu se poate incarca fonts/arial.ttf\n";
    }

    dayText.setFont(font);
    dayText.setCharacterSize(24);
    dayText.setFillColor(sf::Color::White);
    dayText.setOutlineColor(sf::Color::Black);
    dayText.setOutlineThickness(2.f);
    dayText.setPosition(20.f, 20.f);
    dayText.setString("Day: 0");

    if (!steelTex.loadFromFile("images/steel.png"))       std::cerr << "Lipseste images/steel.png\n";
    if (!tungstenTex.loadFromFile("images/tungsten.png")) std::cerr << "Lipseste images/tungsten.png\n";
    if (!aluminumTex.loadFromFile("images/aluminum.png")) std::cerr << "Lipseste images/aluminum.png\n";
    if (!chromiumTex.loadFromFile("images/chromium.png")) std::cerr << "Lipseste images/chromium.png\n";
    if (!oilTex.loadFromFile("images/oil.png"))           std::cerr << "Lipseste images/oil.png\n";

    if (!civTex.loadFromFile("images/civ.png"))           std::cerr << "Lipseste images/civ.png\n";
    if (!milTex.loadFromFile("images/mil.png"))           std::cerr << "Lipseste images/mil.png\n";
    if (!infraTex.loadFromFile("images/infra.png"))       std::cerr << "Lipseste images/infra.png\n";
    if (!dockyardTex.loadFromFile("images/dockyard.png")) std::cerr << "Lipseste images/dockyard.png\n";
    if (!airfieldTex.loadFromFile("images/airfield.png")) std::cerr << "Lipseste images/airfield.png\n";

    if (!manpowerTex.loadFromFile("images/manpower.png"))       std::cerr << "Lipseste images/manpower.png\n";
    if (!fuelStockTex.loadFromFile("images/fuel_stockpile.png")) std::cerr << "Lipseste images/fuel_stockpile.png\n";

    roFuelSprite.setTexture(fuelStockTex);
    roFuelSprite.setScale(0.6f, 0.6f);
    roFuelSprite.setPosition(20.f, 60.f);

    roManpowerSprite.setTexture(manpowerTex);
    roManpowerSprite.setScale(0.6f, 0.6f);
    roManpowerSprite.setPosition(20.f, 100.f);

    roFuelText.setFont(font);
    roFuelText.setCharacterSize(18);
    roFuelText.setFillColor(sf::Color::White);
    roFuelText.setOutlineColor(sf::Color::Black);
    roFuelText.setOutlineThickness(1.f);
    roFuelText.setPosition(60.f, 60.f);
    roFuelText.setString("0");

    roManpowerText.setFont(font);
    roManpowerText.setCharacterSize(18);
    roManpowerText.setFillColor(sf::Color::White);
    roManpowerText.setOutlineColor(sf::Color::Black);
    roManpowerText.setOutlineThickness(1.f);
    roManpowerText.setPosition(60.f, 100.f);
    roManpowerText.setString("0");

    huFuelSprite.setTexture(fuelStockTex);
    huFuelSprite.setScale(0.6f, 0.6f);
    huFuelSprite.setPosition(220.f, 60.f);

    huManpowerSprite.setTexture(manpowerTex);
    huManpowerSprite.setScale(0.6f, 0.6f);
    huManpowerSprite.setPosition(220.f, 100.f);

    huFuelText.setFont(font);
    huFuelText.setCharacterSize(18);
    huFuelText.setFillColor(sf::Color::White);
    huFuelText.setOutlineColor(sf::Color::Black);
    huFuelText.setOutlineThickness(1.f);
    huFuelText.setPosition(260.f, 60.f);
    huFuelText.setString("0");

    huManpowerText.setFont(font);
    huManpowerText.setCharacterSize(18);
    huManpowerText.setFillColor(sf::Color::White);
    huManpowerText.setOutlineColor(sf::Color::Black);
    huManpowerText.setOutlineThickness(1.f);
    huManpowerText.setPosition(260.f, 100.f);
    huManpowerText.setString("0");

    setupProvinceUI();
}

sf::Vector2f Interface::getIconPositionFor(const std::string& countryName,
                                           const std::string& provinceName,
                                           StatKind kind)
{
    if (countryName == "Romania" && provinceName == "Moldavia") {
        switch (kind) {
            case StatKind::Steel:    return { 1500.f, 1150.f };
            case StatKind::Tungsten: return { 1500.f, 1180.f };
            case StatKind::Aluminum: return { 1500.f, 1210.f };
            case StatKind::Chromium: return { 1500.f, 1240.f };
            case StatKind::Oil:      return { 1500.f, 1270.f };

            case StatKind::Civ:      return { 1540.f, 1150.f };
            case StatKind::Mil:      return { 1540.f, 1180.f };
            case StatKind::Infra:    return { 1540.f, 1210.f };
            case StatKind::Dockyard: return { 1540.f, 1240.f };
            case StatKind::Airfield: return { 1540.f, 1270.f };
        }
    }

    if (countryName == "Romania" && provinceName == "Wallachia") {
        switch (kind) {
            case StatKind::Steel:    return { 1900.f, 600.f };
            case StatKind::Tungsten: return { 1900.f, 630.f };
            case StatKind::Aluminum: return { 1900.f, 660.f };
            case StatKind::Chromium: return { 1900.f, 690.f };
            case StatKind::Oil:      return { 1900.f, 720.f };

            case StatKind::Civ:      return { 1940.f, 600.f };
            case StatKind::Mil:      return { 1940.f, 630.f };
            case StatKind::Infra:    return { 1940.f, 660.f };
            case StatKind::Dockyard: return { 1940.f, 690.f };
            case StatKind::Airfield: return { 1940.f, 720.f };
        }
    }
    if (countryName == "Romania" && provinceName == "Transylvania") {
        switch (kind) {
            case StatKind::Steel:    return { 1200.f, 450.f };
            case StatKind::Tungsten: return { 1200.f, 480.f };
            case StatKind::Aluminum: return { 1200.f, 510.f };
            case StatKind::Chromium: return { 1200.f, 540.f };
            case StatKind::Oil:      return { 1200.f, 570.f };

            case StatKind::Civ:      return { 1240.f, 450.f };
            case StatKind::Mil:      return { 1240.f, 480.f };
            case StatKind::Infra:    return { 1240.f, 510.f };
            case StatKind::Dockyard: return { 1240.f, 540.f };
            case StatKind::Airfield: return { 1240.f, 570.f };
        }
    }

    if (countryName == "Hungary" && provinceName == "Alfold") {
        switch (kind) {
            case StatKind::Steel:    return { 300.f, 500.f };
            case StatKind::Tungsten: return { 300.f, 530.f };
            case StatKind::Aluminum: return { 300.f, 560.f };
            case StatKind::Chromium: return { 300.f, 590.f };
            case StatKind::Oil:      return { 300.f, 620.f };

            case StatKind::Civ:      return { 340.f, 500.f };
            case StatKind::Mil:      return { 340.f, 530.f };
            case StatKind::Infra:    return { 340.f, 560.f };
            case StatKind::Dockyard: return { 340.f, 590.f };
            case StatKind::Airfield: return { 340.f, 620.f };
        }
    }

    if (countryName == "Hungary" && provinceName == "Transdanubia") {
        switch (kind) {
            case StatKind::Steel:    return { 650.f, 450.f };
            case StatKind::Tungsten: return { 650.f, 480.f };
            case StatKind::Aluminum: return { 650.f, 510.f };
            case StatKind::Chromium: return { 650.f, 540.f };
            case StatKind::Oil:      return { 650.f, 570.f };

            case StatKind::Civ:      return { 690.f, 450.f };
            case StatKind::Mil:      return { 690.f, 480.f };
            case StatKind::Infra:    return { 690.f, 510.f };
            case StatKind::Dockyard: return { 690.f, 540.f };
            case StatKind::Airfield: return { 690.f, 570.f };
        }
    }


    return { 50.f, 50.f };
}

void Interface::setupProvinceUI() {
    const auto& countries = engine.getCountries();
    provinceUI.resize(countries.size());

    for (std::size_t ci = 0; ci < countries.size(); ++ci) {
        const auto& provs = countries[ci].getProvinces();
        provinceUI[ci].resize(provs.size());

        const std::string& countryName = countries[ci].getName();

        for (std::size_t pi = 0; pi < provs.size(); ++pi) {
            ProvinceUI& pui = provinceUI[ci][pi];
            const Province& prov = provs[pi];
            const std::string& provName = prov.getName();

            auto makeIcon = [&](StatKind kind, sf::Texture& tex) {
                ResourceIconUI ui;
                ui.kind = kind;
                ui.icon.setTexture(tex);
                ui.icon.setScale(0.5f, 0.5f);

                sf::Vector2f pos = getIconPositionFor(countryName, provName, kind);
                ui.icon.setPosition(pos);

                ui.value.setFont(font);
                ui.value.setCharacterSize(16);
                ui.value.setFillColor(sf::Color::White);
                ui.value.setString("0");
                ui.value.setPosition(pos.x + 32.f, pos.y + 4.f);

                ui.lastValue = -1;

                pui.resourceIcons.push_back(ui);
            };

            makeIcon(StatKind::Steel,    steelTex);
            makeIcon(StatKind::Tungsten, tungstenTex);
            makeIcon(StatKind::Aluminum, aluminumTex);
            makeIcon(StatKind::Chromium, chromiumTex);
            makeIcon(StatKind::Oil,      oilTex);

            makeIcon(StatKind::Civ,      civTex);
            makeIcon(StatKind::Mil,      milTex);
            makeIcon(StatKind::Infra,    infraTex);
            makeIcon(StatKind::Dockyard, dockyardTex);
            makeIcon(StatKind::Airfield, airfieldTex);
        }
    }
}

void Interface::updateUI() {
    dayText.setString("Day: " + std::to_string(engine.getDay()));

    const auto& countries = engine.getCountries();

    for (std::size_t ci = 0; ci < countries.size(); ++ci) {
        const auto& provs = countries[ci].getProvinces();

        for (std::size_t pi = 0; pi < provs.size(); ++pi) {
            const Province& p = provs[pi];
            ProvinceUI& pui   = provinceUI[ci][pi];

            for (auto& icon : pui.resourceIcons) {
                int value = 0;
                switch (icon.kind) {
                    case StatKind::Steel:    value = p.getSteel();     break;
                    case StatKind::Tungsten: value = p.getTungsten();  break;
                    case StatKind::Aluminum: value = p.getAluminum();  break;
                    case StatKind::Chromium: value = p.getChromium();  break;
                    case StatKind::Oil:      value = p.getOil();       break;
                    case StatKind::Civ:      value = p.getCiv();       break;
                    case StatKind::Mil:      value = p.getMil();       break;
                    case StatKind::Infra:    value = p.getInfra();     break;
                    case StatKind::Dockyard: value = p.getDockyards(); break;
                    case StatKind::Airfield: value = p.getAirfields(); break;
                }

                if (icon.lastValue == -1) {
                    icon.value.setFillColor(sf::Color::White);
                } else {
                    if (value > icon.lastValue) {
                        icon.value.setFillColor(sf::Color::Green);
                    } else if (value < icon.lastValue) {
                        icon.value.setFillColor(sf::Color::Red);
                    } else {
                        icon.value.setFillColor(sf::Color::White);
                    }
                }

                icon.lastValue = value;
                icon.value.setString(std::to_string(value));
            }
        }
    }

    for (const auto& c : countries) {
        const auto& stock = c.getResourceStockpile();
        if (c.getName() == "Romania") {
            roFuelText.setString(std::to_string(stock.getFuel()));
            roManpowerText.setString(std::to_string(stock.getManpower()));
        } else if (c.getName() == "Hungary") {
            huFuelText.setString(std::to_string(stock.getFuel()));
            huManpowerText.setString(std::to_string(stock.getManpower()));
        }
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

    for (auto& countryUI : provinceUI) {
        for (auto& pui : countryUI) {
            for (auto& icon : pui.resourceIcons) {
                window.draw(icon.icon);
                window.draw(icon.value);
            }
        }
    }

    window.draw(dayText);

    window.draw(roFuelSprite);
    window.draw(roManpowerSprite);
    window.draw(roFuelText);
    window.draw(roManpowerText);

    window.draw(huFuelSprite);
    window.draw(huManpowerSprite);
    window.draw(huFuelText);
    window.draw(huManpowerText);

    window.display();
}

void Interface::run() {
    sf::Clock clock;
    float accumulator = 0.f;
    const float secondsPerDay = 1.0f;

    while (window.isOpen()) {
        handleEvents();

        float dt = clock.restart().asSeconds();
        accumulator += dt;

        while (accumulator >= secondsPerDay) {
            engine.simulateOneDay();
            accumulator -= secondsPerDay;
        }

        updateUI();
        render();
    }
}
