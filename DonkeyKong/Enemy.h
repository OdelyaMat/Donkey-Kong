#pragma once

#include <iostream>
#include "utilits.h"

class Board;

class Enemy{
private:

    int x, y;
   
    char ch;

protected: 
    static constexpr double SAME_DIRECTION_PROB = 0.95;
    static constexpr int MOVE_INTERVAL_MS = 50;
    static constexpr int UP = -1;
    static constexpr int DOWN = 1;
    static constexpr char X_GHOST_CHAR = 'X';
    static constexpr char GHOST_CHAR = 'x';
    static constexpr char LADDER = 'H';
    static constexpr char DONKEYKONG = '&';
    static constexpr char PAULINE = '$';
    static constexpr char HAMMER = 'p';
    static constexpr char FLOOR_EQUA = '=';
    static constexpr char FLOOR_RIGHT = '<';
    static constexpr char FLOOR_LEFT = '>';

    struct Direction {
        int x, y;
    };
    Board* pBoard = nullptr;
    bool _isHammer = true;                  // tell if a hammer is exist on board (mario did or didnt take it already)

    void draw(char c) const {
        gotoxy(x, y);
        std::cout << c;
    }

    static double randomProbability() {
        return (double)rand() / RAND_MAX;
    }

    static Direction randomDirection() {
        return (rand() % 2 == 0) ? Direction{ -1, 0 } : Direction{ 1, 0 };
    }
public:
    bool isSilentMode = false;
    bool isLoadMode = false;

    Enemy(int startX, int startY, char c)
    {
        x = startX;
        y = startY;
        ch = c;
    }
   
    void setBoard(Board& board) {
        pBoard = &board;
    }
    
    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    bool getIsHammer() const {
        return _isHammer;
    }

    void erase() const {
        draw(' ');
    }

    void draw() const {
        draw(ch);
    }

    void setX(int _x) {
        x = _x;
    }

    void setY(int _y) {
        y = _y;
    }





};



