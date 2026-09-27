# 🎮 Othello Game

A console-based implementation of the classic Othello game written in C++.

The game supports both single-player and two-player modes, different board sizes, saving and loading games, and game history.

## ✨ Features

- Single-player mode against a robot
- Two-player mode
- Two difficulty levels for the robot:
  - Simple
  - Advanced
- Three board sizes:
  - 4×4
  - 6×6
  - 8×8
- Automatic detection of valid moves
- Automatic flipping of opponent pieces
- Save and load unfinished games
- Game history
- Automatic score calculation
- Winner detection
- Console colors and Unicode symbols
- Dynamic memory management for game history

## 🎯 Game Modes

### Single Player

Play against the computer.

Two difficulty levels are available:

- Simple: The robot chooses a random valid move.
- Advanced: The robot chooses the move that flips the highest number of opponent pieces.

### Two Player

Two players can play against each other on the same computer.

## 🎮 Controls

| Key | Action |
|-----|--------|
| W | Move Up |
| A | Move Left |
| S | Move Down |
| D | Move Right |
| Enter | Place a piece |
| Q | Save the current game and return to the menu |

## 🔹 Board Symbols

| Symbol | Meaning |
|--------|---------|
| ○ | Black piece |
| ● | White piece |
| □ | Empty cell |
| * | Possible move |

## 📋 Main Menu

The main menu contains:

1. New Game
2. Load Game
3. Help
4. Game History
5. Exit

## 💾 Save & Load

An unfinished game can be saved by pressing Q during the game.

The current game information is stored in:

loadGame.txt

The saved game can later be continued using the Load Game option from the main menu.

## 📜 Game History

After a game is finished, information about the game is stored in:

gameHistory.txt

The history includes:

- Player names
- Final scores
- Winner
- Date and time of the game

Games are displayed from newest to oldest.

## 🧠 Game Rules

- Players take turns placing their pieces on the board.
- A move is valid only if at least one opponent piece can be flipped.
- Opponent pieces are flipped when they are trapped between two pieces of the current player.
- If a player has no valid move, their turn is automatically passed.
- The game ends when neither player has a valid move.
- The player with the most pieces wins the game.


## ▶️ How to Run

This project is designed for Windows because it uses:
```cpp
#include <windows.h>
#include <conio.h>
```
Compile the program using a C++ compiler such as MinGW / g++:

g++ miniProject.cpp -o Othello


Then run:

Othello.exe


## 📁 Project Structure
```text
Othello/
│
├── miniProject.cpp
├── gameHistory.txt
├── loadGame.txt
└── README.md
```
gameHistory.txt and loadGame.txt are used by the program for storing game information.

## 📌 Notes
The game uses Unicode characters for displaying the board.
UTF-8 console output is enabled for correct symbol display.
Saved games can be continued before starting another game.
The game history storage automatically expands when more space is needed.