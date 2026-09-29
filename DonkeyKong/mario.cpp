#include "mario.h"

void Mario::init(int startX, int startY, std::vector<Ghost>* ghosts, std::vector<Xghost>* Xghosts, bool isSilent, bool isLoad) {
    x = startX;
    y = startY;
    dir.x = START_DIR;
    dir.y = START_DIR;
    _jumpStepsCount = 0;
    _fallStepCount = 0;
    _isFallTooMuch = false;
    _isOnFloor = true;
    _gotHammer = false;
    _ghosts = ghosts;
    _Xghosts = Xghosts;
    isLoadMode = isLoad;
    isSilentMode = isSilent;
}

void Mario::fall()
{
    _isOnFloor = false;
    char underChar = pBoard->getChar(x, y + 1);
    if (underChar == ' ') {
        erase();
        y++;
        //draw();
    }

    _fallStepCount++;


}

void Mario::jump() {

    if (this->_jumpStepsCount < 2)
    {
        char currentChar = pBoard->getChar(x, y);
        char newChar = pBoard->getChar(x + dir.x, y - 1); // Character at the new position
        if (currentChar == LADDER)
        {
            dir.x = dir.y = 0;
            return;
        }
        if (newChar == WALL || newChar == FLOOR_EQUA || newChar == FLOOR_LEFT || newChar == FLOOR_RIGHT) {
            fall();
            dir.x = dir.y = 0;
            _jumpStepsCount = 0;
            return; // Stop jumping if hitting a wall, floor
        }
        erase();  // Erase Mario from the current position
        x += dir.x; // Apply horizontal movement
        y--;        // Move up
        //draw();     // Draw Mario in the new position
    }
    else
    {
        char currentChar = pBoard->getChar(x, y);
        char newChar = pBoard->getChar(x + dir.x, y + 1); // Character at the new position
        if (currentChar == LADDER)
        {
            dir.x = dir.y = 0;
            return;
        }
        if (currentChar == WALL || currentChar == FLOOR_EQUA || currentChar == FLOOR_LEFT ||
            currentChar == FLOOR_RIGHT) {
            fall();
            dir.x = dir.y = 0;
            return; // Stop falling if hitting a wall, floor     
        }
        if (newChar == WALL || newChar == FLOOR_EQUA || newChar == FLOOR_LEFT || newChar == FLOOR_RIGHT) {
            fall();
            dir.x = dir.y = 0;
            _jumpStepsCount = 0;
            return; // Stop falling if hitting a wall, floor      
        }
        erase();  // Erase Mario from the current position
        x += dir.x; // Apply horizontal movement
        y++;        // Move down
       // draw();     // Draw Mario in the new position
    }

    this->_jumpStepsCount++;
    if (_jumpStepsCount == HALFWAY_JUMP)
        dir.y = 1;
    if (_jumpStepsCount == TOTAL_JUMP_STEPS) {
        dir.y = 0; // Reset vertical direction after jumping
        _jumpStepsCount = 0;
    }

}

void Mario::handleJump(char ch)
{
    if (!(_jumpStepsCount == 0 && ch == ' ')) {
        jump();
        return;
    }
    dir.y = 0;
    return;
}

void Mario::handleDownLadder(int newX)
{
    erase();
    x = newX;
    y += 2 * dir.y;
}

void Mario::handeLadder(int newX, int newY)
{
    erase();
    _jumpStepsCount = 0;
    if (_fallStepCount >= MAX_FALL_STEPS)
        _isFallTooMuch = true;
    _fallStepCount = 0;

    x = newX;
    y = newY; // Allow movement onto the ladder
}

void Mario::handleOnLadder(char ch, int newX, int newY)
{
    if (dir.x == -1 || dir.x == 1) {
        dir.x = 0;
        //draw(); // Redraw Mario at the current position
        _jumpStepsCount = 0;
        if (_fallStepCount >= MAX_FALL_STEPS)
            _isFallTooMuch = true;
        _fallStepCount = 0;

        return;
    }
    else if (ch == FLOOR_EQUA || ch == FLOOR_LEFT || ch == FLOOR_RIGHT) {
        x = newX;
        y = newY;
    }
}

void Mario::handleFreeFall()
{
    if (pBoard->getChar(x, y + 1) == ' ') {
        fall();
        char underChar = pBoard->getChar(x, y + 1);
        if (underChar == FLOOR_EQUA || underChar == FLOOR_LEFT || underChar == FLOOR_RIGHT || underChar == WALL)
            _isOnFloor = true;
        if (_fallStepCount >= MAX_FALL_STEPS)
            _isFallTooMuch = true;

    }
}

void Mario::handleAfterFall(char underChar)
{
    if (underChar == FLOOR_EQUA || underChar == FLOOR_LEFT || underChar == FLOOR_RIGHT) {
        _isOnFloor = true;
        if (_fallStepCount >= MAX_FALL_STEPS) {
            _isFallTooMuch = true;
            _fallStepCount = 0;
            return;
        }
        _fallStepCount = 0;
    }
}

void Mario::normalMove(int newX, int newY)
{
    erase();
    x = newX;
    y = newY;
}

void Mario::restoreChar(char prevChar)
{
    if (prevChar == LADDER) {
        if (dir.x != 0 || dir.y != 0)
        {
            if (!(isSilentMode && isLoadMode)){
                gotoxy(x - dir.x, y - dir.y); // Go to Mario's previous position
                std::cout << LADDER;// Redraw the ladder
            }
        }
    }
    else if (prevChar == FLOOR_EQUA) {
        if (dir.x != 0 || dir.y != 0)
        {
            if (!(isSilentMode && isLoadMode)){
                gotoxy(x - dir.x, y - dir.y); // Go to Mario's previous position
                std::cout << FLOOR_EQUA;
            }
            dir.x = dir.y = 0;
        }
    }
    else if (prevChar == FLOOR_LEFT) {
        if (dir.x != 0 || dir.y != 0)
        {
            if (!(isSilentMode && isLoadMode)){
                gotoxy(x - dir.x, y - dir.y); // Go to Mario's previous position
                std::cout << FLOOR_LEFT;
            }
            dir.x = dir.y = 0;
        }
    }
    else if (prevChar == FLOOR_RIGHT) {
        if (dir.x != 0 || dir.y != 0)
        {
            if (!(isSilentMode && isLoadMode)){
                gotoxy(x - dir.x, y - dir.y); // Go to Mario's previous position
                std::cout << FLOOR_RIGHT;
            }
            dir.x = dir.y = 0;
        }
    }

    if (prevChar == DONKEYKONG) {
        gotoxy(x - dir.x, y - dir.y); // Go to Mario's previous position
        std::cout << DONKEYKONG;
    }
}

void Mario::keyPressed(char key)
{
    for (size_t i = 0; i < numKeys; i++) {
        if (std::tolower(key) == keys[i]) {
            if (directions[i].y == -1) { // UP key
                dir.y = -1;              // Set vertical direction to up
            }
            else {
                dir = directions[i];     // Update both x and y for other keys
            }
            return;
        }

    }
    if (std::tolower(key) == 'p') {
        _usingHammer = true;
    }
}

void Mario::handleHammerPickup() {
    if (_ghosts) { // Check if _ghosts is not null
        for (auto& ghost : *_ghosts) {
            ghost.setIsHammer(); // Call the Ghost method to set the hammer state
        }
    }
    if (_Xghosts) { // Check if _ghosts is not null
        for (auto& xghost : *_Xghosts) {
            xghost.setIsHammer(); // Call the Ghost method to set the hammer state
        }
    }
}

void Mario::move()
{
    int newX = x + dir.x;
    int newY = y + dir.y;
    char currentChar = pBoard->getChar(x, y);     // Character at Mario's current position
    char newChar = pBoard->getChar(newX, newY);  // Character at Mario's new position
    char underChar = pBoard->getChar(x, y + 1);
    char upperChar = pBoard->getChar(x, y - 1);
    char prevChar = pBoard->getChar(x, y);
    char underUnderChar = pBoard->getChar(x, y + 2);

    handleAfterFall(underChar);

    if (currentChar == HAMMER)
    {
        this->_gotHammer = true;
        handleHammerPickup();
    }

    if (isPauline(currentChar))
    {
        handlePauline();
        return;
    }

    if (isLadder(currentChar) && !(isSilentMode && isLoadMode))
        draw();

    //Check for walls and other non-walkable characters
    if (checkNonWalkableChars(newChar)) {
        if (fallTooMuch())
            _isFallTooMuch = true;

        _fallStepCount = 0;

        if (checkNormalLadderMove(currentChar, newChar, underUnderChar)) {
            normalMove(newX, newY);
        }
        else if (checkDownLadderMove(currentChar, underUnderChar)) {
            handleDownLadder(newX);
        }
        else {
            dir = { 0, 0 }; // Stop movement
            //draw(); // Redraw Mario at the current position
            return;
        }
    }

    else if (isStillJump(currentChar)) {
        handleJump(underChar);
        return;
    }
    //down a ladder
    else if (currentChar == ' ' && underUnderChar == LADDER && dir.y == 1) {
        handleDownLadder(newX);
    }
    else if (isFloorAndDown(currentChar)) {
        erase();
        y += -1;
    }
    else if (isAbleToJump(currentChar, underChar)) {
        jump();
        return;
    }
    //Handle ladder interaction
    else if (newChar == LADDER && dir.y != 0) {
        handeLadder(newX, newY);
    }
    else if (isOnLadder(currentChar, underChar)) {
        handleOnLadder(upperChar, newX, newY);
    }

    //Normal movement
    else {
        normalMove(newX, newY);
    }

    //Draw Mario in his new position
    //draw();

    //Restore ladder ('H') at Mario's previous position if he moved off it
    restoreChar(prevChar);
    //Handling free fall
    handleFreeFall();
}

