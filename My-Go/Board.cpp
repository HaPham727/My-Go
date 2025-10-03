#include "Header.h"

Board::Board() //Initialize the board-recording 2D array 
{
	for (int i{}; i <= HALF_OF_SQUARES * 2; ++i)
	{
		for (int j{}; j <= HALF_OF_SQUARES * 2; ++j)
		{
			this->m_board[i][j] = 0;
		}
	}
}

void Board::printBoard() //Print board to console
{
	for (int i{}; i <= HALF_OF_SQUARES * 2; ++i)
	{
		std::cout << "Row " << std::setw(2) << i + 1 << ": ";

		for (int j{}; j <= HALF_OF_SQUARES * 2; ++j)
		{
			std::cout << std::setw(2) << this->m_board[i][j] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void Board::searchAndRemoveDeadGroup(int& i, int& j, std::vector<std::vector<int>>& checked_pos, std::vector<std::pair<int, int>>& piecesInGroup) //Use BFS from coords i & j to check if group is dead, and destroy, if so
{
	std::queue<std::pair<int, int>> q;
	q.push({ i, j });

	int liberty_count{ 0 };

	piecesInGroup.push_back({ i, j });

	while (!q.empty())
	{
		int queue_size = static_cast<int>(q.size());

		for (int k{}; k < queue_size; k++)
		{
			auto [row, col] = q.front();

			q.pop();

			for (auto& l : DIRECTIONS)
			{
				int new_row = row + l[0];
				int new_col = col + l[1];

				if (new_row >= 0 &&
					new_col >= 0 &&
					new_row < NUMBER_OF_SQUARES &&
					new_col < NUMBER_OF_SQUARES &&
					checked_pos[new_row][new_col] == 0)
				{
					if (this->m_board[new_row][new_col] == 0)
					{
						++liberty_count;
					}
					else if (this->m_board[new_row][new_col] == this->m_board[i][j])
					{
						checked_pos[new_row][new_col] = 2;

						q.push({ new_row, new_col });

						piecesInGroup.push_back({ new_row, new_col });
					}
					//else if (this->m_board[new_row][new_col] == this->m_player * -1)
					//	Code for later
				}
			}
		}
	}

	//Remove dead groups
	if (liberty_count == 0)
	{
		std::cout << "The group of pieces starting from (" << i << ", " << j << ") is DEAD\n";
		for (std::pair<int, int> p : piecesInGroup)
		{
			this->m_board[p.first][p.second] = 0;
		}
	}
}

void Board::checkMovesOrPasses(float& screen_width, float& screen_height)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePos = GetMousePosition();

		//Check for moves
		if (mousePos.x > (screen_width - HALF_OF_SQUARES * 2 * SQUARE_SIZE - SQUARE_SIZE) / 2 &&
			mousePos.x < (screen_width + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE) / 2 &&
			mousePos.y > (screen_height - HALF_OF_SQUARES * 2 * SQUARE_SIZE - SQUARE_SIZE) / 2 &&
			mousePos.y < (screen_height + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE) / 2)
		{
			int row = std::round((mousePos.y - (screen_height - HALF_OF_SQUARES * 2 * SQUARE_SIZE) / 2) / SQUARE_SIZE);
			int col = std::round((mousePos.x - (screen_width - HALF_OF_SQUARES * 2 * SQUARE_SIZE) / 2) / SQUARE_SIZE);

			if (m_board[row][col] == 0)
			{
				this->m_board[row][col] += this->m_player;
				this->m_player *= -1;

				//Start of S&R section

				std::vector<std::vector<int>> checked_pos(NUMBER_OF_SQUARES, std::vector<int>(NUMBER_OF_SQUARES, 0));

				for (int i{}; i < NUMBER_OF_SQUARES; ++i)
				{
					for (int j{}; j < NUMBER_OF_SQUARES; ++j)
					{
						if (this->m_board[i][j] != 0 && checked_pos[i][j] == 0)
						{
							checked_pos[i][j] = 2;

							std::vector<std::pair<int, int>> piecesInGroup;

							searchAndRemoveDeadGroup(i, j, checked_pos, piecesInGroup);
						}
					}
				}

				//End of S&R section

				this->printBoard();
			}
		}

		//Check for passes
		if (mousePos.x >= (screen_width - BUTTON_WIDTH) / 2 &&
			mousePos.x < (screen_width + BUTTON_WIDTH) / 2 &&
			mousePos.y >= (screen_height + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE) / 2 &&
			mousePos.y < (screen_height + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE + BUTTON_HEIGHT * 2) / 2)
		{
			this->m_player *= -1;
		}
	}
}

void Board::drawGrid(float& screen_width, float& screen_height)
{
	for (int i{ -HALF_OF_SQUARES }; i <= HALF_OF_SQUARES; i += 1)
	{
		DrawLineEx({ screen_width / 2 + i * SQUARE_SIZE, screen_height / 2 + HALF_OF_SQUARES * SQUARE_SIZE }, { screen_width / 2 + i * SQUARE_SIZE,  screen_height / 2 - HALF_OF_SQUARES * SQUARE_SIZE }, 3.0, BLACK);
		DrawLineEx({ screen_width / 2 + HALF_OF_SQUARES * SQUARE_SIZE, screen_height / 2 + i * SQUARE_SIZE }, { screen_width / 2 - HALF_OF_SQUARES * SQUARE_SIZE,  screen_height / 2 + i * SQUARE_SIZE }, 3.0, BLACK);
	}
}

void Board::drawPieces(float& screen_width, float& screen_height)
{
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
}

void Board::drawCurrentPlayer(float& screen_width, float& screen_height)
{
	if (m_player > 0)
		DrawTextEx(GetFontDefault(), "Black to play", { (screen_width - TEXT_X_OFFSET) / 2, (screen_height - FEATURES_Y_OFFSET) / 2 }, TEXT_SIZE, FEATURES_SPACING, BLACK);
	else 
		DrawTextEx(GetFontDefault(), "White to play", { (screen_width - TEXT_X_OFFSET) / 2, (screen_height - FEATURES_Y_OFFSET) / 2 }, TEXT_SIZE, FEATURES_SPACING, WHITE);
}

void Board::drawPassButton(float& screen_width, float& screen_height)
{
	DrawRectangle((screen_width - BUTTON_WIDTH) / 2, (screen_height + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE) / 2, BUTTON_WIDTH, BUTTON_HEIGHT, BROWN);
	DrawTextEx(GetFontDefault(), "PASS", { (screen_width - BUTTON_X_OFFSET) / 2, (screen_height + HALF_OF_SQUARES * 2 * SQUARE_SIZE + BUTTON_HEIGHT) / 2 }, TEXT_SIZE, FEATURES_SPACING, BEIGE);
}

void Board::playMove(const int& row, const int& col) //Debug tool (deprecated). Plays move via index 
{
	if (this->m_board[row][col] == 0)
	{
		this->m_board[row][col] = m_player;
		m_player *= -1;
	}
}

void Board::renderGame() //Does all the drawing inside a raylib window loop 
{
	//Get screen width & height for calcs
	float screen_width = static_cast<int>(GetScreenWidth());
	float screen_height = GetScreenHeight();

	//Start Draw
	BeginDrawing();
	ClearBackground(BEIGE);

	//Update Board
	checkMovesOrPasses(screen_width, screen_height);

	//Draw 
	drawGrid(screen_width, screen_height);
	drawPieces(screen_width, screen_height);
	drawCurrentPlayer(screen_width, screen_height);
	drawPassButton(screen_width, screen_height);
	DrawFPS(0, 0);

	//End Draw
	EndDrawing();
}

void Board::loopGame() //Contains the entire game loop
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "My-Go");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		this->renderGame();
	}

	CloseWindow();
}