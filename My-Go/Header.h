#pragma once

//Library 
#include <iostream>
#include <raylib.h>
#include <array>
#include <cmath>

//Constant expressions
constexpr auto SQUARE_SIZE = 42; //Set square size 
constexpr auto HALF_OF_SQUARES = 9; //How many squares are in 1/2 of a grid side (17x17 would have 8)
constexpr auto PIECE_RADIUS = 18; //Set piece radius

constexpr float default_screen_width = 1280.0; //Set default window width
constexpr float default_screen_height = 1000.0; //Set default window height

//Forward-declaration
class Board
{
public:
	Board();

	void drawBoard();

	void loopGame(const float& default_screen_width, const float& default_screen_height);

	void printBoard();

	void playMove(const int& row, const int& col); 

private:
	std::array <std::array<int, 19>, 19> m_board;

	int m_player{ 1 }; //Players will be represented via 1 (Black) and -1 (White) so swapping between them is m_player * (-1)
};

