#pragma once
#include "utilits.h"
#include "board.h"
#include <vector>
#include <random>
#include <iostream>
#include "Enemy.h"
class Xghost : public Enemy
{
private:
    Direction dir{ 1, 0 };                  // Default direction: right
    DWORD lastMoveTime = 0;                 // Last time the ghost moved
    bool isGoingDown;
    bool isGoingUp;
    bool isClimbing;
public:
	Xghost(int startX, int startY, char c) : Enemy(startX, startY, c) {
        dir.x = 1;
        dir.y = 0;
        isGoingDown = true;
        isGoingUp = false;
        isClimbing = false;
    }
    void setIsHammer() {
        _isHammer = false;
    }
    int getDirX() const {
        return dir.x;
    }
    int getDirY() const {
        return dir.y;
    }

    void changeDirX() {
        dir.x *= -1;
    }
    void move();

    void climbLadder(int dirY);

    bool isFloor(char ch) const {
        return (ch == FLOOR_EQUA || ch == FLOOR_RIGHT || ch == FLOOR_LEFT);
    }

    bool isLadder(char ch) const {
        return ch == LADDER;
    }
};

