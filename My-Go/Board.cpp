#include "Header.h"

Board::Board() //Constructor for "Board"-class objects to initialize the board-recording 2D array 
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

void Board::searchAndRemoveDeadGroup(int& i, int& j, std::vector<std::vector<int>>& checked_pos, std::vector<std::pair<int, int>>& pieces_in_group)
{
	std::queue<std::pair<int, int>> q;
	q.push({ i, j });

	pieces_in_group.push_back({ i, j });

	int liberty_count{ 0 };

	while (!q.empty())
	{
		int queue_size = q.size();

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

						pieces_in_group.push_back({ new_row, new_col });
					}
					//else if (this->m_board[new_row][new_col] == this->m_player * -1)
					//	
				}
			}
		}
	}

	if (liberty_count == 0)
	{
		std::cout << "The group of pieces starting from (" << i << ", " << j << ") is DEAD\n";
		for (std::pair<int, int> p : pieces_in_group)
		{
			m_board[p.first][p.second] = 0;
		}
	}

	//Clear the entirety of the visited-elements-tracker
	for (int m{}; m < NUMBER_OF_SQUARES; ++m)
	{
		for (int n{}; n < NUMBER_OF_SQUARES; ++n)
		{
			checked_pos[m][n] *= 0;
		}
	}
}

void Board::checkPlayedMoves(float& screen_width, float& screen_height)
{
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
				m_board[row][col] += m_player;
				m_player *= -1;

				//Start of S&R section

				std::vector<std::vector<int>> checked_pos(NUMBER_OF_SQUARES, std::vector<int>(NUMBER_OF_SQUARES, 0));

				for (int i{}; i < NUMBER_OF_SQUARES; ++i)
				{
					for (int j{}; j < NUMBER_OF_SQUARES; ++j)
					{
						if (this->m_board[i][j] != 0 && checked_pos[i][j] == 0)
						{
							checked_pos[i][j] = 2;

							//This vector "pieces_in_group" is created every time a new (meaning unvisited, meaning checked_pos value = 0) group is found and (therefore) needs to get BFS'd
							std::vector<std::pair<int, int>> pieces_in_group;

							searchAndRemoveDeadGroup(i, j, checked_pos, pieces_in_group);
						}
					}
				}

				//End of S&R section

				this->printBoard();
			}
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

void Board::playMove(const int& row, const int& col) //Debug tool (deprecated). Plays move via index (starts from 0)
{
	if (this->m_board[row][col] == 0)
	{
		this->m_board[row][col] = m_player;
		m_player *= -1;
	}
}

void Board::renderGame() //Does all the drawing needed inside a raylib window loop (BeginDrawing, ClearBackGround, Game Loop, and EndDrawing)
{
	//Constantly check for screen width & height for use in calculations
	float screen_width = GetScreenWidth();
	float screen_height = GetScreenHeight();

	//Start Drawing
	BeginDrawing();

	//Clear the Background
	ClearBackground(BEIGE);

	//Update Board
	checkPlayedMoves(screen_width, screen_height);

	//Draw 
	drawGrid(screen_width, screen_height);
	drawPieces(screen_width, screen_height);
	DrawFPS(0, 0);

	//End Drawing
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