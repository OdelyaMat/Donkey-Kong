#pragma once
#include <vector>
#include <string>
#include "Ghosts.h"
#include "Xghost.h"
#include <fstream>
#include <filesystem>

class Ghost;
class Xghost;

class Board {

	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 25;
	static constexpr char DONKEYKONG = '&';
	static constexpr char MARIO = '@';
	static constexpr char HAMMER = 'p';
	static constexpr char GHOST = 'x';
	static constexpr char X_GHOST = 'X';
	static constexpr char PAULINE = '$';

	int _marioX = -1;
	int _marioY = -1;
	int _hammerX = -1;
	int _hammerY = -1;
	int _donkeyX = -1;
	int _donkeyY = -1;
	int _screenCount = 0; // Counter for the number of screen files
	int _screenIdx;

	char currentBoard[MAX_Y][MAX_X + 1]; // +1 for null terminator

	std::vector<std::string> _screenNames; // Vector to store screen file names
	std::vector<Ghost> ghosts;
	std::vector<Xghost> Xghosts;



	const char* originalBoard[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                                                                              Q", // 1
		  "Q                                                                              Q", // 2
		  "Q                                                                              Q", // 3
		  "Q                                                                    &   $     Q", // 4
		  "Q            <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Q", // 5
		  "Q                H                   H               H                         Q", // 6
		  "Q                H                   H               H                         Q", // 7
		  "Q                H                   H               H                         Q", // 8
		  "Q                H                   H               H                         Q", // 9
		  "Q>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>                Q", // 10
		  "Q                        H                   H                                 Q", // 11
		  "Q                        H                   H                                 Q", // 12
		  "Q                        H                   H                                 Q", // 13
		  "Q                        H                   H                                 Q", // 14
		  "Q             <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Q", // 15
		  "Q                                H                H                            Q", // 16
		  "Q                                H                H                            Q", // 17
		  "Q                                H                H                            Q", // 18
		  "Q                                H                H                            Q", // 19
		  "Q                            =========            H                            Q", // 20
		  "Q                  =========                      H                            Q", // 21
		  "Q                                                 H                            Q", // 22
		  "Q==============================================================================Q", // 23
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};


public:

	Board();

	bool reset();//returns true if there is a next level

	void loadScreenList();

	void checkScreen(std::vector<std::string>& screenNames);

	void print() const;

	void setScreenIndex(int screen) {
		_screenIdx = screen - 1;
	}

	int getMax_X() const {
		return MAX_X;
	}

	int getMax_Y() const {
		return MAX_Y;
	}

	int getScreenIndx() const {
		return _screenIdx;
	}

	int getMarioX() const {
		return _marioX;
	}

	int getMarioY() const {
		return _marioY;
	}

	int getDonkeyX() const {
		return _donkeyX;
	}

	int getDonkeyY() const {
		return _donkeyY;
	}

	int getHammerY() const {
		return _hammerY;
	}

	int getHammerX() const {
		return _hammerX;
	}

	char getChar(int x, int y) const {
		return currentBoard[y][x];
	}

	std::vector<Ghost>& getGhosts() {
		return ghosts;
	}
	std::vector<Xghost>& getXghosts() {
		return Xghosts;
	}

};
