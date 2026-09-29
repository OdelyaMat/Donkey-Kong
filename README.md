# Donkey Kong - C++ Console Edition

A feature-rich recreation of the classic **Donkey Kong** arcade game, built in C++ for the Windows Console. This project demonstrates Object-Oriented Programming (OOP) principles, inheritance, and a unique game recording/validation system.

## Gameplay Overview
Navigate Mario through challenging levels, climb ladders, dodge rolling barrels, and outsmart various enemies to save Pauline.

### Key Features
* **Dynamic Level System**: Automatically scans and loads levels from external `.screen` files.
* **Advanced AI Enemies**:
    * **Barrels (O)**: Follow gravity and floor slopes, exploding upon impact with walls or after falling from significant heights.
    * **Ghosts (x)**: Move horizontally on floors, changing direction based on probability or obstacles.
    * **X-Ghosts (X)**: Smart enemies capable of climbing ladders and navigating the board vertically.
* **Combat & Scoring**: Pick up the **Hammer (p)** to destroy ghosts and barrels for extra points.
* **Physics Engine**: Realistic implementation of gravity, falling damage (falling from height kills Mario), and ladder mechanics.

## Project Structure
* **Core Logic**: `Game`, `Board`, `Mario`.
* **Enemy Hierarchy**: Utilizes a base `Enemy` class with specialized logic for `Ghost`, `Xghost`, and `Barrel`.
* **Recording System**: `Steps` and `Results` classes manage the recording and playback of game sessions.
* **Utilities**: Custom `utilits` for Windows-specific console manipulation (e.g., `gotoxy`, cursor control).

## Execution Modes
The engine supports three modes via command-line arguments:

| Mode       | Argument        | Description                                                         |
| :---       | :---            | :---                                                                |
| **Play**   | *None*          | Standard interactive gameplay.                                      |
| **Save**   | `-save`         | Records all moves to `.steps` and results to `.result` files.       |
| **Load**   | `-load`         | Replays a recorded game from existing files.                        |
| **Silent** | `-load -silent` | Runs the game at max speed without UI to verify result consistency. |

## Controls
/ X / A / D: Move Up / Down / Left / Right.
S: Stay in place.
P: Use hammer (if active).
ESC: Return to menu or pause.
