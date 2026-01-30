#include "../headers/Interface.h"
#include "../headers/GameExceptions.h"
#include <iostream>

Interface::Interface(Engine &eng, const std::string &title) : engine(eng) {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, title, sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    if (!mapTexture.loadFromFile("images/harta.png")) throw AssetLoadException("harta.png");
    mapSprite.setTexture(mapTexture);
    float scaleX = static_cast<float>(desktop.width) / mapTexture.getSize().x;
    float scaleY = static_cast<float>(desktop.height) / mapTexture.getSize().y;
    mapSprite.setScale(scaleX, scaleY);

    if (!font.loadFromFile("fonts/arial.ttf")) std::cerr << "Eroare font\n";

    dayText.setFont(font);
    dayText.setCharacterSize(24);
    dayText.setPosition(20.f, 20.f);
    dayText.setString("Day: 0");
    infoText.setFont(font);
    infoText.setCharacterSize(30);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition(desktop.width / 2.f - 200.f, 20.f);
    infoText.setString("CLICK ON A PROVINCE TO SELECT COUNTRY");

    if (!steelTex.loadFromFile("images/steel.png")) std::cerr << "Lipseste steel.png\n";
    if (!tungstenTex.loadFromFile("images/tungsten.png")) std::cerr << "Lipseste tungsten.png\n";
    if (!aluminumTex.loadFromFile("images/aluminum.png")) std::cerr << "Lipseste aluminum.png\n";
    if (!chromiumTex.loadFromFile("images/chromium.png")) std::cerr << "Lipseste chromium.png\n";
    if (!oilTex.loadFromFile("images/oil.png")) std::cerr << "Lipseste oil.png\n";
    if (!civTex.loadFromFile("images/civ.png")) std::cerr << "Lipseste civ.png\n";
    if (!milTex.loadFromFile("images/mil.png")) std::cerr << "Lipseste mil.png\n";
    if (!infraTex.loadFromFile("images/infra.png")) std::cerr << "Lipseste infra.png\n";
    if (!dockyardTex.loadFromFile("images/dockyard.png")) std::cerr << "Lipseste dockyard.png\n";
    if (!airfieldTex.loadFromFile("images/airfield.png")) std::cerr << "Lipseste airfield.png\n";
    if (!manpowerTex.loadFromFile("images/manpower.png")) std::cerr << "Lipseste manpower.png\n";
    if (!fuelStockTex.loadFromFile("images/fuel_stockpile.png")) std::cerr << "Lipseste fuel.png\n";

    roFuelSprite.setTexture(fuelStockTex);
    roFuelSprite.setScale(0.6f, 0.6f);
    roFuelSprite.setPosition(20.f, 60.f);
    roManpowerSprite.setTexture(manpowerTex);
    roManpowerSprite.setScale(0.6f, 0.6f);
    roManpowerSprite.setPosition(20.f, 100.f);
    roFuelText.setFont(font);
    roFuelText.setCharacterSize(18);
    roFuelText.setPosition(60.f, 60.f);
    roFuelText.setString("0");
    roManpowerText.setFont(font);
    roManpowerText.setCharacterSize(18);
    roManpowerText.setPosition(60.f, 100.f);
    roManpowerText.setString("0");

    huFuelSprite.setTexture(fuelStockTex);
    huFuelSprite.setScale(0.6f, 0.6f);
    huFuelSprite.setPosition(20.f, 60.f);
    huManpowerSprite.setTexture(manpowerTex);
    huManpowerSprite.setScale(0.6f, 0.6f);
    huManpowerSprite.setPosition(20.f, 100.f);
    huFuelText.setFont(font);
    huFuelText.setCharacterSize(18);
    huFuelText.setPosition(60.f, 60.f);
    huFuelText.setString("0");
    huManpowerText.setFont(font);
    huManpowerText.setCharacterSize(18);
    huManpowerText.setPosition(60.f, 100.f);
    huManpowerText.setString("0");

    if (!focusBgTex.loadFromFile("images/FocusTreeBG.png")) std::cerr << "Lipseste FocusTreeBG.png\n";
    focusBgSprite.setTexture(focusBgTex);
    float bgScaleX = static_cast<float>(desktop.width) / focusBgTex.getSize().x;
    float bgScaleY = static_cast<float>(desktop.height) / focusBgTex.getSize().y;
    focusBgSprite.setScale(bgScaleX, bgScaleY);

    for (int i = 0; i < 4; ++i) {
        std::string path = "images/Focus" + std::to_string(i + 1) + ".png";
        if (!focusIconTex[i].loadFromFile(path)) std::cerr << "Lipseste " << path << "\n";
        focusIconSprites[i].setTexture(focusIconTex[i]);
        focusNameTexts[i].setFont(font);
        focusNameTexts[i].setCharacterSize(24);
        focusNameTexts[i].setFillColor(sf::Color::White);
        focusNameTexts[i].setOutlineColor(sf::Color::Black);
        focusNameTexts[i].setOutlineThickness(2.f);
    }

    setupFocusUI();
    setupProvinceUI();
}

void Interface::setupFocusUI() {
    sf::Vector2u windowSize = window.getSize();
    float centerX = windowSize.x / 2.0f;
    float centerY = windowSize.y / 2.0f;
    float spacing = 300.f;
    float startX = centerX - (1.5f * spacing);

    for (int i = 0; i < 4; ++i) {
        sf::Vector2u size = focusIconTex[i].getSize();
        focusIconSprites[i].setOrigin(size.x / 2.f, size.y / 2.f);
        focusIconSprites[i].setPosition(startX + i * spacing, centerY);
        focusNameTexts[i].setPosition(startX + i * spacing - 60.f, centerY + 80.f);

        if (i == 0) focusNameTexts[i].setString("Industrial");
        if (i == 1) focusNameTexts[i].setString("Military");
        if (i == 2) focusNameTexts[i].setString("Infrastructure");
        if (i == 3) focusNameTexts[i].setString("Naval Base");
    }
}

sf::Vector2f Interface::getIconPositionFor(const std::string &countryName, const std::string &provinceName,
                                           StatKind kind) {
    if (countryName == "Romania" && provinceName == "Moldavia") {
        switch (kind) {
            case StatKind::Steel: return {1500.f, 1150.f};
            case StatKind::Tungsten: return {1500.f, 1180.f};
            case StatKind::Aluminum: return {1500.f, 1210.f};
            case StatKind::Chromium: return {1500.f, 1240.f};
            case StatKind::Oil: return {1500.f, 1270.f};
            case StatKind::Civ: return {1540.f, 1150.f};
            case StatKind::Mil: return {1540.f, 1180.f};
            case StatKind::Infra: return {1540.f, 1210.f};
            case StatKind::Dockyard: return {1540.f, 1240.f};
            case StatKind::Airfield: return {1540.f, 1270.f};
        }
    }
    if (countryName == "Romania" && provinceName == "Wallachia") {
        switch (kind) {
            case StatKind::Steel: return {1900.f, 600.f};
            case StatKind::Tungsten: return {1900.f, 630.f};
            case StatKind::Aluminum: return {1900.f, 660.f};
            case StatKind::Chromium: return {1900.f, 690.f};
            case StatKind::Oil: return {1900.f, 720.f};
            case StatKind::Civ: return {1940.f, 600.f};
            case StatKind::Mil: return {1940.f, 630.f};
            case StatKind::Infra: return {1940.f, 660.f};
            case StatKind::Dockyard: return {1940.f, 690.f};
            case StatKind::Airfield: return {1940.f, 720.f};
        }
    }
    if (countryName == "Romania" && provinceName == "Transylvania") {
        switch (kind) {
            case StatKind::Steel: return {1200.f, 450.f};
            case StatKind::Tungsten: return {1200.f, 480.f};
            case StatKind::Aluminum: return {1200.f, 510.f};
            case StatKind::Chromium: return {1200.f, 540.f};
            case StatKind::Oil: return {1200.f, 570.f};
            case StatKind::Civ: return {1240.f, 450.f};
            case StatKind::Mil: return {1240.f, 480.f};
            case StatKind::Infra: return {1240.f, 510.f};
            case StatKind::Dockyard: return {1240.f, 540.f};
            case StatKind::Airfield: return {1240.f, 570.f};
        }
    }
    if (countryName == "Hungary" && provinceName == "Alfold") {
        switch (kind) {
            case StatKind::Steel: return {300.f, 500.f};
            case StatKind::Tungsten: return {300.f, 530.f};
            case StatKind::Aluminum: return {300.f, 560.f};
            case StatKind::Chromium: return {300.f, 590.f};
            case StatKind::Oil: return {300.f, 620.f};
            case StatKind::Civ: return {340.f, 500.f};
            case StatKind::Mil: return {340.f, 530.f};
            case StatKind::Infra: return {340.f, 560.f};
            case StatKind::Dockyard: return {340.f, 590.f};
            case StatKind::Airfield: return {340.f, 620.f};
        }
    }
    if (countryName == "Hungary" && provinceName == "Transdanubia") {
        switch (kind) {
            case StatKind::Steel: return {650.f, 450.f};
            case StatKind::Tungsten: return {650.f, 480.f};
            case StatKind::Aluminum: return {650.f, 510.f};
            case StatKind::Chromium: return {650.f, 540.f};
            case StatKind::Oil: return {650.f, 570.f};
            case StatKind::Civ: return {690.f, 450.f};
            case StatKind::Mil: return {690.f, 480.f};
            case StatKind::Infra: return {690.f, 510.f};
            case StatKind::Dockyard: return {690.f, 540.f};
            case StatKind::Airfield: return {690.f, 570.f};
        }
    }
    return {50.f, 50.f};
}

void Interface::setupProvinceUI() {
    const auto &countries = engine.getCountries();
    provinceUI.resize(countries.size());
    clickZones.clear();

    for (std::size_t ci = 0; ci < countries.size(); ++ci) {
        const auto &provs = countries[ci].getProvinces();
        provinceUI[ci].resize(provs.size());
        const std::string &countryName = countries[ci].getName();

        for (std::size_t pi = 0; pi < provs.size(); ++pi) {
            ProvinceUI &pui = provinceUI[ci][pi];
            const Province &prov = provs[pi];
            const std::string &provName = prov.getName();

            sf::Vector2f refPos = getIconPositionFor(countryName, provName, StatKind::Steel);
            ClickZone zone;
            zone.bounds = sf::FloatRect(refPos.x - 150.f, refPos.y - 150.f, 700.f, 800.f);
            zone.countryIndex = static_cast<int>(ci);
            clickZones.push_back(zone);

            auto makeIcon = [&](StatKind kind, sf::Texture &tex) {
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

            makeIcon(StatKind::Steel, steelTex);
            makeIcon(StatKind::Tungsten, tungstenTex);
            makeIcon(StatKind::Aluminum, aluminumTex);
            makeIcon(StatKind::Chromium, chromiumTex);
            makeIcon(StatKind::Oil, oilTex);
            makeIcon(StatKind::Civ, civTex);
            makeIcon(StatKind::Mil, milTex);
            makeIcon(StatKind::Infra, infraTex);
            makeIcon(StatKind::Dockyard, dockyardTex);
            makeIcon(StatKind::Airfield, airfieldTex);
        }
    }
}

void Interface::updateUI() {
    if (selectedCountryIndex == -1) {
        dayText.setString("Day: " + std::to_string(engine.getDay()) + " (PAUSED)");
        return;
    }
    dayText.setString("Day: " + std::to_string(engine.getDay()) + " (Running)");

    const auto &countries = engine.getCountries();
    std::size_t ci = static_cast<std::size_t>(selectedCountryIndex);
    if (ci >= countries.size()) return;

    const auto &provs = countries[ci].getProvinces();
    for (std::size_t pi = 0; pi < provs.size(); ++pi) {
        const Province &p = provs[pi];
        ProvinceUI &pui = provinceUI[ci][pi];
        for (auto &icon: pui.resourceIcons) {
            int value = 0;
            switch (icon.kind) {
                case StatKind::Steel: value = p.getSteel();
                    break;
                case StatKind::Tungsten: value = p.getTungsten();
                    break;
                case StatKind::Aluminum: value = p.getAluminum();
                    break;
                case StatKind::Chromium: value = p.getChromium();
                    break;
                case StatKind::Oil: value = p.getOil();
                    break;
                case StatKind::Civ: value = p.getCiv();
                    break;
                case StatKind::Mil: value = p.getMil();
                    break;
                case StatKind::Infra: value = p.getInfra();
                    break;
                case StatKind::Dockyard: value = p.getDockyards();
                    break;
                case StatKind::Airfield: value = p.getAirfields();
                    break;
            }
            if (icon.lastValue != -1) {
                if (value > icon.lastValue) icon.value.setFillColor(sf::Color::Green);
                else if (value < icon.lastValue) icon.value.setFillColor(sf::Color::Red);
                else icon.value.setFillColor(sf::Color::White);
            }
            icon.lastValue = value;
            icon.value.setString(std::to_string(value));
        }
    }
    const auto &stock = countries[ci].getResourceStockpile();
    if (countries[ci].getName() == "Romania") {
        roFuelText.setString(std::to_string(stock.getFuel()));
        roManpowerText.setString(std::to_string(stock.getManpower()));
    } else if (countries[ci].getName() == "Hungary") {
        huFuelText.setString(std::to_string(stock.getFuel()));
        huManpowerText.setString(std::to_string(stock.getManpower()));
    }
}

void Interface::handleEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
            if (selectedCountryIndex != -1) showFocusTree = !showFocusTree;
            else std::cout << "[UI] Selecteaza o tara mai intai!\n";
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            if (showFocusTree && selectedCountryIndex != -1) {
                Country &currentCountry = engine.getMutableCountries()[selectedCountryIndex];
                for (int i = 0; i < 4; ++i) {
                    if (focusIconSprites[i].getGlobalBounds().contains(mousePos)) {
                        if (currentCountry.startFocus(i)) std::cout << "Focus " << i << " started!\n";
                    }
                }
            } else if (!showFocusTree) {
                for (const auto &zone: clickZones) {
                    if (zone.bounds.contains(mousePos)) {
                        selectedCountryIndex = zone.countryIndex;
                        infoText.setString(
                            "Selected: " + engine.getCountries()[selectedCountryIndex].getName() +
                            " (Press Q for Focus)");
                        break;
                    }
                }
            }
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            if (showFocusTree) showFocusTree = false;
            else {
                selectedCountryIndex = -1;
                infoText.setString("PAUSED - CLICK TO SELECT");
            }
        }
    }
}

void Interface::render() {
    window.clear();
    if (showFocusTree && selectedCountryIndex != -1) {
        window.draw(focusBgSprite);
        const FocusTree &tree = engine.getCountries()[selectedCountryIndex].getFocusTree();
        int activeIndex = tree.getActiveFocusIndex();

        for (int i = 0; i < 4; ++i) {
            sf::Sprite &s = focusIconSprites[i];
            if (tree.isFocusCompleted(i)) s.setColor(sf::Color(100, 100, 100));
            else if (i == activeIndex) s.setColor(sf::Color(100, 255, 100));
            else if (s.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x),
                                                  static_cast<float>(sf::Mouse::getPosition(window).y))) s.setColor(
                sf::Color::White);
            else s.setColor(sf::Color(200, 200, 200));
            window.draw(s);
            window.draw(focusNameTexts[i]);
        }
        sf::Text title;
        title.setFont(font);
        title.setString("National Focus: " + engine.getCountries()[selectedCountryIndex].getName());
        title.setCharacterSize(40);
        title.setPosition(50.f, 50.f);
        title.setFillColor(sf::Color::White);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(3.f);
        window.draw(title);
    } else {
        window.draw(mapSprite);
        if (selectedCountryIndex != -1) {
            std::size_t ci = static_cast<std::size_t>(selectedCountryIndex);
            if (ci < provinceUI.size()) {
                for (auto &pui: provinceUI[ci]) {
                    for (auto &icon: pui.resourceIcons) {
                        window.draw(icon.icon);
                        window.draw(icon.value);
                    }
                }
            }
            if (engine.getCountries()[ci].getName() == "Romania") {
                window.draw(roFuelSprite);
                window.draw(roManpowerSprite);
                window.draw(roFuelText);
                window.draw(roManpowerText);
            } else if (engine.getCountries()[ci].getName() == "Hungary") {
                window.draw(huFuelSprite);
                window.draw(huManpowerSprite);
                window.draw(huFuelText);
                window.draw(huManpowerText);
            }
        }
        window.draw(dayText);
        window.draw(infoText);
    }
    window.display();
}

void Interface::run() {
    sf::Clock clock;
    float accumulator = 0.f;
    const float secondsPerDay = 1.0f;
    while (window.isOpen()) {
        handleEvents();
        if (selectedCountryIndex != -1) {
            float dt = clock.restart().asSeconds();
            accumulator += dt;
            while (accumulator >= secondsPerDay) {
                engine.simulateOneDay();
                accumulator -= secondsPerDay;
            }
            updateUI();
        } else { clock.restart(); }
        render();
    }
}
