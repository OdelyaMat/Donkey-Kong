#include <cstring>
#include <iostream>
#include "board.h"
#include "Game.h"
#include <filesystem>

using namespace std;

Board::Board() {
	loadScreenList();
	_screenIdx = 0;
	checkScreen(_screenNames);
	_screenCount = _screenNames.size();
}

bool Board::reset() {

	//the screens in the vector are already after validation checks

	ghosts.clear();
	Xghosts.clear();
	if (_screenIdx < _screenNames.size() && _screenCount > 0)
	{
		ifstream lvlFile(_screenNames[_screenIdx]);
		for (int row = 0; row < MAX_Y; row++) {
			for (int col = 0; col < MAX_X; col++)
			{
				currentBoard[row][col] = lvlFile.get();

				if (currentBoard[row][col] == MARIO)
				{
					_marioX = col;
					_marioY = row;
				}
				else if (currentBoard[row][col] == DONKEYKONG)
				{
					_donkeyX = col;
					_donkeyY = row;
				}
				else if (currentBoard[row][col] == HAMMER)
				{
					_hammerX = col;
					_hammerY = row;
				}
				else if (currentBoard[row][col] == GHOST) {
					Ghost ghost(col, row, GHOST);  // Create a ghost at (col, row)
					ghost.setBoard(*this);  // Link the ghost to the board
					ghosts.push_back(ghost);
				}
				else if (currentBoard[row][col] == X_GHOST) {
					Xghost Xghost(col, row, X_GHOST);  // Create a ghost at (col, row)
					Xghost.setBoard(*this);  // Link the ghost to the board
					Xghosts.push_back(Xghost);
				}
			}
			currentBoard[row][MAX_X] = '\0';
			lvlFile.get();
		}


		return true;
	}
	else
	{
		return false;
	}

}

void Board::loadScreenList() {
	std::filesystem::path folderPath = std::filesystem::current_path(); // Current directory

	for (const auto& file : std::filesystem::directory_iterator(folderPath)) {
		// Check if it's a regular file and has the .screen extension
		if (file.is_regular_file() && file.path().extension() == ".screen") {
			_screenNames.push_back(file.path().filename().string()); // Add to vector
			_screenCount++; // Increment the counter
		}
	}
}

void Board::checkScreen(std::vector<std::string>& screenNames) {
	char boardTester[MAX_Y][MAX_X + 1];

	// Use remove_if to filter out invalid screens
	screenNames.erase(
		std::remove_if(screenNames.begin(), screenNames.end(),
			[&](const std::string& screenName) {
				bool isMario = false;
				bool isDonkey = false;
				bool isPauline = false;

				std::ifstream lvlFile(screenName);
				if (!lvlFile.is_open()) {
					_screenCount--;
					return true; // Remove if the file cannot be opened
				}

				// Check the screen content
				for (int row = 0; row < MAX_Y; row++) {
					for (int col = 0; col < MAX_X; col++) {
						boardTester[row][col] = lvlFile.get();

						if (boardTester[row][col] == MARIO) {
							isMario = true;
						}
						else if (boardTester[row][col] == DONKEYKONG) {
							isDonkey = true;
						}
						else if (boardTester[row][col] == PAULINE) {
							isPauline = true;
						}
					}
					boardTester[row][MAX_X] = '\0';
					lvlFile.get(); // Skip newline
				}

				// A screen is valid if it has Mario, Donkey Kong, and Pauline
				if (!(isDonkey && isMario && isPauline)) {
					_screenCount--; // Decrement counter for invalid screens
					return true; // Remove invalid screen
				}
				else
					return false;
			}),

		screenNames.end()
	);
}

void Board::print() const {
	for (int i = 0; i < MAX_Y - 1; i++) {
		std::cout << currentBoard[i] << '\n';
	}
	std::cout << currentBoard[MAX_Y - 1];
}
