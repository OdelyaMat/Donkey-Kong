#include "Game.h"

using namespace std;
Game::Game(bool saveMode, bool loadMode, bool silentMode)
    : isSaveMode(saveMode), isLoadMode(loadMode), isSilentMode(silentMode), gameTime(0), nextBarrelSpawnTime(0){ // Initialize game state
    // Initialize the board and Mario
    if (board.reset())
    {
        this->ghosts = &board.getGhosts();
        for (auto& ghost : *ghosts) {
            ghost.isLoadMode = isLoadMode;
            ghost.isSilentMode = isSilentMode;
        }
        this->Xghosts = &board.getXghosts();
        for (auto& xghost : *Xghosts) {
            xghost.isLoadMode = isLoadMode;
            xghost.isSilentMode = isSilentMode;
        }
        mario.init(board.getMarioX(), board.getMarioY(), &board.getGhosts(), &board.getXghosts(), isSilentMode, isLoadMode);
        mario.setBoard(board);
        this->setHammerX(board.getHammerX());
        this->setHammerY(board.getHammerY());
        this->_score = 0;

    }
    else {
        cout << "No valid screens were found in your directory.";
        running = false;
    }
    if (isLoadMode) {
        std::string firstStepsFile = findFirstLevelFile("steps");
        if (!firstStepsFile.empty()) {
            currentLevel = extractLevelFromFilename(firstStepsFile);
            loadSteps();
            loadResults();
        }
    }
    if (isSaveMode) {
        deleteAllDkongFiles();  // Clear previous files if in save mode
    }

}

int Game::extractLevelFromFilename(const std::string& filename) const {
    std::regex levelRegex("dkong_(\\d{2})\\.");
    std::smatch match;

    if (std::regex_search(filename, match, levelRegex)) {
        return std::stoi(match[1].str());
    }

    return 1;  // Default to Level 1 if no match found
}

void Game::deleteAllDkongFiles() {
    if (!isSaveMode) return;  // Only delete files in save mode


    for (const auto& file : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        std::string filename = file.path().filename().string();

        // Check if the file ends with ".steps" or ".result"
        if (filename.find("dkong_") == 0 &&
            (filename.size() >= 6 &&
                (filename.substr(filename.size() - 6) == ".steps" ||
                    filename.substr(filename.size() - 7) == ".result"))) {
            try {
                std::filesystem::remove(file);
                std::cout << "Deleted: " << filename << std::endl;
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Failed to delete " << filename << ": " << e.what() << std::endl;
            }
        }
    }
}


void Game::loadResults() {
    std::string resultsFile = findFirstLevelFile("result");
     expectedResults = Results::loadResults(resultsFile);

}


std::string Game::findFirstLevelFile(const std::string& fileType) const {
    std::string pattern = "dkong_(\\d{2})\\." + fileType;
    std::regex fileRegex(pattern);
    std::string earliestFile;
    int minLevel = INT_MAX;

    // Iterate over files in the current directory
    for (const auto& file : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        std::string filename = file.path().filename().string();
        std::smatch match;

        // Check if the file matches the dkong_XX.steps or dkong_XX.result pattern
        if (std::regex_match(filename, match, fileRegex)) {
            int level = std::stoi(match[1].str());
            if (level < minLevel) {
                minLevel = level;
                earliestFile = filename;
            }
        }
    }

    if (earliestFile.empty()) {
        std::cerr << "❌ No " << fileType << " files found in the directory!" << std::endl;
    }

    return earliestFile;
}

void Game::loadSteps() {
    std::string stepsFile = findFirstLevelFile("steps");
    recordedSteps = Steps::loadSteps(stepsFile);
}



void Game::showMenu() {
    system("cls");
    char choice;

    // Define the main menu
    string menu[] = {
        "##########################################",
        "#                                        #",
        "#                ########                #",
        "#              ##        ##              #",
        "#             ##   MENU   ##             #",
        "#              ##        ##              #",
        "#                ########                #",
        "#                                        #",
        "#     (1) START A NEW GAME               #",
        "#     (8) PRESENT INSTRUCTIONS AND KEYS  #",
        "#     (9) EXIT                           #",
        "#                                        #",
        "##########################################"
    };

    // Define the instructions menu
    string instruct_keys[] = {
        "##########################################",
        "#       KEYS            INSTRUCTIONS     #",
        "#                                        #",
        "# RIGHT   : d or D      You are mario.   #",
        "# UP/JUMP : w or W      The goal is to   #",
        "# DOWN    : x or X      save Pauline.    #",
        "# STAY    : s or S      Avoid barrels,   #",
        "# LEFT    : a or A      you only have 3  #",
        "#                       lives. Go get    #",
        "#                       them tiger.      #",
        "#                                        #",
        "#         PRESS ANY KEY TO MENU          #",
        "##########################################"
    };

    string byebye[] = {
        "###########################################",
        "#                                         #",
        "#             ******   ******             #",
        "#           ********* *********           #",
        "#          *********************          #",
        "#         ***********************         #",
        "#         *******THANK YOU*******         #",
        "#          *****FOR PLAYING*****          #",
        "#           ******BYE BYE******           #",
        "#            *****************            #",
        "#             ***************             #",
        "#               ***********               #",
        "#                 *******                 #",
        "#                   ***                   #",
        "#                    *                    #",
        "###########################################"
    };
    string chooseScreen[] = { 
        "###########################################",
        "#                                         #",
        "#                                         #",
        "#                                         #",
        "#                                         #",
        "#                                         #",
        "#                                         #",
        "#              (1) LEVEL #1               #",
        "#              (2) LEVEL #2               #",
        "#              (3) LEVEL #3               #",
        "#                                         #",
        "#                                         #",
        "#                                         #",
        "#            PRESS ESC TO MENU            #",
        "#                                         #",
        "###########################################"
    };
    while (true) {
        if (running)
            break;
        // Display the menu
        menuY = MENU_POS_Y;
        for (const auto& line : menu) {
            gotoxy(menuX, menuY);
            cout << line;
            menuY++;
        }

        // Get the user's choice
        choice = _getch();

        // Process the user's choice
        switch (choice) {
        case START_NEW_GAME: {
            system("cls");
            menuY = MENU_POS_Y;
            for (const auto& line : chooseScreen) {
                gotoxy(menuX, menuY);
                cout << line;
                menuY++;
            }
            choice = _getch();
            while (choice != ESC_KEY)
            {
                while (choice != '1' && choice != '2' && choice != '3' && choice != ESC_KEY)
                    choice = _getch();

                if (choice == ESC_KEY) {
                    system("cls");
                    menuY = MENU_POS_Y;
                    for (const auto& line : menu) {
                        gotoxy(menuX, menuY);
                        cout << line;
                        menuY++;
                    }
                    choice = _getch();
                    break;
                }
                else {
                    this->board.setScreenIndex(choice - '0');
                    // Start a new game
                    running = true;
                    break;
                }

            }
            if (choice == ESC_KEY) {
                system("cls");
                break;
            }
            if (running)
                break;
        }
        case INSTRUCTIONS: {
            // Display instructions
            system("cls");
            menuY = MENU_POS_Y;
            for (const auto& line : instruct_keys) {
                gotoxy(menuX, menuY);
                cout << line;
                menuY++;
            }
            _getch();
            break;
        }
        case EXIT: {
            // Exit the game
            system("cls");
            menuY = MENU_POS_Y;
            for (const auto& line : byebye) {
                gotoxy(menuX, menuY);
                cout << line;
                menuY++;
            }
            running = false;
            return;
        }
        default: {
            break; // Simply break out of the switch
        }
        }
    }
}

void Game::processInput() {
    if (_kbhit()) { // Check if a key was pressed
        char key = _getch(); // Get the pressed key

        if (isSaveMode) {
            recordedSteps.addStep(gameTime, key);  //Save every keypress in .steps
        }

        if (key == ESC_KEY) {
            // Toggle the paused state
            paused = !paused;

            if (paused) {
                system("cls");
                std::cout << "\n\n\n\n\n\n\n\n\n\n";
                std::cout << "================================================================================\n";
                std::cout << "                    Game paused. Press ESC again to continue.                   \n";
                std::cout << "================================================================================\n";
                while (_getch() != ESC_KEY); // Wait for ESC key to resume
                system("cls");
                paused = !paused;
                reRenderObjects(); // Re-render initial state after resuming
            }
            else {

                gotoxy(35, 12);
                std::cout << "                                    "; // Clear the message
            }
        }
        if (!paused) {
            // If not paused, let mario handle keys
            mario.resetUseHammer();
            mario.keyPressed(key);
        }
    }
}

//if mario died in the current turn - returns true. false otherwise
bool Game::updateObjects() {
    // Update Mario's state and position
    mario.move();
//    if (!(isSilentMode && isLoadMode))
        mario.draw();
    if (mario.getHammer() &&!(isSilentMode && isLoadMode)) {
        gotoxy(HAMMER_PRINT_POS_X, HAMMER_PRINT_POS_Y);
        cout << weapons;
    }

    for (auto& ghost : *ghosts) {
        ghost.move();
        if (!(isSilentMode && isLoadMode))
            ghost.draw();
    }
    for (auto& xghost : *Xghosts) {
        xghost.move();
        if (!(isSilentMode && isLoadMode))
            xghost.draw();
    }
    if (mario.getFallTooMuch() == true)
    {
        if (mario.getIsOnFloor() == true) {
            _lives--;
            livesVisual.pop_back();
            return true;
        }
    }

    // Update all barrels moves - check collision with Mario
    bool res = handleCollisions();
    if (res == true)
    {
        livesVisual.pop_back();
        return true;
    }
    // Remove barrels that are no longer needed
    // Barrel removal is from chatGpt4o
    barrels.erase(
        std::remove_if(barrels.begin(), barrels.end(),
            [](const Barrel& barrel) { return barrel.getRemoved(); }),
        barrels.end());

    return false;
}

void Game::renderObjects() const {

    gotoxy(NEW_SCORE_PRINT_POS_X, NEW_SCORE_PRINT_POS_Y);
    cout << _score;
    // Render all barrels
    for (const auto& barrel : barrels) {
        barrel.draw();
    }
}

void Game::reRenderObjects() const {
    board.print();
    // Render Mario in his current position
    mario.draw();

    // Render all barrels
    for (const auto& barrel : barrels) {
        barrel.draw();
    }
}

// The spawnBarrel func is originally from ChatGpt 4o
void Game::spawnBarrel() {
    // Create a new barrel
    Barrel newBarrel;

    // Link the new barrel to the board
    newBarrel.setBoard(board);

    // Set the barrel's starting position- each spawn will be at a different side of Donkeykong
    newBarrel.setX(board.getDonkeyX() - 1 * barrelDirChange);
    newBarrel.setY(board.getDonkeyY());
    setBarrelDirChange(-1); // Responsible for barrel's direction change each iteration 

    newBarrel.isSilentMode = isSilentMode;
    newBarrel.isLoadMode = isLoadMode;
    // Draw the new barrel at its starting position
    if (!(isSilentMode && isLoadMode))
        newBarrel.draw();

    // Add the new barrel to the barrels vector
    barrels.push_back(newBarrel);

    // Schedule the next barrel spawn time
    nextBarrelSpawnTime = GetTickCount() + 4000; // Spawn every 4 seconds
}

bool Game::handleBarrelCollisions() {
    for (auto it = barrels.begin(); it != barrels.end(); ) {
        it->move(); // Update each barrel's position and state
        if (!(isSilentMode && isLoadMode))
            it->draw();
        int marioNextX = mario.getX() + mario.getDirX();
        int marioNextY = mario.getY() + mario.getDirY();

        // Predict Barrel's next position
        int barrelNextX = it->getX() + it->getDirX();
        int barrelNextY = it->getY(); // Assuming ghosts don't move vertically

        // Check if Mario is hit by a barrel
        if ((it->getX() == mario.getX() && it->getY() == mario.getY()) ||
            (mario.getX() == barrelNextX && mario.getY() == barrelNextY)) {
            _lives--; // Reduce Mario's life
            return true; // Exit this frame to restart immediately
        }

        // Check if Mario can remove the barrel with a hammer
        if (abs(it->getX() - mario.getX()) > 0 && abs(it->getX() - mario.getX()) <= 2 &&
            it->getY() == mario.getY()) {
            if (mario.getHammer() && mario.getUseHammer()) {
                it->erase();
                it = barrels.erase(it); // Remove the barrel and update the iterator
                mario.resetUseHammer(); // Reset hammer usage
                this->_addedScore += 5;
                this->_score += 5; // 5 points for killing a barrel;
                continue; // Skip incrementing the iterator
            }
        }

        // Check if a removed barrel is near Mario
        if (it->getRemoved() &&
            abs(mario.getX() - it->getX()) <= 2 &&
            abs(mario.getY() - it->getY()) <= 2) {
            _lives--; // Reduce Mario's life
            return true; // Exit the loop and frame
        }

        // Handle barrels marked as removed
        if (it->getRemoved()) {
            it->exploded(); // Trigger the explosion logic
            it = barrels.erase(it); // Remove the barrel and update the iterator
            continue; // Skip incrementing the iterator
        }

        ++it; // Increment the iterator if no barrel was erased
    }
    return false;
}

bool Game::handleGhostCollisions() {
    // Update ghost movements
    for (auto it = ghosts->begin(); it != ghosts->end(); ) {
        for (auto other = ghosts->begin(); other != ghosts->end(); ++other) { // Handle collision between ghosts
            if (it == other)
                continue;
            if (it->getY() == other->getY() && it->getX() + it->getDirX() == other->getX()) {
                it->changeDirX();
                other->changeDirX();
            }
        }

        it->move(); // Move each ghost
        if (!(isSilentMode && isLoadMode))
            it->draw();
        // Check if Mario can remove the ghost with a hammer
        if (abs(it->getX() - mario.getX()) > 0 && abs(it->getX() - mario.getX()) <= 2 &&
            it->getY() == mario.getY()) {
            if (mario.getHammer() && mario.getUseHammer()) {
                it = ghosts->erase(it); // Remove the ghost and update the iterator
                mario.resetUseHammer();
                this->_addedScore += 10;
                this->_score += 10; // 10 points for killing a ghost;
                continue; // Skip incrementing to avoid invalid iterator
            }
        }
        int marioNextX = mario.getX() + mario.getDirX();
        int marioNextY = mario.getY() + mario.getDirY();

        // Predict Ghost's next position
        int ghostNextX = it->getX() + it->getDirX();
        int ghostNextY = it->getY(); // Assuming ghosts don't move vertically

        // Check if Mario meets a ghost
        if ((it->getX() == mario.getX() && it->getY() == mario.getY()) ||
            (mario.getX() == ghostNextX && mario.getY() == ghostNextY)) {
            _lives--; // Reduce Mario's life
            return true; // Exit this frame to restart immediately
        }



        ++it; // Increment iterator if no ghost was erased
    }

    return false;
}

bool Game::handleXghostCollisions() {
    // Update ghost movements
    for (auto it = Xghosts->begin(); it != Xghosts->end(); ) {
        for (auto other = Xghosts->begin(); other != Xghosts->end(); ++other) { // Handle collision between ghosts
            if (it == other)
                continue;
            if (it->getY() == other->getY() && it->getX() + it->getDirX() == other->getX()) {
                it->changeDirX();
                other->changeDirX();
            }
        }
        it->move(); // Move each ghost
        if (!(isSilentMode && isLoadMode))
            it->draw();
        // Check if Mario can remove the ghost with a hammer
        if (abs(it->getX() - mario.getX()) > 0 && abs(it->getX() - mario.getX()) <= 2 &&
            it->getY() == mario.getY()) {
            if (mario.getHammer() && mario.getUseHammer()) {
                it = Xghosts->erase(it); // Remove the ghost and update the iterator
                mario.resetUseHammer();
                this->_addedScore += 15;
                this->_score += 15; // 15 points for killing an Xghost;
                continue; // Skip incrementing to avoid invalid iterator
            }
        }
        int marioNextX = mario.getX() + mario.getDirX();
        int marioNextY = mario.getY() + mario.getDirY();

        // Predict Ghost's next position
        int XghostNextX = it->getX() + it->getDirX();
        int XghostNextY = it->getY() + it->getDirY(); // Assuming ghosts don't move vertically

        // Check if Mario meets a ghost
        if ((it->getX() == mario.getX() && it->getY() == mario.getY()) ||
            (mario.getX() == XghostNextX && mario.getY() == XghostNextY)) {
            _lives--; // Reduce Mario's life
            return true; // Exit this frame to restart immediately
        }



        ++it; // Increment iterator if no ghost was erased
    }

    return false;
}

void Game::displayWinner()
{
    if (isSilentMode && isLoadMode)
        return;
    system("cls");
    string winner[] = {
        "##########################################",
        "#      *            *             *      #",
        "#       **         ***          **       #",
        "#        ***      *****       ***        #",
        "#         ****   *******    ****         #",
        "#          ********************          #",
        "#        ************************        #",
        "#         **********************         #",
        "#                WINNER!                 #",
        "#               GOOD JOB!                #",
        "#                                        #",
        "#                                        #",
        "##########################################"
    };
    menuY = MENU_POS_Y;

    for (const auto& line : winner) {
        gotoxy(menuX, menuY);
        cout << line;
        menuY++;
    }
    Sleep(7000); // Show the message for 7 seconds


}

bool Game::handleCollisions() {
    bool barrelCollision = handleBarrelCollisions();
    bool ghostCollision = handleGhostCollisions();
    bool xghostCollision = handleXghostCollisions();

    if (barrelCollision || ghostCollision || xghostCollision) {
        if (isSaveMode) {
            expectedResults.addResult(gameTime, Results::hitEnemy);  // 🔴 Save Mario's death
        }
        return true;  // Mario died
    }

    return false;
}



void Game::cleanupObjects() {
    // Remove barrels marked for deletion
    barrels.erase(
        std::remove_if(barrels.begin(), barrels.end(),
            [](const Barrel& barrel) { return barrel.getRemoved(); }),
        barrels.end());
}


void Game::restartLevel() {
    //cls, print initial board, put mario in init pos with no move, brls - empty array
    if(!(isSilentMode && isLoadMode))
        system("cls");
    ghosts->clear();
    Xghosts->clear();
    barrels.clear();
    board.reset();
    mario.init(board.getMarioX(), board.getMarioY(), &board.getGhosts(), &board.getXghosts(), isSilentMode, isLoadMode);
    mario.setBoard(board);
    this->setHammerX(board.getHammerX());
    this->setHammerY(board.getHammerY());
    this->ghosts = &board.getGhosts();
    for (auto& ghost : *ghosts) {
        ghost.isLoadMode = isLoadMode;
        ghost.isSilentMode = isSilentMode;
    }
    this->Xghosts = &board.getXghosts();
    for (auto& xghost : *Xghosts) {
        xghost.isLoadMode = isLoadMode;
        xghost.isSilentMode = isSilentMode;
    }
    this->_score -= this->_addedScore;
    this->_addedScore = 0;

    if (!(isSilentMode && isLoadMode)) {
        board.print();
        mario.draw();
        gotoxy(LIVE_PRINT_POS_X, LIVE_PRINT_POS_Y); // live's printing starting position
        cout << "LIVES: " << livesVisual;
        gotoxy(SCORE_PRINT_POS_X, SCORE_PRINT_POS_Y);
        cout << score << _score;
    }
}

void Game::resetGame() {
    // Clear the screen and reprint the board
    if (!(isSilentMode && isLoadMode))
        system("cls");

    // Reset lives and lives visual
    _lives = INIT_NUM_LIVES;
    livesVisual = "@@@";

    // Clear active barrels
    barrels.clear();

    board.reset();

    mario.init(board.getMarioX(), board.getMarioY(), &board.getGhosts(), &board.getXghosts(), isSilentMode, isLoadMode);

    mario.setBoard(board);

    this->setHammerX(board.getHammerX());

    this->setHammerY(board.getHammerY());

    this->ghosts = &board.getGhosts();
    for (auto& ghost : *ghosts) {
        ghost.isLoadMode = isLoadMode;
        ghost.isSilentMode = isSilentMode;
    }
    this->Xghosts = &board.getXghosts();
    for (auto& xghost : *Xghosts) {
        xghost.isLoadMode = isLoadMode;
        xghost.isSilentMode = isSilentMode;
    }

    this->_addedScore = 0;

    // Reset timing
    nextBarrelSpawnTime = GetTickCount() + 4000; //4 seconds delay for the first barrel

    // Reset game state flags
    paused = false;
    running = true;

    menuY = MENU_POS_Y;

    recordedSteps.clear();

    if (isLoadMode) {
        std::string nextStepsFile = "dkong_" + formatScreenNumber(currentLevel) + ".steps";
        std::string nextResultsFile = "dkong_" + formatScreenNumber(currentLevel) + ".result";

        // Load steps and results for the next level
        recordedSteps = Steps::loadSteps(nextStepsFile);
        expectedResults = Results::loadResults(nextResultsFile);
    }

}

void Game::executeStep(char step) {
    switch (step) {
    case 'w': mario.keyPressed('w'); break;
    case 'a': mario.keyPressed('a'); break;
    case 'd': mario.keyPressed('d'); break;
    case 'x': mario.keyPressed('x'); break;
    case 's': mario.keyPressed('s'); break;
    case 'p': mario.keyPressed('p'); break; // Hammer pickup
    }
}

std::string Game::formatScreenNumber(int screenIndex) const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << (screenIndex);
    return oss.str();
}

void Game::saveSteps() {
    std::string filename = "dkong_" + formatScreenNumber(board.getScreenIndx() +1) + ".steps";
    recordedSteps.saveSteps(filename);
    recordedSteps.clear();

}


void Game::saveResults(bool val) {
    std::string filename = "dkong_" + formatScreenNumber(board.getScreenIndx() +1) + ".result";
    if (val) {
        expectedResults.addResult(gameTime, Results::noResult);
    }
    else if (!val) {
        expectedResults.addResult(gameTime, Results::finished);
    }
    expectedResults.addResult(_score, Results::SCORE);
    expectedResults.saveResults(filename);
    expectedResults.clear();

}


void Game::validateResults() {
    gotoxy(0, this->currentLevel);
    for (auto& result : expectedResults.getResults()) {
        
        if (!expectedResults.validateResult(gameTime, result.second)) {
            std::cout << "Test Failed at time: " << gameTime << " Due to " << Results::ResultValueToString(static_cast<Results::ResultValue>(result.second)) << '\n';
            return;
        }
        else
            cout << "Test for level: " <<currentLevel<< " Passed" << endl;
    }

}




void Game::run() {
    //when in load mode, wer'e dorsim the last result and steps files, we need to handle better the 
    //way we run the game in load mode.
    //but when the steps and result files are good the load mood runs good, except it doesnt end 
    // when mario dies 3 times (didnt check for when mario completes 3 levels)
    while (true) {
        if (!isLoadMode) {
            showMenu();  // Show menu in normal or save mode
        }

        if (!running && !isLoadMode) {
            break;
        }
        if (!(isSilentMode && isLoadMode))
            system("cls");
        if (isLoadFinished)
            break;
        resetGame();
        if (!(isSilentMode && isLoadMode)){
            board.print();
            mario.draw();
            gotoxy(2, 1);
            cout << "LIVES: " << livesVisual;
            gotoxy(SCORE_PRINT_POS_X, SCORE_PRINT_POS_Y);
            cout << score << _score;
        }
        while (running) {
            

            if (isLoadMode) {
                // Execute saved steps (ignore user input)
                if (recordedSteps.isNextStepOnIteration(gameTime)) {
                    char step = recordedSteps.popStep();
                    executeStep(step);
                }
            }
            else {
                processInput(); // Normal or save mode
            }

            char marioCurrChar = board.getChar(mario.getX(), mario.getY());

            if (marioCurrChar == PAULINE) {
                if(!isLoadMode){
                    saveSteps();
                    saveResults(false);
                }        

                if (this->board.getScreenIndx() == 2) {
                    displayWinner();
                    if(isLoadMode){
                        isLoadFinished = true;
                    }
                    running = false;
               
                    if (isLoadMode && isSilentMode) {
                        validateResults();
                    }
                    break;
                }
                else {
                    if (isLoadMode && isSilentMode) {
                        validateResults();
                    }
                    currentLevel++;
                    gameTime = 0;
                    this->board.setScreenIndex(this->board.getScreenIndx() + 2);
                    resetGame();
                    if (!(isSilentMode && isLoadMode)) {
                        board.print();
                        mario.draw();
                        gotoxy(2, 1);
                        cout << "LIVES: " << livesVisual;
                        gotoxy(SCORE_PRINT_POS_X, SCORE_PRINT_POS_Y);
                        cout << score;
                    }
                    continue;
                }
            }

            if (!paused) {
                if (GetTickCount() >= nextBarrelSpawnTime) {
                    spawnBarrel();
                }

                bool isMarioDead = updateObjects();

                if (isMarioDead) {
                    if (_lives == 0) {
                        if(!isLoadMode){
                            saveSteps();  //  Ensure final steps are saved
                            saveResults(isMarioDead); //  Ensure final results are saved
                        }
                        if (isLoadMode && isSilentMode) {
                            validateResults();
                            running = false;
                            break;
                        }
                        running = false;
                        if (isLoadMode) {
                            isLoadFinished = true;
                            break;
                        }
                    }
                    else {
                        restartLevel();

                    }
                }
                else {
                    cleanupObjects();
                    if (!(isSilentMode && isLoadMode)) {
                        mario.draw();
                        renderObjects();
                    }
                }
            }

            if (!isSilentMode) {
                Sleep(FRAME_DELAY_MS); // Normal frame rate control
            }
            gameTime++;
        }
        _score = 0;
    }
}

