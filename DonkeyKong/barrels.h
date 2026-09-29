#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>
#include "utilits.h"
#include "board.h"
#include "Enemy.h"


class Barrel : public Enemy {

    struct Direction {
        int x, y;
    };
    static constexpr char FLOOR_EQUA = '=';
    static constexpr char FLOOR_RIGHT = '>';
    static constexpr char FLOOR_LEFT = '<';
    static constexpr char LADDER = 'H';
    static constexpr char WALL = 'Q';
    static constexpr char DONKEYKONG = '&';
    static constexpr char PAULINE = '$';
    static constexpr char GHOST = 'x';
    static constexpr int MAX_FALL_COUNT = 8;
    
    static constexpr Direction directions[] = {
        {0, -1}, {-1, 0}, {0, 1}, {1, 0}, {0, 0}
    };

   
    int fallDistance = 0; // Tracks how far the barrel has fallen
    bool toBeRemoved = false;

   

    Direction dir{ -1, 0 }; // Default direction (left)
   

 
public:
    Barrel() : Enemy(66, 4, 'O') {}

    bool isSilentMode = false;
    bool isLoadMode = false;

    void fall();

    void move();

    void exploded();

    void determineDirection(char floorChar);

    void restoreChar(char c);

    bool getRemoved() const {
        return toBeRemoved;
    }

    int getDirX() const {
        return dir.x;
    }

    void setDirX(int change) {
        dir.x *= change;
    }

};