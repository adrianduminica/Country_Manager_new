#include "Engine.h"
#include "Interface.h"

int main() {
    Engine engine;
    engine.run();

    Interface ui("Country Manager UI");
    ui.run();

    return 0;
}
