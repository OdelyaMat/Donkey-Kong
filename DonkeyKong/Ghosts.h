#pragma once
#include "utilits.h"
#include "board.h"
#include <vector>
#include <random>
#include <iostream>
#include "Enemy.h"


using namespace std;
class Board;



class Ghost : public Enemy{
private:






    Direction dir{ 1, 0 };                  // Default direction: right
    
   
    DWORD lastMoveTime = 0;                 // Last time the ghost moved


    



public:
   

    Ghost(int startX, int startY, char c) : Enemy(startX, startY, c) {
        dir.x = 1;
        dir.y = 0;
    }

    void move();

    void setIsHammer() {
        _isHammer = false;
    }

    int getDirX() const {
        return dir.x;
    }

    void changeDirX() {
        dir.x *= -1;
    }
    


    
};

