#include "Engine.h"
#include "Interface.h"

int main() {
    Engine engine;
    engine.init();

    Interface ui(engine, "Country Manager UI");
    ui.run();

    return 0;
}
