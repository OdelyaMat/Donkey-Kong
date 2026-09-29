#include "Ghosts.h"

void Ghost::move() {

    DWORD currentTime = GetTickCount();
    if (currentTime - lastMoveTime < MOVE_INTERVAL_MS) {
        return; // Skip this move if not enough time has passed
    }
    lastMoveTime = currentTime; // Update the last move time
    // Calculate new position
    int newX = getX() + dir.x;
    char newChar = pBoard->getChar(getX() + dir.x, getY());
    char newUnderChar = pBoard->getChar(newX, getY() + 1);

    // Decide direction based on probability
    if (randomProbability() > SAME_DIRECTION_PROB || newUnderChar == ' ' || newUnderChar == 'Q') {
        dir = { -dir.x, 0 }; // Change direction if at the edge or a wall
        newX = getX() + dir.x;
        newUnderChar = pBoard->getChar(newX, getY() + 1);
        newChar = pBoard->getChar(getX() + dir.x, getY());
    }

    // Handle movement
    if (newUnderChar != ' ' && newChar != 'Q') {
        erase();
        setX(newX);
        //draw();
    }

    if (pBoard->getChar(getX() - dir.x, getY()) == LADDER && !(isSilentMode && isLoadMode)) {
        gotoxy(getX() - dir.x, getY());
        cout << LADDER;
    }
    else if (pBoard->getChar(getX() - dir.x, getY()) == PAULINE && !(isSilentMode && isLoadMode)) {
        gotoxy(getX() - dir.x, getY());
        cout << PAULINE;
    }
    else if (pBoard->getChar(getX() - dir.x, getY()) == DONKEYKONG && !(isSilentMode && isLoadMode)) {
        gotoxy(getX() - dir.x, getY());
        cout << DONKEYKONG;
    }
    else if (pBoard->getChar(getX() - dir.x, getY()) == HAMMER && getIsHammer() && !(isSilentMode && isLoadMode)) {
        gotoxy(getX() - dir.x, getY());
        cout << HAMMER;
    }
}


