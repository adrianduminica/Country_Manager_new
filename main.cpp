#include "Engine.h"
#include "Interface.h"
#include "GameExceptions.h"
#include <iostream>

int main() {
    try {
        Engine engine;
        Interface ui(engine);
        ui.run();
    } catch (const AssetLoadException& e) {
        std::cerr << "Eroare la incarcare resurse: " << e.what() << "\n";
        return 1;
    } catch (const GameException& e) {
        std::cerr << "Eroare de joc: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Eroare neasteptata: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
