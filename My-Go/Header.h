#pragma once

//Library 
#include <iostream>
#include <raylib.h>
#include <array>
#include <vector>
#include <queue>
#include <utility>
#include <cmath>
#include <iomanip>

//Constant expressions (set to liking)
constexpr int  NUMBER_OF_SQUARES = 19; //Set number of squares per side of the grid (MUST BE ODD) (19x19 is standard for Go)
constexpr auto SQUARE_SIZE = 42; //Set square size 
constexpr float DEFAULT_SCREEN_WIDTH = 1280.0; //Set default window width
constexpr float DEFAULT_SCREEN_HEIGHT = 960.0; //Set default window height

//Helper consexpr's
constexpr auto HALF_OF_SQUARES = NUMBER_OF_SQUARES / 2; //Half & round down for calculations
constexpr auto PIECE_RADIUS = SQUARE_SIZE * 90 / 200; //Calculate the piece radius for rendering
constexpr std::array<std::array<int, 2>, 4> DIRECTIONS = { {{1, 0}, {-1, 0}, {0, 1}, {0, -1}} }; //Directions to reach adjacent elements

//Forward-declaration
class Board
{
public:
	Board();

	void drawGrid(float& screen_width, float& screen_height);

	void checkPlayedMoves(float& screen_width, float& screen_height);

	void drawPieces(float& screen_width, float& screen_height);

	void searchAndRemoveDeadGroup(int& i, int& j, std::vector<std::vector<int>>& checked_pos, std::vector<std::pair<int, int>>& pieces_in_group);

	void printBoard();

	void playMove(const int& row, const int& col); 

	void renderGame();

	void loopGame();

private:

	std::array <std::array<int, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> m_board;

	int m_player{ 1 }; //Players will be represented via 1 (Black) and -1 (White) so swapping between them is m_player * (-1)
};

