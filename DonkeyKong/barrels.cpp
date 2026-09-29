#include "barrels.h"
#include <array>
#include <thread>

using namespace std;

void Barrel::fall() {
    // Simulates gravity for the barrel
    int newX = getX() + dir.x; 
    char currentChar = pBoard->getChar(getX(), getY());
    char underChar = pBoard->getChar(newX, getY() + 1);
    if (underChar == ' ' || underChar == LADDER) { // If there's nothing below, keep falling
        erase();
        int newY = getY() + 1;
        setY(newY);
        setX(newX); 
        fallDistance++;
        //draw();

        if (currentChar == LADDER && !(isSilentMode && isLoadMode)) {
            gotoxy(getX() - dir.x, getY() - 1);
            std::cout << LADDER;
        }
    }
}

void Barrel::move() {
    // Handle horizontal movement first
    char underChar = pBoard->getChar(getX(), getY() + 1);
    char currentChar = pBoard->getChar(getX(), getY());
    char newChar = pBoard->getChar(getX() + dir.x, getY() + dir.y);

    if (underChar != ' ' && fallDistance >= MAX_FALL_COUNT) {
        toBeRemoved = true;
        return;
    }


    if (newChar == WALL)
        exploded();

    if (underChar == FLOOR_LEFT) {
        dir.x = -1; // Move left
    }

    else if (underChar == FLOOR_RIGHT) {
        dir.x = 1;  // Move right
    }

    else if (underChar == FLOOR_EQUA) {
        // Continue in the same direction as before
        // Direction remains unchanged
    }

    else if (pBoard->getChar(getX() + dir.x, getY()) == WALL) {
        dir = { 0, 0 }; // Stop movement
        return;
    }


    // Horizontal movement
    int newX = getX() + dir.x;
    char floorChar = pBoard->getChar(newX, getY() + 1); // Check the new position on the floor

    if (floorChar == ' ' || floorChar == LADDER) {
        fall();
    }
    floorChar = pBoard->getChar(getX(), getY() + 1);
    if (floorChar != ' ') { // Ensure we don't overwrite floor chars
        erase();
        setX(newX); // Update the position
        //draw();
        if (currentChar == LADDER && !(isSilentMode && isLoadMode)) {
            gotoxy(getX() - dir.x, getY());
            std::cout << LADDER;
        }
        else if (currentChar == PAULINE && !(isSilentMode && isLoadMode)) {
            gotoxy(getX() - dir.x, getY());
            std::cout << PAULINE;
        }
        else if (currentChar == DONKEYKONG && !(isSilentMode && isLoadMode)) {
            gotoxy(getX() - dir.x, getY());
            std::cout << DONKEYKONG;
        }
        if (fallDistance < MAX_FALL_COUNT)
            fallDistance = 0;

    }
}

void Barrel::determineDirection(char floorChar) {
    if (floorChar == FLOOR_LEFT) {
        dir = { -1, 0 }; // Move left
    }
    else if (floorChar == FLOOR_RIGHT) {
        dir = { 1, 0 }; // Move right
    }
    else if (floorChar == FLOOR_EQUA) {
        // No change in direction, retain the current direction
        // dir remains unchanged
    }
}

void Barrel::exploded() {
    erase();
    toBeRemoved = true; // Mark the barrel for removal after the explosion
}