#include "Xghost.h"

void Xghost::move()
{
    DWORD currentTime = GetTickCount();
    if (currentTime - lastMoveTime < MOVE_INTERVAL_MS) {
        return; // Skip this move if not enough time has passed
    }
    lastMoveTime = currentTime; // Update the last move time
    // Calculate new position
    int newX = getX() + dir.x;
    char newChar = pBoard->getChar(getX() + dir.x, getY() + dir.y);
    char newUnderChar = pBoard->getChar(newX, getY() + 1);
    char UnderUnderChar = pBoard->getChar(getX(), getY() + 2);
    char currentChar = pBoard->getChar(getX(), getY());
    char upperChar = pBoard->getChar(getX(), getY() - 1);

    // Decide direction based on probability
    if (randomProbability() > SAME_DIRECTION_PROB || newUnderChar == ' ' || newUnderChar == 'Q') {
        dir = { -dir.x, 0 }; // Change direction if at the edge or a wall
        newX = getX() + dir.x;
        newUnderChar = pBoard->getChar(newX, getY() + 1);
        newChar = pBoard->getChar(getX() + dir.x, getY());
    }

    if (                (isFloor(currentChar) || currentChar == LADDER) &&
        (isFloor(upperChar) || upperChar == LADDER || upperChar == ' ') &&
                                                  isGoingDown == false) {
        isGoingUp = true;
        isClimbing = true;
        climbLadder(UP);
        if (pBoard->getChar(getX(), getY()) == ' ') {
            isGoingUp = isClimbing = false;
            dir.x *= -1;
        }
    }
    else if ((isFloor(currentChar) || UnderUnderChar == LADDER ||
              currentChar == LADDER) && isGoingUp == false) {
        isGoingDown = true;
        isClimbing = true;
        climbLadder(DOWN);
    }
    // Re-calculatin ghost's surrounding
    newX = getX() + dir.x;
    newUnderChar = pBoard->getChar(newX, getY() + 1);
    newChar = pBoard->getChar(getX() + dir.x, getY() + dir.y);

    // Handle movement
    if (newUnderChar != ' ' && newChar != 'Q' && isClimbing == false) {
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

void Xghost::climbLadder(int dirY)
{
    
    char currentChar = pBoard->getChar(getX(), getY());
    char newChar = pBoard->getChar(getX(), getY() + dirY);
    char underUnderChar = pBoard->getChar(getX(), getY() + 2);
    if ( dirY == UP && currentChar == ' ') {
        isGoingUp = false;
        isClimbing = false;
        return;
    }
    if (dirY == DOWN && (isFloor(newChar) && !isLadder(underUnderChar))) {
        isClimbing = false;
        isGoingDown = false;
        return;
    }

    erase();
    setY(getY() + dirY);
    //draw();

    if (currentChar == LADDER && !(isSilentMode && isLoadMode)) {
        gotoxy(getX(), getY() - dirY);
        cout << LADDER;
    }
    if (isFloor(currentChar) && !(isSilentMode && isLoadMode)) {
        gotoxy(getX(), getY() - dirY);
        cout << currentChar;
    }

}
