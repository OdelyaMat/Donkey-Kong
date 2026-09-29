#pragma once
#include<iostream>
#include <Windows.h>
#include "utilits.h"
#include "board.h"

class Mario
{
	static constexpr char keys[] = { 'w', 'a', 'x', 'd', 's' };
	static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]);
	struct Direction { int x, y; }; // inner private struct
	// the directions array order is exactly the same as the keys array - must keep it that way


	static constexpr Direction directions[] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0}, {0, 0} };
	static constexpr char FLOOR_EQUA = '=';
	static constexpr char FLOOR_RIGHT = '<';
	static constexpr char FLOOR_LEFT = '>';
	static constexpr char LADDER = 'H';
	static constexpr char WALL = 'Q';
	static constexpr char DONKEYKONG = '&';
	static constexpr char PAULINE = '$';
	static constexpr char HAMMER = 'p';


	static constexpr int TOTAL_JUMP_STEPS = 4;
	static constexpr int HALFWAY_JUMP = 2;
	static constexpr int MAX_FALL_STEPS = 5;
	static constexpr int START_X = 1;
	static constexpr int START_DIR = 0;
	static constexpr int START_Y = 22;


	Direction dir{ START_DIR, START_DIR };					// current direction: dir.x, dir.y
	Board* pBoard = nullptr;
	std::vector<Ghost>* _ghosts = nullptr;                  // Vector to store ghosts
	std::vector<Xghost>* _Xghosts = nullptr;
	char ch = '@';
	int x = START_X, y = START_Y;
	int _jumpStepsCount = 0;
	int _fallStepCount = 0;
	bool _isFallTooMuch = false;
	bool _isOnFloor = true;
	bool _gotHammer = false;
	bool _usingHammer = false;
	void draw(char c) const {
		gotoxy(x, y);
		std::cout << c;
	}

public:
	bool isSilentMode = false;
	bool isLoadMode = false;
	void init(int startX, int startY, std::vector<Ghost>* ghosts, std::vector<Xghost>* Xghosts, bool isSilent, bool isLoad);
	void handleJump(char ch);
	void handleDownLadder(int newX);
	void handeLadder(int newX, int newY);
	void handleOnLadder(char ch, int newX, int newY);
	void handleFreeFall();
	void handleAfterFall(char underChar);
	void normalMove(int newX, int newY);
	void restoreChar(char ch);
	void keyPressed(char key);
	void handleHammerPickup();
	void move();
	void fall();
	void jump();

	void handlePauline() {
		dir.x = dir.y = 0;
	}

	void draw() const {
		draw(ch);
	}

	void erase() const {
		draw(' ');
	}

	void setBoard(Board& board) {
		pBoard = &board;
	}

	void resetUseHammer() {
		_usingHammer = false;
	}

	int getDirX() const {
		return dir.x;
	}

	int getDirY() const {
		return dir.y;
	}

	int getHammer() const {
		return _gotHammer;
	}

	int getUseHammer() const {
		return _usingHammer;
	}
	int getX() const {
		return x;
	}

	bool getFallTooMuch() const {
		return this->_isFallTooMuch;
	}

	bool getIsOnFloor() const {
		return this->_isOnFloor;
	}

	int getY() const {
		return y;
	}

	bool isPauline(char ch) const {
		return ch == PAULINE;
	}

	bool isLadder(char ch) const {
		return ch == LADDER;
	}

	bool isOnLadder(char ch, char uc) const {
		return (ch == LADDER && uc == LADDER);
	}

	bool checkNonWalkableChars(char ch) const {
		return ch == WALL || ch == FLOOR_EQUA || ch == FLOOR_LEFT || ch == FLOOR_RIGHT;
	}

	bool fallTooMuch() const {
		return _fallStepCount >= MAX_FALL_STEPS;
	}

	bool checkNormalLadderMove(char ch, char nc, char uc) const {
		return (ch == LADDER && nc != WALL && uc != ' ' && uc != WALL);
	}

	bool checkDownLadderMove(char ch, char uc) const {
		return(ch == ' ' && uc == LADDER && dir.y == 1);
	}
	bool isStillJump(char ch) const {
		return (ch == ' ' && (dir.y == -1 || _jumpStepsCount != 0));
	}
	bool isFloorAndDown(char ch) const {
		return ((ch == FLOOR_EQUA || ch == FLOOR_RIGHT || ch == FLOOR_LEFT) && dir.y == -1);
	}
	bool isAbleToJump(char ch, char uc) const {
		return (dir.y == -1 && ch != LADDER && uc != LADDER && uc != ' ');
	}
	bool isPrevLadder(char ch) const {
		return ch == LADDER;
	}

};
