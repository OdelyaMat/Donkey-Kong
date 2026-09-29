#include <iostream>
#include "Game.h"
#include "utilits.h"
#include <fstream>

int main(int argc, char* argv[]) {
    bool isSaveMode = false;
    bool isLoadMode = false;
    bool isSilentMode = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-save") {
            isSaveMode = true;
        }
        else if (arg == "-load") {
            isLoadMode = true;
        }
        else if (arg == "-silent" && isLoadMode) {
            isSilentMode = true;
        }
    }

    // Hide console cursor for a cleaner game experience
    ShowConsoleCursor(false);

    // Create Game object
    Game game(isSaveMode, isLoadMode, isSilentMode);

    // Start game
    game.run();

    return 0;
}
