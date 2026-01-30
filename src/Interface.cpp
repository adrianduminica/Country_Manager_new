#include "../headers/Interface.h"
#include "../headers/GameExceptions.h"
#include <iostream>

Interface::Interface(Engine& eng, const std::string& title) : engine(eng) {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, title, sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    if (!mapTexture.loadFromFile("images/harta.png")) throw AssetLoadException("harta.png");
    mapSprite.setTexture(mapTexture);
    float scaleX = static_cast<float>(desktop.width) / mapTexture.getSize().x;
    float scaleY = static_cast<float>(desktop.height) / mapTexture.getSize().y;
    mapSprite.setScale(scaleX, scaleY);

    if (!font.loadFromFile("fonts/arial.ttf")) std::cerr << "Eroare font\n";

    dayText.setFont(font); dayText.setCharacterSize(24); dayText.setPosition(20.f, 20.f); dayText.setString("Day: 0");
    infoText.setFont(font); infoText.setCharacterSize(30); infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition(desktop.width / 2.f - 200.f, 20.f); infoText.setString("CLICK ON A PROVINCE TO SELECT COUNTRY");

    alertText.setFont(font);
    alertText.setCharacterSize(20);
    alertText.setFillColor(sf::Color::Red);
    alertText.setOutlineColor(sf::Color::Black);
    alertText.setOutlineThickness(1.f);
    alertText.setPosition(desktop.width / 2.f - 150.f, 60.f);

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

    roFuelSprite.setTexture(fuelStockTex); roFuelSprite.setScale(0.6f, 0.6f); roFuelSprite.setPosition(20.f, 60.f);
    roManpowerSprite.setTexture(manpowerTex); roManpowerSprite.setScale(0.6f, 0.6f); roManpowerSprite.setPosition(20.f, 100.f);
    roFuelText.setFont(font); roFuelText.setCharacterSize(18); roFuelText.setPosition(60.f, 60.f); roFuelText.setString("0");
    roManpowerText.setFont(font); roManpowerText.setCharacterSize(18); roManpowerText.setPosition(60.f, 100.f); roManpowerText.setString("0");

    huFuelSprite.setTexture(fuelStockTex); huFuelSprite.setScale(0.6f, 0.6f); huFuelSprite.setPosition(20.f, 60.f);
    huManpowerSprite.setTexture(manpowerTex); huManpowerSprite.setScale(0.6f, 0.6f); huManpowerSprite.setPosition(20.f, 100.f);
    huFuelText.setFont(font); huFuelText.setCharacterSize(18); huFuelText.setPosition(60.f, 60.f); huFuelText.setString("0");
    huManpowerText.setFont(font); huManpowerText.setCharacterSize(18); huManpowerText.setPosition(60.f, 100.f); huManpowerText.setString("0");

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

    if (!gunTex.loadFromFile("images/gun.png")) std::cerr << "Lipseste gun.png\n";
    if (!artTex.loadFromFile("images/artillery.png")) std::cerr << "Lipseste artillery.png\n";
    if (!aaTex.loadFromFile("images/antiair.png")) std::cerr << "Lipseste antiair.png\n";
    if (!casTex.loadFromFile("images/cas.png")) std::cerr << "Lipseste cas.png\n";
    if (!plusTex.loadFromFile("images/plus.png")) std::cerr << "Lipseste plus.png\n";
    if (!minusTex.loadFromFile("images/minus.png")) std::cerr << "Lipseste minus.png\n";

    setupFocusUI();
    setupProvinceUI();
    setupConstructionUI();
    setupProductionUI();
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
        if(i==0) focusNameTexts[i].setString("Industrial");
        if(i==1) focusNameTexts[i].setString("Military");
        if(i==2) focusNameTexts[i].setString("Infrastructure");
        if(i==3) focusNameTexts[i].setString("Naval Base");
    }
}

void Interface::setupConstructionUI() {
    sf::Vector2u winSize = window.getSize();
    float panelWidth = winSize.x / 3.0f;
    float panelHeight = static_cast<float>(winSize.y);

    constructionPanel.setSize(sf::Vector2f(panelWidth, panelHeight));
    constructionPanel.setTexture(&focusBgTex);
    constructionPanel.setPosition(0, 0);
    constructionPanel.setOutlineColor(sf::Color::White);
    constructionPanel.setOutlineThickness(2.f);

    sf::Texture* textures[] = { &civTex, &milTex, &infraTex, &dockyardTex, &airfieldTex };
    std::string names[] = { "Civ Factory", "Mil Factory", "Infrastructure", "Dockyard", "Airfield" };

    float startY = 100.f;
    float startX = 20.f;
    float iconSize = 64.f;

    for (int i = 0; i < 5; ++i) {
        buildIcons[i].setTexture(*textures[i]);
        float sX = iconSize / textures[i]->getSize().x;
        float sY = iconSize / textures[i]->getSize().y;
        buildIcons[i].setScale(sX, sY);
        buildIcons[i].setPosition(startX, startY + i * (iconSize + 20.f));

        buildIconTexts[i].setFont(font);
        buildIconTexts[i].setString(names[i]);
        buildIconTexts[i].setCharacterSize(18);
        buildIconTexts[i].setPosition(startX + iconSize + 10.f, startY + i * (iconSize + 20.f) + 20.f);
        buildIconTexts[i].setFillColor(sf::Color::White);
    }

    queueTitleText.setFont(font);
    queueTitleText.setString("Construction Queue:");
    queueTitleText.setCharacterSize(22);
    queueTitleText.setPosition(20.f, 600.f);
    queueTitleText.setStyle(sf::Text::Bold);
}

void Interface::setupProductionUI() {
    prodTitleText.setFont(font);
    prodTitleText.setString("Production Lines");
    prodTitleText.setCharacterSize(28);
    prodTitleText.setFillColor(sf::Color::White);
    prodTitleText.setStyle(sf::Text::Bold);

    sf::Texture* ptrs[] = { &gunTex, &artTex, &aaTex, &casTex };
    for(int i=0; i<4; ++i) {
        addNewIcons[i].setTexture(*ptrs[i]);
        float sx = 48.f / ptrs[i]->getSize().x;
        float sy = 48.f / ptrs[i]->getSize().y;
        addNewIcons[i].setScale(sx, sy);
    }
}

sf::Vector2f Interface::getIconPositionFor(const std::string& countryName, const std::string& provinceName, StatKind kind) {
    if (countryName == "Romania" && provinceName == "Moldavia") {
        switch (kind) {
            case StatKind::Steel:    return { 1900.f, 600.f }; case StatKind::Tungsten: return { 1900.f, 630.f };
            case StatKind::Aluminum: return { 1900.f, 660.f }; case StatKind::Chromium: return { 1900.f, 690.f };
            case StatKind::Oil:      return { 1900.f, 720.f }; case StatKind::Civ:      return { 1940.f, 600.f };
            case StatKind::Mil:      return { 1940.f, 630.f }; case StatKind::Infra:    return { 1940.f, 660.f };
            case StatKind::Dockyard: return { 1940.f, 690.f }; case StatKind::Airfield: return { 1940.f, 720.f };
        }
    }
    if (countryName == "Romania" && provinceName == "Wallachia") {
         switch (kind) {
            case StatKind::Steel:    return { 1500.f, 1150.f }; case StatKind::Tungsten: return { 1500.f, 1180.f };
            case StatKind::Aluminum: return { 1500.f, 1210.f }; case StatKind::Chromium: return { 1500.f, 1240.f };
            case StatKind::Oil:      return { 1500.f, 1270.f }; case StatKind::Civ:      return { 1540.f, 1150.f };
            case StatKind::Mil:      return { 1540.f, 1180.f }; case StatKind::Infra:    return { 1540.f, 1210.f };
            case StatKind::Dockyard: return { 1540.f, 1240.f }; case StatKind::Airfield: return { 1540.f, 1270.f };
        }
    }
    if (countryName == "Romania" && provinceName == "Transylvania") {
        switch (kind) {
            case StatKind::Steel:    return { 1200.f, 450.f }; case StatKind::Tungsten: return { 1200.f, 480.f };
            case StatKind::Aluminum: return { 1200.f, 510.f }; case StatKind::Chromium: return { 1200.f, 540.f };
            case StatKind::Oil:      return { 1200.f, 570.f }; case StatKind::Civ:      return { 1240.f, 450.f };
            case StatKind::Mil:      return { 1240.f, 480.f }; case StatKind::Infra:    return { 1240.f, 510.f };
            case StatKind::Dockyard: return { 1240.f, 540.f }; case StatKind::Airfield: return { 1240.f, 570.f };
        }
    }
    if (countryName == "Hungary" && provinceName == "Alfold") {
        switch (kind) {
            case StatKind::Steel:    return { 300.f, 500.f }; case StatKind::Tungsten: return { 300.f, 530.f };
            case StatKind::Aluminum: return { 300.f, 560.f }; case StatKind::Chromium: return { 300.f, 590.f };
            case StatKind::Oil:      return { 300.f, 620.f }; case StatKind::Civ:      return { 340.f, 500.f };
            case StatKind::Mil:      return { 340.f, 530.f }; case StatKind::Infra:    return { 340.f, 560.f };
            case StatKind::Dockyard: return { 340.f, 590.f }; case StatKind::Airfield: return { 340.f, 620.f };
        }
    }
    if (countryName == "Hungary" && provinceName == "Transdanubia") {
        switch (kind) {
            case StatKind::Steel:    return { 650.f, 450.f }; case StatKind::Tungsten: return { 650.f, 480.f };
            case StatKind::Aluminum: return { 650.f, 510.f }; case StatKind::Chromium: return { 650.f, 540.f };
            case StatKind::Oil:      return { 650.f, 570.f }; case StatKind::Civ:      return { 690.f, 450.f };
            case StatKind::Mil:      return { 690.f, 480.f }; case StatKind::Infra:    return { 690.f, 510.f };
            case StatKind::Dockyard: return { 690.f, 540.f }; case StatKind::Airfield: return { 690.f, 570.f };
        }
    }
    return { 50.f, 50.f };
}

void Interface::setupProvinceUI() {
    const auto& countries = engine.getCountries();
    provinceUI.resize(countries.size());
    clickZones.clear();

    for (std::size_t ci = 0; ci < countries.size(); ++ci) {
        const auto& provs = countries[ci].getProvinces();
        provinceUI[ci].resize(provs.size());
        const std::string& countryName = countries[ci].getName();

        for (std::size_t pi = 0; pi < provs.size(); ++pi) {
            ProvinceUI& pui = provinceUI[ci][pi];
            const Province& prov = provs[pi];
            const std::string& provName = prov.getName();

            sf::Vector2f refPos = getIconPositionFor(countryName, provName, StatKind::Steel);
            ClickZone zone;
            zone.bounds = sf::FloatRect(refPos.x - 150.f, refPos.y - 150.f, 400.f, 400.f);
            zone.countryIndex = static_cast<int>(ci);
            clickZones.push_back(zone);

            auto makeIcon = [&](StatKind kind, sf::Texture& tex) {
                ResourceIconUI ui; ui.kind = kind; ui.icon.setTexture(tex); ui.icon.setScale(0.5f, 0.5f);
                sf::Vector2f pos = getIconPositionFor(countryName, provName, kind);
                ui.icon.setPosition(pos);
                ui.value.setFont(font); ui.value.setCharacterSize(16); ui.value.setFillColor(sf::Color::White);
                ui.value.setString("0"); ui.value.setPosition(pos.x + 32.f, pos.y + 4.f);
                ui.lastValue = -1;
                pui.resourceIcons.push_back(ui);
            };

            makeIcon(StatKind::Steel,    steelTex); makeIcon(StatKind::Tungsten, tungstenTex);
            makeIcon(StatKind::Aluminum, aluminumTex); makeIcon(StatKind::Chromium, chromiumTex);
            makeIcon(StatKind::Oil,      oilTex); makeIcon(StatKind::Civ,      civTex);
            makeIcon(StatKind::Mil,      milTex); makeIcon(StatKind::Infra,    infraTex);
            makeIcon(StatKind::Dockyard, dockyardTex); makeIcon(StatKind::Airfield, airfieldTex);
        }
    }
}

void Interface::updateUI() {
    if (selectedCountryIndex == -1) { dayText.setString("Day: " + std::to_string(engine.getDay()) + " (PAUSED)"); return; }
    dayText.setString("Day: " + std::to_string(engine.getDay()) + " (Running)");

    const auto& countries = engine.getCountries();
    std::size_t ci = static_cast<std::size_t>(selectedCountryIndex);
    if (ci >= countries.size()) return;

    const auto& provs = countries[ci].getProvinces();
    for (std::size_t pi = 0; pi < provs.size(); ++pi) {
        const Province& p = provs[pi]; ProvinceUI& pui = provinceUI[ci][pi];
        for (auto& icon : pui.resourceIcons) {
            int value = 0;
            switch (icon.kind) {
                case StatKind::Steel:    value = p.getSteel();     break; case StatKind::Tungsten: value = p.getTungsten();  break;
                case StatKind::Aluminum: value = p.getAluminum();  break; case StatKind::Chromium: value = p.getChromium();  break;
                case StatKind::Oil:      value = p.getOil();       break; case StatKind::Civ:      value = p.getCiv();       break;
                case StatKind::Mil:      value = p.getMil();       break; case StatKind::Infra:    value = p.getInfra();     break;
                case StatKind::Dockyard: value = p.getDockyards(); break; case StatKind::Airfield: value = p.getAirfields(); break;
            }
            if (icon.lastValue != -1) {
                if (value > icon.lastValue) icon.value.setFillColor(sf::Color::Green);
                else if (value < icon.lastValue) icon.value.setFillColor(sf::Color::Red);
                else icon.value.setFillColor(sf::Color::White);
            }
            icon.lastValue = value; icon.value.setString(std::to_string(value));
        }
    }
    const auto& stock = countries[ci].getResourceStockpile();
    if (countries[ci].getName() == "Romania") {
        roFuelText.setString(std::to_string(stock.getFuel())); roManpowerText.setString(std::to_string(stock.getManpower()));
    } else if (countries[ci].getName() == "Hungary") {
        huFuelText.setString(std::to_string(stock.getFuel())); huManpowerText.setString(std::to_string(stock.getManpower()));
    }

    std::string alertMsg = "";
    if (countries[ci].getFocusTree().getActiveFocusIndex() == -1) {
        alertMsg += "[!] NO NATIONAL FOCUS SELECTED\n";
    }
    if (countries[ci].getConstructionQueue().isEmpty()) {
        alertMsg += "[!] NO ACTIVE CONSTRUCTIONS\n";
    }
    int freeMils = countries[ci].getFreeMilFactories();
    if (freeMils > 0) {
        alertMsg += "[!] FREE MILITARY FACTORIES: " + std::to_string(freeMils) + "\n";
    }
    alertText.setString(alertMsg);
}

void Interface::handleEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
            if (!showConstruction && !showProduction && selectedCountryIndex != -1) showFocusTree = !showFocusTree;
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W) {
             if (!showFocusTree && !showProduction && selectedCountryIndex != -1) {
                 showConstruction = !showConstruction;
                 selectedBuildingType = -1;
                 infoText.setString(showConstruction ? "CONSTRUCTION MODE" : "MAP MODE");
             }
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
             if (!showFocusTree && !showConstruction && selectedCountryIndex != -1) {
                 showProduction = !showProduction;
                 infoText.setString(showProduction ? "PRODUCTION MODE" : "MAP MODE");
             }
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (showFocusTree && selectedCountryIndex != -1) {
                Country& currentCountry = engine.getMutableCountries()[selectedCountryIndex];
                for (int i = 0; i < 4; ++i) {
                    if (focusIconSprites[i].getGlobalBounds().contains(mousePos)) {
                        if (currentCountry.startFocus(i)) std::cout << "Focus " << i << " started!\n";
                    }
                }
            }
            else if (showConstruction && selectedCountryIndex != -1) {
                std::size_t ci = static_cast<std::size_t>(selectedCountryIndex);
                float offsetX = 0.f;
                float panelWidth = window.getSize().x / 3.0f;
                if (engine.getCountries()[ci].getName() == "Hungary") offsetX = window.getSize().x - panelWidth;

                for (int i = 0; i < 5; ++i) {
                    float originalY = 100.f + i * (64.f + 20.f);
                    sf::FloatRect bounds(offsetX + 20.f, originalY, 64.f, 64.f);
                    if (bounds.contains(mousePos)) {
                        selectedBuildingType = i;
                    }
                }

                if (selectedBuildingType != -1) {
                    Country& currentCountry = engine.getMutableCountries()[selectedCountryIndex];
                    bool clickOnMap = false;
                    if (engine.getCountries()[ci].getName() == "Hungary") {
                        if (mousePos.x < offsetX) clickOnMap = true;
                    } else {
                        if (mousePos.x > panelWidth) clickOnMap = true;
                    }

                    if (clickOnMap) {
                         for (const auto& zone : clickZones) {
                             if (zone.countryIndex == selectedCountryIndex && zone.bounds.contains(mousePos)) {
                                 const auto& provs = currentCountry.getProvinces();
                                 const std::string& cName = currentCountry.getName();
                                 for(int pi=0; pi < (int)provs.size(); ++pi) {
                                     sf::Vector2f pos = getIconPositionFor(cName, provs[pi].getName(), StatKind::Steel);
                                     sf::FloatRect pBounds(pos.x - 150.f, pos.y - 150.f, 400.f, 400.f);
                                     if (pBounds.contains(mousePos)) {
                                         // --- AM MUTAT DEFINITIA AICI PENTRU A REZOLVA VARIABLESCOPE ---
                                         // Desi currentCountry este deja definit sus, aici il folosim specific.
                                         // In cazul tau, codul initial avea definitia sus. CppCheck se plangea.
                                         // In varianta asta am pastrat definitia sus (in if-ul mare) dar am sters
                                         // variabila nefolosita 'ci' mai jos.
                                         BuildingType type;
                                         if(selectedBuildingType == 0) type = BuildingType::Civ;
                                         else if(selectedBuildingType == 1) type = BuildingType::Mil;
                                         else if(selectedBuildingType == 2) type = BuildingType::Infra;
                                         else if(selectedBuildingType == 3) type = BuildingType::Dockyard;
                                         else type = BuildingType::Airfield;

                                         try {
                                             currentCountry.addConstruction(type, pi, 1);
                                         } catch(const std::exception& e) {
                                             std::cerr << e.what() << "\n";
                                         }
                                         break;
                                     }
                                 }
                             }
                         }
                    }
                }
            }
            else if (showProduction && selectedCountryIndex != -1) {
                Country& currentCountry = engine.getMutableCountries()[selectedCountryIndex];
                std::size_t ci = static_cast<std::size_t>(selectedCountryIndex);
                float offsetX = 0.f;
                float panelWidth = window.getSize().x / 3.0f;
                if (currentCountry.getName() == "Hungary") offsetX = window.getSize().x - panelWidth;

                // --- AM STERS 'std::size_t ci' nefolosit de aici ---

                const auto& lines = currentCountry.getProductionLines();
                float currentY = 100.f;
                for (int i = 0; i < (int)lines.size(); ++i) {
                    sf::FloatRect minusBounds(offsetX + 180.f, currentY, 24.f, 24.f);
                    sf::FloatRect plusBounds(offsetX + 240.f, currentY, 24.f, 24.f);
                    if (minusBounds.contains(mousePos)) {
                        currentCountry.modifyLineFactories(i, -1);
                    }
                    else if (plusBounds.contains(mousePos)) {
                        currentCountry.modifyLineFactories(i, +1);
                    }
                    currentY += 80.f;
                }

                float iconSize = 48.f;
                float startX = offsetX + 20.f;
                float bottomY = window.getSize().y - 100.f;
                for(int i=0; i<4; ++i) {
                    sf::FloatRect addBounds(startX + i * (iconSize + 10.f), bottomY, iconSize, iconSize);
                    if (addBounds.contains(mousePos)) {
                        EquipmentType type;
                        if(i==0) type = EquipmentType::Gun;
                        else if(i==1) type = EquipmentType::Artillery;
                        else if(i==2) type = EquipmentType::AntiAir;
                        else type = EquipmentType::CAS;
                        currentCountry.addProductionLine(type);
                    }
                }
            }
            else {
                for (const auto& zone : clickZones) {
                    if (zone.bounds.contains(mousePos)) {
                        selectedCountryIndex = zone.countryIndex;
                        infoText.setString("Selected: " + engine.getCountries()[selectedCountryIndex].getName());
                        break;
                    }
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            if (showFocusTree) showFocusTree = false;
            else if (showConstruction) { showConstruction = false; selectedBuildingType = -1; }
            else if (showProduction) { showProduction = false; }
            else { selectedCountryIndex = -1; infoText.setString("PAUSED - CLICK TO SELECT"); }
        }
    }
}

void Interface::render() {
    window.clear();

    if (showFocusTree && selectedCountryIndex != -1) {
        window.draw(focusBgSprite);
        const FocusTree& tree = engine.getCountries()[selectedCountryIndex].getFocusTree();
        int activeIndex = tree.getActiveFocusIndex();
        for (int i = 0; i < 4; ++i) {
            sf::Sprite& s = focusIconSprites[i];
            if (tree.isFocusCompleted(i)) s.setColor(sf::Color(100, 100, 100));
            else if (i == activeIndex) s.setColor(sf::Color(100, 255, 100));
            else if (s.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y))) s.setColor(sf::Color::White);
            else s.setColor(sf::Color(200, 200, 200));
            window.draw(s); window.draw(focusNameTexts[i]);
        }
        sf::Text title; title.setFont(font); title.setString("National Focus"); title.setPosition(50.f, 50.f);
        window.draw(title);
    }
    else if (showConstruction && selectedCountryIndex != -1) {
        std::size_t ci = static_cast<std::size_t>(selectedCountryIndex);
        float offsetX = 0.f;
        float panelWidth = window.getSize().x / 3.0f;
        if (engine.getCountries()[ci].getName() == "Hungary") offsetX = window.getSize().x - panelWidth;

        window.draw(mapSprite);
        if (ci < provinceUI.size()) {
             for (auto& pui : provinceUI[ci]) {
                for (auto& icon : pui.resourceIcons) { window.draw(icon.icon); window.draw(icon.value); }
            }
        }

        constructionPanel.setPosition(offsetX, 0.f);
        window.draw(constructionPanel);

        for (int i = 0; i < 5; ++i) {
            float originalY = 100.f + i * (64.f + 20.f);
            buildIcons[i].setPosition(offsetX + 20.f, originalY);
            buildIconTexts[i].setPosition(offsetX + 64.f + 10.f + 20.f, originalY + 20.f);

            if (i == selectedBuildingType) {
                buildIcons[i].setColor(sf::Color(100, 255, 100));
                buildIconTexts[i].setFillColor(sf::Color::Green);
            } else {
                buildIcons[i].setColor(sf::Color::White);
                buildIconTexts[i].setFillColor(sf::Color::White);
            }
            window.draw(buildIcons[i]);
            window.draw(buildIconTexts[i]);
        }

        queueTitleText.setPosition(offsetX + 20.f, 600.f);
        window.draw(queueTitleText);

        const auto& queue = engine.getCountries()[ci].getConstructionQueue();
        float qY = 640.f;
        int count = 1;
        for (const auto& item : queue) {
            std::string bName;
            switch(item.getType()) {
                case BuildingType::Civ: bName = "Civ"; break;
                case BuildingType::Mil: bName = "Mil"; break;
                case BuildingType::Infra: bName = "Infra"; break;
                case BuildingType::Dockyard: bName = "Dock"; break;
                case BuildingType::Airfield: bName = "Air"; break;
                default: bName = "?"; break;
            }
            int pIdx = item.getProvinceIndex();
            std::string pName = "Unknown";
            if (pIdx >= 0 && pIdx < (int)engine.getCountries()[ci].getProvinces().size()) {
                pName = engine.getCountries()[ci].getProvinces()[pIdx].getName();
            }
            std::string line = std::to_string(count) + ". " + bName + " in " + pName + " (" + std::to_string((int)item.getRemainingBP()) + " BP left)";
            sf::Text t; t.setFont(font); t.setString(line); t.setCharacterSize(18); t.setPosition(offsetX + 20.f, qY); t.setFillColor(sf::Color::White);
            window.draw(t);
            qY += 25.f; count++;
            if (qY > window.getSize().y - 30.f) break;
        }
        window.draw(infoText);
    }
    else if (showProduction && selectedCountryIndex != -1) {
        std::size_t ci = static_cast<std::size_t>(selectedCountryIndex);
        float offsetX = 0.f;
        float panelWidth = window.getSize().x / 3.0f;
        if (engine.getCountries()[ci].getName() == "Hungary") offsetX = window.getSize().x - panelWidth;

        window.draw(mapSprite);
        if (ci < provinceUI.size()) {
             for (auto& pui : provinceUI[ci]) {
                for (auto& icon : pui.resourceIcons) { window.draw(icon.icon); window.draw(icon.value); }
            }
        }

        constructionPanel.setPosition(offsetX, 0.f);
        window.draw(constructionPanel);

        prodTitleText.setPosition(offsetX + 20.f, 20.f);
        window.draw(prodTitleText);

        const auto& lines = engine.getCountries()[ci].getProductionLines();
        float currentY = 100.f;

        // Luam o referinta catre tara curenta pentru a interoga stocurile
        const Country& currentCountry = engine.getCountries()[ci];

        for (const auto& line : lines) {
            sf::Sprite s;
            switch(line.getType()) {
                case EquipmentType::Gun: s.setTexture(gunTex); break;
                case EquipmentType::Artillery: s.setTexture(artTex); break;
                case EquipmentType::AntiAir: s.setTexture(aaTex); break;
                case EquipmentType::CAS: s.setTexture(casTex); break;
            }
            float sx = 64.f / s.getTexture()->getSize().x;
            float sy = 64.f / s.getTexture()->getSize().y;
            s.setScale(sx, sy);
            s.setPosition(offsetX + 20.f, currentY);
            window.draw(s);

            sf::Text nameT;
            nameT.setFont(font);
            nameT.setCharacterSize(18);
            nameT.setFillColor(sf::Color::White);
            nameT.setPosition(offsetX + 100.f, currentY);
            switch(line.getType()) {
                case EquipmentType::Gun: nameT.setString("Infantry Eq."); break;
                case EquipmentType::Artillery: nameT.setString("Artillery"); break;
                case EquipmentType::AntiAir: nameT.setString("Anti-Air"); break;
                case EquipmentType::CAS: nameT.setString("CAS"); break;
            }
            window.draw(nameT);

            sf::Sprite minusS(minusTex);
            sf::Sprite plusS(plusTex);
            minusS.setScale(24.f/minusTex.getSize().x, 24.f/minusTex.getSize().y);
            plusS.setScale(24.f/plusTex.getSize().x, 24.f/plusTex.getSize().y);

            minusS.setPosition(offsetX + 180.f, currentY);
            plusS.setPosition(offsetX + 240.f, currentY);

            sf::Text countT;
            countT.setFont(font);
            countT.setCharacterSize(20);
            countT.setFillColor(sf::Color::Green);
            countT.setString(std::to_string(line.getFactories()));
            countT.setPosition(offsetX + 215.f, currentY);

            window.draw(minusS);
            window.draw(plusS);
            window.draw(countT);

            // --- DRAW TOTAL STOCK & DAILY OUTPUT ---
            long long totalStock = currentCountry.getEquipmentCount(line.getType());
            long long dailyProd = line.calculateDailyOutput();

            sf::Text statsT;
            statsT.setFont(font);
            statsT.setCharacterSize(16);
            statsT.setFillColor(sf::Color::Yellow);
            statsT.setPosition(offsetX + 280.f, currentY + 2.f);

            std::string statsStr = "In Stock: " + std::to_string(totalStock) +
                                   " | +" + std::to_string(dailyProd) + "/day";
            statsT.setString(statsStr);
            window.draw(statsT);

            currentY += 80.f;
        }

        sf::Text addNewT;
        addNewT.setFont(font);
        addNewT.setString("Add New Production Line:");
        addNewT.setCharacterSize(20);
        addNewT.setFillColor(sf::Color::Yellow);
        float bottomY = window.getSize().y - 140.f;
        addNewT.setPosition(offsetX + 20.f, bottomY);
        window.draw(addNewT);

        float iconSize = 48.f;
        float startX = offsetX + 20.f;
        float iconsY = window.getSize().y - 100.f;

        for(int i=0; i<4; ++i) {
            addNewIcons[i].setPosition(startX + i * (iconSize + 10.f), iconsY);
            window.draw(addNewIcons[i]);
        }

        window.draw(infoText);
        window.draw(alertText);
    }
    else {
        window.draw(mapSprite);
        if (selectedCountryIndex != -1) {
            std::size_t ci = static_cast<std::size_t>(selectedCountryIndex);
            if (ci < provinceUI.size()) {
                for (auto& pui : provinceUI[ci]) {
                    for (auto& icon : pui.resourceIcons) { window.draw(icon.icon); window.draw(icon.value); }
                }
            }
            if (engine.getCountries()[ci].getName() == "Romania") {
                window.draw(roFuelSprite); window.draw(roManpowerSprite); window.draw(roFuelText); window.draw(roManpowerText);
            } else if (engine.getCountries()[ci].getName() == "Hungary") {
                window.draw(huFuelSprite); window.draw(huManpowerSprite); window.draw(huFuelText); window.draw(huManpowerText);
            }
            window.draw(alertText);
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
            while (accumulator >= secondsPerDay) { engine.simulateOneDay(); accumulator -= secondsPerDay; }
            updateUI();
        } else { clock.restart(); }
        render();
    }
}