#pragma once

//Library 
#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <utility>
#include <cmath>
#include <iomanip>
#include <raylib.h>

//Constant expressions (Set to liking)
constexpr int  NUMBER_OF_SQUARES = 9; //Set board size (MUST BE ODD) (19x19 is standard for Go)
constexpr auto SQUARE_SIZE = 40; //Set square size (Suggested: 40 for 19x19)
constexpr float DEFAULT_SCREEN_WIDTH = 1280.0; //Set default window width
constexpr float DEFAULT_SCREEN_HEIGHT = 960.0; //Set default window height


//Helper constant expressions (Do not change)
constexpr std::array<std::array<int, 2>, 4> DIRECTIONS = { { {1, 0}, {-1, 0}, {0, 1}, {0, -1} } }; //Directions to reach adjacent elements

constexpr auto HALF_OF_SQUARES = NUMBER_OF_SQUARES / 2; //Half & round down for calculations
constexpr auto PIECE_RADIUS = SQUARE_SIZE * 9 / 20; //Calculate the piece radius for rendering

constexpr auto FEATURES_Y_OFFSET = (NUMBER_OF_SQUARES + 2) * SQUARE_SIZE; //Y offset for player display text and pass button
constexpr float FEATURES_SPACING = 7; //Spacing for player display & button texts

constexpr float TEXT_X_OFFSET = 318; 
constexpr float TEXT_SIZE = 42; 

constexpr float BUTTON_X_OFFSET = 126;
constexpr float BUTTON_WIDTH = 400; 
constexpr float BUTTON_HEIGHT = 50;


//Forward-declaration
class Board
{
public:
	Board();

	void printBoard();

	void searchAndRemoveDeadGroup(int& i, int& j, std::vector<std::vector<int>>& checked_pos, std::vector<std::pair<int, int>>& pieces_in_group);

	void checkMovesOrPasses(float& screen_width, float& screen_height);

	void drawGrid(float& screen_width, float& screen_height);

	void drawPieces(float& screen_width, float& screen_height);

	void drawCurrentPlayer(float& screen_width, float& screen_height);

	void drawPassButton(float& screen_width, float& screen_height);

	void playMove(const int& row, const int& col); 

	void renderGame();

	void loopGame();

private:

	std::array <std::array<int, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> m_board;

	int m_player{ 1 }; //Black is 1 and White is -1
};

