#include "../headers/Engine.h"
#include "../headers/Interface.h"
#include "../headers/GameExceptions.h"
#include <iostream>

int main() {
    try {
        Engine engine;
        Interface ui(engine);
        ui.run();
    } catch (const AssetLoadException& e) {
        std::cerr << "Asset Error: " << e.what() << "\n";
        return 1;
    } catch (const GameException& e) {
        std::cerr << "Game Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}