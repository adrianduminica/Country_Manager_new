#include "../headers/Engine.h"
#include "../headers/Interface.h"
#include "../headers/GameExceptions.h"
#include <iostream>

int main() {
    try {
        // --- MODIFICARE: Instantiere normala (Nu mai e Singleton) ---
        Engine engine;

        // Trimitem engine-ul prin referinta catre interfata
        Interface ui(engine);
        ui.run();
    } catch (const AssetLoadException &e) {
        std::cerr << "Eroare la incarcare resurse: " << e.what() << "\n";
        return 1;
    } catch (const InvalidProvinceIndexException &e) {
        std::cerr << "Eroare index provincie: " << e.what() << "\n";
        std::cerr << "Index invalid: " << e.getIndex() << "\n";
        return 1;
    } catch (const InvalidConstructionCountException &e) {
        std::cerr << "Eroare numar constructii: " << e.what() << "\n";
        std::cerr << "Count invalid: " << e.getCount() << "\n";
        return 1;
    } catch (const GameException &e) {
        std::cerr << "Eroare de joc: " << e.what() << "\n";
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Eroare neasteptata: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
