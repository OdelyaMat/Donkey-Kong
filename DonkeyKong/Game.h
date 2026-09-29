#pragma once
#include <conio.h>
#include <vector>
#include <sstream>  
#include <iomanip>  
#include <filesystem>
#include <regex>
#include "board.h"
#include "mario.h"
#include "barrels.h"
#include "Ghosts.h"
#include "Steps.h"
#include "Results.h"


class Game {
private:
    static constexpr int LIVE_PRINT_POS_Y = 1;
    static constexpr int LIVE_PRINT_POS_X = 2;
    static constexpr int HAMMER_PRINT_POS_X = 2;
    static constexpr int HAMMER_PRINT_POS_Y = 2;
    static constexpr int SCORE_PRINT_POS_X = 68;
    static constexpr int SCORE_PRINT_POS_Y = 1;
    static constexpr int NEW_SCORE_PRINT_POS_X = 75;
    static constexpr int NEW_SCORE_PRINT_POS_Y = 1;



    static constexpr int INIT_NUM_LIVES = 3;
    static constexpr int FRAME_DELAY_MS = 100;                           // Time delay for each game loop iteration
    static constexpr int SCREEN_WIDTH = 80;
    static constexpr int menuX = 17;
    static constexpr int MENU_POS_Y = 5;
    static constexpr int MAX_X = 80;
    static constexpr int MAX_Y = 25;
    static constexpr char ESC_KEY = 27;
    static constexpr char EXIT = '9';
    static constexpr char START_NEW_GAME = '1';
    static constexpr char SCREEN1 = '1';
    static constexpr char SCREEN2 = '2';
    static constexpr char SCREEN3 = '3';
    static constexpr char INSTRUCTIONS = '8';
    static constexpr char DONKEYKONG = '&';
    static constexpr char PAULINE = '$';
    static constexpr char FLOOR_EQUA = '=';
    static constexpr char LADDER = 'H';

    std::string livesVisual = "@@@";
    std::string weapons = "WEAPONS: HAMMER";
    std::string score = "SCORE: ";

    long gameTime = 0;

    int _lives = INIT_NUM_LIVES;
    int nextBarrelSpawnTime;                                                   // Time to spawn the next barrel
    int menuY = 5;
    int _hammerX = -1;
    int _hammerY = -1;
    int _score = 0;
    int _addedScore = 0;                            // Score that was added in the current level
    int barrelDirChange = -1;
    int currentLevel = 1;  // Start at Level 1
    bool paused = false;                        // Track if the game is paused
    bool running = false;                         // Track if the game is running
    bool isSaveMode = false;
    bool isLoadMode = false;
    bool isSilentMode = false;
    bool isLoadFinished = false;
    Steps recordedSteps;
    Results expectedResults;
    Board board;                                                               // The game board
    Mario mario;                                                               // The Mario character
    std::vector<Barrel> barrels;                                               // Vector to store barrels
    std::vector<Ghost>* ghosts = nullptr;                                      // Vector to store ghosts
    std::vector<Xghost>* Xghosts = nullptr;                                      // Vector to store ghosts

    void processInput();                                                       // Handles user input
    bool updateObjects();                                                      // Updates positions of Mario, barrels, etc. returns true if mario died
    void renderObjects() const;                                                // Renders the game objects on the console
    void reRenderObjects() const;
    void spawnBarrel();                                                        // Spawns a new barrel at the appropriate time
    void cleanupObjects();                                                     // Removes objects marked for removal (like off-screen barrels)
    void restartLevel();
    void resetGame();
    void executeStep(char step);
    void saveSteps();
    void saveResults(bool val);
    void validateResults();
    std::string formatScreenNumber(int screenIndex) const;
    std::string findFirstLevelFile(const std::string& fileType) const;
    int extractLevelFromFilename(const std::string& filename) const;
    void deleteAllDkongFiles();

public:

    Game(bool saveMode, bool loadMode, bool silentMode);                                                                    // Constructor to initialize the game
    void run();                                                                // Main game loop
    void showMenu();
    bool handleCollisions();                                                   // Checks and resolves collisions between objects
    bool handleBarrelCollisions();
    bool handleGhostCollisions();
    bool handleXghostCollisions();
    bool getIsLoad() {
        return isLoadMode;
    }
    bool getIsSilent() {
        return isSilentMode;
    }
    void loadSteps(); // Loads steps from a file for replay mode
    void loadResults();

    int getCenterPadding(const std::string& text, int totalWidth) const {
        return (totalWidth - text.length()) / 2;
    }

    void setHammerX(int x) {
        _hammerX = x;
    }

    void setHammerY(int y) {
        _hammerY = y;
    }

    void setBarrelDirChange(int change) {
        barrelDirChange *= -1;
    }

    void displayWinner();

};
