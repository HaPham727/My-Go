#include "Header.h"

Board::Board() //Constructor for "Board"-class objects which has a 2D array to hold the board 
{
	for (int i{}; i <= HALF_OF_SQUARES * 2; ++i)
	{
		for (int j{}; j <= HALF_OF_SQUARES * 2; ++j)
		{
			this->m_board[i][j] = 0;
		}
	}
}

void Board::drawBoard() //Does all the drawing needed inside a raylib window loop (BeginDrawing, ClearBackGround, Game Loop, and EndDrawing)
{
	//Start Drawing
	BeginDrawing();

	//Clear the Background
	ClearBackground(BEIGE);

	//Constantly check for screen width & height for calculations
	float screen_width = GetScreenWidth();
	float screen_height = GetScreenHeight();

	//Draw the Grid
	for (int i{ -HALF_OF_SQUARES }; i <= HALF_OF_SQUARES; i += 1)
	{
		DrawLineEx({ screen_width / 2 + i * SQUARE_SIZE, screen_height / 2 + HALF_OF_SQUARES * SQUARE_SIZE }, { screen_width / 2 + i * SQUARE_SIZE,  screen_height / 2 - HALF_OF_SQUARES * SQUARE_SIZE }, 3.0, BLACK);
		DrawLineEx({ screen_width / 2 + HALF_OF_SQUARES * SQUARE_SIZE, screen_height / 2 + i * SQUARE_SIZE }, { screen_width / 2 - HALF_OF_SQUARES * SQUARE_SIZE,  screen_height / 2 + i * SQUARE_SIZE }, 3.0, BLACK);
	}

	//Check for any moves played
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
	{
		Vector2 mousePos = GetMousePosition();

		if (mousePos.x >= (screen_width - HALF_OF_SQUARES * 2 * SQUARE_SIZE - SQUARE_SIZE) / 2 &&
			mousePos.x < (screen_width + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE) / 2 &&
			mousePos.y >= (screen_height - HALF_OF_SQUARES * 2 * SQUARE_SIZE - SQUARE_SIZE) / 2 &&
			mousePos.y < (screen_height + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE) / 2)
		{
			int row = std::round((mousePos.y - (screen_height - HALF_OF_SQUARES * 2 * SQUARE_SIZE) / 2) / SQUARE_SIZE);
			int col = std::round((mousePos.x - (screen_width - HALF_OF_SQUARES * 2 * SQUARE_SIZE) / 2) / SQUARE_SIZE);

			if (m_board[row][col] == 0)
			{
				m_board[row][col] = m_player;
				m_player *= -1;
			}
		}
		this->printBoard();
	}

	//Draw the pieces
	for (int i{ -HALF_OF_SQUARES }; i <= HALF_OF_SQUARES; ++i)
	{
		for (int j{ -HALF_OF_SQUARES }; j <= HALF_OF_SQUARES; ++j)
		{
			switch (this->m_board[i + HALF_OF_SQUARES][j + HALF_OF_SQUARES])
			{
			case 1:
				DrawCircleV({ screen_width / 2 + j * SQUARE_SIZE, screen_height / 2 + i * SQUARE_SIZE }, PIECE_RADIUS, BLACK);
				continue;
			case -1:
				DrawCircleV({ screen_width / 2 + j * SQUARE_SIZE, screen_height / 2 + i * SQUARE_SIZE }, PIECE_RADIUS, WHITE);
				continue;
			default:
				continue;
			}
		}
	}

	//Show the FPS count
	DrawFPS(0, 0);

	//End Drawing
	EndDrawing();
}

void Board::loopGame(const float& default_screen_width, const float& default_screen_height) //Contains the game loop
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(default_screen_width, default_screen_height, "Go Game!");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		this->drawBoard();
	}
	CloseWindow();
}

void Board::printBoard() //Print board to console
{
	for (int i{}; i <= HALF_OF_SQUARES * 2; ++i)
	{
		if (i < 9)
			std::cout << "Row 0" << i + 1 << " : ";
		else
			std::cout << "Row " << i + 1 << " : ";

		for (int j{}; j <= HALF_OF_SQUARES * 2; ++j)
		{
			std::cout << this->m_board[i][j] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void Board::playMove(const int& row, const int& col) //Play move via index (starts from 0)
{
	if (this->m_board[row][col] == 0)
	{
		this->m_board[row][col] = m_player;
		m_player *= -1;
	}
}