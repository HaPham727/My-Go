#ifndef HEADER
#define HEADER

//Library 
#include <array>
#include <queue>
#include <cmath>
#include <raylib.h>



//Constant expressions (Set to liking)
constexpr int  NUMBER_OF_SQUARES = 19; //Set board size (MUST BE ODD) (19x19 is standard for Go)
constexpr int SQUARE_SIZE = 40; //Set square size (Suggested: 40 for 19x19)
constexpr float KOMI = 7.5; //Set komi (7.5 is standard under Chinese rules)



//Helper constant expressions (Do not change)
constexpr std::array<std::array<int, 2>, 5> DIRECTIONS = { { {-1, 0}, {0, -1}, {1, 0}, {0, 1}, {0, 0} } }; //Directions to reach adjacent elements

constexpr int HALF_OF_SQUARES = NUMBER_OF_SQUARES / 2; //Half & round down for calculations
constexpr int PIECE_RADIUS = SQUARE_SIZE * 9 / 20; //Calculate the piece radius for rendering
constexpr int DOT_RADIUS = SQUARE_SIZE / 8; //Calculate the piece radius for rendering
constexpr float LINE_THICKNESS = 3.0; //Thickness of lines on the board

constexpr int FEATURES_Y_OFFSET = (NUMBER_OF_SQUARES + 2) * SQUARE_SIZE; 
constexpr int FEATURES_SPACING = 7;
constexpr int TEXT_X_OFFSET = 318; 
constexpr int BASE_TEXT_SIZE = 42; 
constexpr int BUTTON_TEXT_X_OFFSET = 126;
constexpr int BUTTON_TEXT_Y_OFFSET = 10;
constexpr int BUTTON_WIDTH = 400; 
constexpr int BUTTON_HEIGHT = 50;

constexpr int PA_POPUP_WIDTH = 710;
constexpr int PA_POPUP_HEIGHT = 400;
constexpr int PA_FEATURES_Y_OFFSET = 100;
constexpr int PA_TEXT_X_OFFSET = 430;
constexpr int PA_BUTTON_WIDTH = 325;
constexpr int PA_BUTTON_HEIGHT = 75;
constexpr int PA_BUTTON_TEXT_X_OFFSET = 250;
constexpr int PA_BUTTON_TEXT_Y_OFFSET = 134;



//Forward-declaration
class Board
{
public:
	Board();

	void checkMovesOrPasses();

	void drawGrid();

	void drawPieces();

	void drawCurrentPlayer();

	void drawPassButton();

	void evaluateScore();

	void checkPlayAgainOrExit();

	void drawGameEndPopup();

	void renderGame();

	void loopGame();

private:
	std::array <std::array<int, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> m_board;

	int m_player{ 1 }; //Black is 1 and White is -1

	int m_passes{ 0 }; //Counts how many passes since the last move was played. 2 continuous passes = game end

	float m_score{ -KOMI }; //Positive scores = Black wins, Negative scores = White wins

	bool m_scoreEvaluated{ false }; //Bool to make sure score is only evaluated once at end of game
};

#endif