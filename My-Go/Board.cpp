#include "Header.h"

int screen_width = 1280; //Default window width
int screen_height = 960; //Default window height

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

void Board::checkMovesOrPasses()
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
			int playedRow = std::round((mousePos.y - (static_cast<float>(screen_height) - HALF_OF_SQUARES * 2 * SQUARE_SIZE) / 2) / SQUARE_SIZE);
			int playedCol = std::round((mousePos.x - (static_cast<float>(screen_width) - HALF_OF_SQUARES * 2 * SQUARE_SIZE) / 2) / SQUARE_SIZE);

			if (m_board[playedRow][playedCol] == 0)
			{
				this->m_board[playedRow][playedCol] += this->m_player;

				//std::cout << "Played Row " << playedRow << " and Col " << playedCol << '\n';

				bool playedPieceIsSafe{ false };

				//BFSearch & Remove dead pieces (if any) for the player who did NOT play the last move
				for (int i{ 0 }; i < std::ssize(DIRECTIONS) - 1; ++i)
				{
					int row = playedRow + DIRECTIONS[i][0];
					int col = playedCol + DIRECTIONS[i][1];

					if (row >= 0 &&
						col >= 0 &&
						row < NUMBER_OF_SQUARES &&
						col < NUMBER_OF_SQUARES &&
						m_board[row][col] == this->m_player * (-1))
					{
						bool hasLiberty{ false };

						std::array<std::array<int, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> timesChecked{};

						std::queue<std::pair<int, int>> toBeSearched;
						toBeSearched.push({ row, col });

						//Check if the group starting from [row][col] has liberty (is not dead) or nah
						while (!toBeSearched.empty())
						{
							int queue_size = static_cast<int>(std::ssize(toBeSearched));

							for (int j{ 0 }; j < queue_size; ++j)
							{
								auto& [rowNew, colNew] = toBeSearched.front();

								toBeSearched.pop();

								for (int k{ 0 }; k < std::ssize(DIRECTIONS) - 1; ++k)
								{
									int rowBFS = rowNew + DIRECTIONS[k][0];
									int colBFS = colNew + DIRECTIONS[k][1];

									if (rowBFS >= 0 &&
										colBFS >= 0 &&
										rowBFS < NUMBER_OF_SQUARES &&
										colBFS < NUMBER_OF_SQUARES)
									{
										if (this->m_board[rowBFS][colBFS] == 0)
										{
											hasLiberty = true;

											goto lib_check_1;
										}
										else if (this->m_board[rowBFS][colBFS] == this->m_board[rowNew][colNew] && timesChecked[rowBFS][colBFS] <= TIMES_CHECKED_PERMITTED)
										{
											++timesChecked[rowBFS][colBFS];

											toBeSearched.push({ rowBFS, colBFS });
										}
									}
								}
							}
						}

						//Debug namespace
						{
						std::cout << "This is how the timesChecked array look: \n";
						for (int i{}; i <= HALF_OF_SQUARES * 2; ++i)
						{
							std::cout << "Row " << std::setw(2) << i + 1 << ": ";

							for (int j{}; j <= HALF_OF_SQUARES * 2; ++j)
							{
								std::cout << std::setw(2) << timesChecked[i][j] << ' ';
							}
							std::cout << '\n';
						}
						std::cout << '\n';
						}

						//Remove dead groups if the group starting from [row][col] has no liberty
						if (!hasLiberty)
						{
							std::cout << "The group of pieces starting from (" << row << ", " << col << ") is DEAD\n" << '\n';

							for (int m{}; m < NUMBER_OF_SQUARES; ++m)
							{
								for (int n{}; n < NUMBER_OF_SQUARES; ++n)
								{
									if (timesChecked[m][n] > 0)
									{
										this->m_board[m][n] = 0;
									}
								}
							}

							playedPieceIsSafe = true;
						}


					}

				lib_check_1:
					continue;
				}

				//Havent fixed lower branch yet

				//Reverse move (if need) by searching the group of the played piece				
				if (!playedPieceIsSafe)
				{
					bool hasLiberty{ false };

					int row = playedRow;
					int col = playedCol;

					std::array<std::array<int, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> timesChecked{};

					std::queue<std::pair<int, int>> toBeSearched;
					toBeSearched.push({ row, col });

					//Check if the group starting from [row][col] has liberty (is not dead) or nah
					while (!toBeSearched.empty())
					{
						int queue_size = static_cast<int>(std::ssize(toBeSearched));

						for (int j{ 0 }; j < queue_size; ++j)
						{
							auto [rowNew, colNew] = toBeSearched.front();

							toBeSearched.pop();

							for (int k{ 0 }; k < std::ssize(DIRECTIONS) - 1; ++k)
							{
								int rowBFS = rowNew + DIRECTIONS[k][0];
								int colBFS = colNew + DIRECTIONS[k][1];

								if (rowBFS >= 0 &&
									colBFS >= 0 &&
									rowBFS < NUMBER_OF_SQUARES &&
									colBFS < NUMBER_OF_SQUARES)
								{
									if (this->m_board[rowBFS][colBFS] == 0)
									{
										hasLiberty = true;

										goto lib_check_2;
									}
									else if (this->m_board[rowBFS][colBFS] == this->m_player && 
											 timesChecked[rowBFS][colBFS] == 0)
									{
										++timesChecked[rowBFS][colBFS];

										toBeSearched.push({ rowBFS, colBFS });
									}
								}
							}
						}
					}

					//Detect & reverse illegal moves 
					if (!hasLiberty && this->m_board[playedRow][playedCol] == this->m_player)
					{
						this->m_board[playedRow][playedCol] = 0;
						this->m_player *= -1;
					}

				lib_check_2:
					;
				}

				this->m_player *= -1;

				this->m_passes = 0;

				this->printBoard();
			}
		}

		//Check for passes
		else if (mousePos.x >= (screen_width - BUTTON_WIDTH) / 2 &&
				 mousePos.x < (screen_width + BUTTON_WIDTH) / 2 &&
				 mousePos.y >= (screen_height + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE) / 2 &&
				 mousePos.y < (screen_height + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE + BUTTON_HEIGHT * 2) / 2)
		{
				this->m_player *= -1;
				++this->m_passes;
		}
	}

	if (IsKeyPressed(KEY_SPACE))
	{
		this->m_player *= -1;
		++this->m_passes;
	}

	//Debug tool
	if (IsKeyPressed(KEY_Q))
	{
		for (int i{0}; i < NUMBER_OF_SQUARES - 3; ++i)
		{
			for (int j{}; j < NUMBER_OF_SQUARES; ++j)
			{
				this->m_board[i][j] = -1;

				if (i == NUMBER_OF_SQUARES - 4 && j < NUMBER_OF_SQUARES-1)
				{
					this->m_board[NUMBER_OF_SQUARES - 4][j] = 1;
				}
			}
		}
	}
}

void Board::drawGrid()
{
	for (int i{-HALF_OF_SQUARES}; i <= HALF_OF_SQUARES; i += 1)
	{
		DrawLineEx( {static_cast<float>(screen_width) / 2 + i * SQUARE_SIZE, static_cast<float>(screen_height) / 2 + HALF_OF_SQUARES * SQUARE_SIZE }, { static_cast<float>(screen_width) / 2 + i * SQUARE_SIZE,  static_cast<float>(screen_height) / 2 - HALF_OF_SQUARES * SQUARE_SIZE }, LINE_THICKNESS, BLACK);
		DrawLineEx( {static_cast<float>(screen_width) / 2 + HALF_OF_SQUARES * SQUARE_SIZE, static_cast<float>(screen_height) / 2 + i * SQUARE_SIZE }, { static_cast<float>(screen_width) / 2 - HALF_OF_SQUARES * SQUARE_SIZE,  static_cast<float>(screen_height) / 2 + i * SQUARE_SIZE }, LINE_THICKNESS, BLACK);
	}

	//Draw the dots
	switch (NUMBER_OF_SQUARES)
	{
	case 19:
		for (int i{-1}; i <= 1; ++i)
		{
			for (int j{-1}; j <= 1 ; ++j)
			{
				DrawCircleV({ static_cast<float>(screen_width) / 2 + j * 6 * SQUARE_SIZE, static_cast<float>(screen_height) / 2 + i * 6 * SQUARE_SIZE }, DOT_RADIUS, BLACK);
			}
		}
		return;
	case 13:
		for (int i{ -1 }; i <= 1; ++i)
		{
			for (int j{ -1 }; j <= 1; ++j)
			{
				DrawCircleV({ static_cast<float>(screen_width) / 2 + j * 3 * SQUARE_SIZE, static_cast<float>(screen_height) / 2 + i * 3 * SQUARE_SIZE }, DOT_RADIUS, BLACK);
			}
		}
		return;
	case 9:
		for (int i{ -1 }; i <= 1; i += 2)
		{
			for (int j{ -1 }; j <= 1; j += 2)
			{
				DrawCircleV({ static_cast<float>(screen_width) / 2 + j * 2 * SQUARE_SIZE, static_cast<float>(screen_height) / 2 + i * 2 * SQUARE_SIZE }, DOT_RADIUS, BLACK);
			}
		}
		return;
	default:
		return;
	}
}

void Board::drawPieces()
{
	for (int i{ -HALF_OF_SQUARES }; i <= HALF_OF_SQUARES; ++i)
	{
		for (int j{ -HALF_OF_SQUARES }; j <= HALF_OF_SQUARES; ++j)
		{
			switch (this->m_board[i + HALF_OF_SQUARES][j + HALF_OF_SQUARES])
			{
			case 1:
				DrawCircleV({ static_cast<float>(screen_width) / 2 + j * SQUARE_SIZE, static_cast<float>(screen_height) / 2 + i * SQUARE_SIZE }, PIECE_RADIUS, BLACK);
				continue;
			case -1:
				DrawCircleV({ static_cast<float>(screen_width) / 2 + j * SQUARE_SIZE, static_cast<float>(screen_height) / 2 + i * SQUARE_SIZE }, PIECE_RADIUS, WHITE);
				continue;
			default:
				continue;
			}
		}
	}
}

void Board::drawCurrentPlayer()
{
	if (m_player > 0)
		DrawTextEx(GetFontDefault(), "Black to play", { (static_cast<float>(screen_width) - TEXT_X_OFFSET) / 2, (static_cast<float>(screen_height) - FEATURES_Y_OFFSET) / 2 }, BASE_TEXT_SIZE, FEATURES_SPACING, BLACK);
	else 
		DrawTextEx(GetFontDefault(), "White to play", { (static_cast<float>(screen_width) - TEXT_X_OFFSET) / 2, (static_cast<float>(screen_height) - FEATURES_Y_OFFSET) / 2 }, BASE_TEXT_SIZE, FEATURES_SPACING, WHITE);
}

void Board::drawPassButton()
{
	DrawRectangle((screen_width - BUTTON_WIDTH) / 2, (screen_height + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE) / 2, BUTTON_WIDTH, BUTTON_HEIGHT, BROWN);
	DrawTextEx(GetFontDefault(), "PASS", { (static_cast<float>(screen_width) - BUTTON_TEXT_X_OFFSET) / 2, (static_cast<float>(screen_height) + HALF_OF_SQUARES * 2 * SQUARE_SIZE + SQUARE_SIZE + BUTTON_TEXT_Y_OFFSET) / 2 }, BASE_TEXT_SIZE, FEATURES_SPACING, BEIGE);
}

void Board::evaluateScore()
{
	if (!m_scoreEvaluated)
	{
		std::array<std::array<int, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> timesChecked{};

		std::queue<std::pair<int, int>> toBeSearched;

		//Iterate through Board to check if territories are fully controlled (non-neutral) by one player
		for (int row{0}; row < NUMBER_OF_SQUARES; ++row)
		{
			for (int col{0}; col < NUMBER_OF_SQUARES; ++col)
			{
				if (this->m_board[row][col] == 0 && timesChecked[row][col] == 0)
				{
					bool touchesBlack{ false };
					bool touchesWhite{ false };

					++timesChecked[row][col];

					std::vector<std::pair<int, int>> piecesInGroup;
					piecesInGroup.push_back({ row, col });

					toBeSearched.push({ row, col });

					while (!toBeSearched.empty())
					{
						auto [rowNew, colNew] = toBeSearched.front();

						toBeSearched.pop();

						for (int i{0}; i < std::ssize(DIRECTIONS) - 1; ++i)
						{
							int rowBFS = rowNew + DIRECTIONS[i][0];
							int colBFS = colNew + DIRECTIONS[i][1];

							if (rowBFS >= 0 &&
								colBFS >= 0 &&
								rowBFS < NUMBER_OF_SQUARES &&
								colBFS < NUMBER_OF_SQUARES && 
								timesChecked[rowBFS][colBFS] == 0 )
							{
								if (this->m_board[rowBFS][colBFS] == 1)
								{
									touchesBlack = true;
								}
								else if (this->m_board[rowBFS][colBFS] == -1)
								{
									touchesWhite = true;
								}
								else if (this->m_board[rowBFS][colBFS] == 0)
								{
									++timesChecked[rowBFS][colBFS];

									piecesInGroup.push_back({ rowBFS, colBFS });

									toBeSearched.push({ rowBFS, colBFS });
								}
							}
						}
					}

					//If territory starting from [row][col] is fully controlled by Black
					if (touchesBlack && !touchesWhite)
					{
						std::array<std::array<int, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> cleared{};

						toBeSearched.push({ row, col });

						this->m_board[row][col] = 0;

						while (!toBeSearched.empty())
						{
							int queue_size = static_cast<int>(toBeSearched.size());

							for (int j{ 0 }; j < queue_size; ++j)
							{
								auto& [rowNew, colNew] = toBeSearched.front();

								toBeSearched.pop();

								for (int k{ 0 }; k < std::ssize(DIRECTIONS) - 1; ++k)
								{
									int rowBFS = rowNew + DIRECTIONS[k][0];
									int colBFS = colNew + DIRECTIONS[k][1];

									if (rowBFS >= 0 &&
										colBFS >= 0 &&
										rowBFS < NUMBER_OF_SQUARES &&
										colBFS < NUMBER_OF_SQUARES &&
										this->m_board[rowBFS][colBFS] == 0 &&
										cleared[rowBFS][colBFS] == 0)
									{
										++this->m_score;

										++cleared[rowBFS][colBFS];

										toBeSearched.push({ rowBFS, colBFS });

									}
								}
							}
						}
					}

					//If territory starting from [row][col] is fully controlled by White
					else if (!touchesBlack && touchesWhite)
					{
						std::array<std::array<int, NUMBER_OF_SQUARES>, NUMBER_OF_SQUARES> cleared{};

						toBeSearched.push({ row, col });

						this->m_board[row][col] = 0;

						while (!toBeSearched.empty())
						{
							int queue_size = static_cast<int>(toBeSearched.size());

							for (int j{ 0 }; j < queue_size; ++j)
							{
								auto& [rowNew, colNew] = toBeSearched.front();

								toBeSearched.pop();

								for (int k{ 0 }; k < std::ssize(DIRECTIONS) - 1; ++k)
								{
									int rowBFS = rowNew + DIRECTIONS[k][0];
									int colBFS = colNew + DIRECTIONS[k][1];

									if (rowBFS >= 0 &&
										colBFS >= 0 &&
										rowBFS < NUMBER_OF_SQUARES &&
										colBFS < NUMBER_OF_SQUARES &&
										this->m_board[rowBFS][colBFS] == 0 &&
										cleared[rowBFS][colBFS] == 0)
									{
										--this->m_score;

										++cleared[rowBFS][colBFS];

										toBeSearched.push({ rowBFS, colBFS });

									}
								}
							}
						}
					}
				}
			}
		}

		std::cout << ((m_score > 0) ? "Black" : "White") << " wins by " << ((m_score > 0) ? m_score : -m_score) << " points!\n";

		m_scoreEvaluated = true;
	}
}

void Board::checkPlayAgainOrExit()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePos = GetMousePosition();

		//Check for clicks on "Play Again"
		if (mousePos.x > (static_cast<float>(screen_width) - PA_BUTTON_WIDTH) / 2 &&
			mousePos.x < (static_cast<float>(screen_width) + PA_BUTTON_WIDTH) / 2 &&
			mousePos.y > (static_cast<float>(screen_height) + PA_FEATURES_Y_OFFSET) / 2 &&
			mousePos.y < (static_cast<float>(screen_height) + PA_FEATURES_Y_OFFSET + PA_BUTTON_HEIGHT * 2) / 2)
		{
			for (int i{}; i < NUMBER_OF_SQUARES; ++i)
			{
				for (int j{}; j < NUMBER_OF_SQUARES; ++j)
				{
					this->m_board[i][j] = 0;
				}
			}

			//Reset the player, score, pass counter, and m_scoreEvaluated bool
			this->m_player = 1;
			this->m_passes = 0;
			this->m_score = -KOMI ;
			this->m_scoreEvaluated = false;
		}
	}
}

void Board::drawGameEndPopup()
{
	//DrawRectangle((screen_width - PA_POPUP_WIDTH) / 2, (screen_height - PA_POPUP_HEIGHT) / 2, PA_POPUP_WIDTH, PA_POPUP_HEIGHT, BROWN);

	DrawRectangle((screen_width - PA_BUTTON_WIDTH) / 2, (screen_height + PA_FEATURES_Y_OFFSET) / 2, PA_BUTTON_WIDTH, PA_BUTTON_HEIGHT, DARKBROWN);

	DrawTextEx(GetFontDefault(), (this->m_score > 0) ? ("Black Wins") : ("White Wins"), { (static_cast<float>(screen_width) - PA_TEXT_X_OFFSET) / 2, (static_cast<float>(screen_height) - PA_FEATURES_Y_OFFSET - PA_BUTTON_HEIGHT) / 2 }, BASE_TEXT_SIZE * 2, FEATURES_SPACING, ((this->m_score > 0) ? BLACK : WHITE));

	DrawTextEx(GetFontDefault(), "Play Again", { (static_cast<float>(screen_width) - PA_BUTTON_TEXT_X_OFFSET) / 2, (static_cast<float>(screen_height) + PA_BUTTON_TEXT_Y_OFFSET) / 2 }, BASE_TEXT_SIZE, FEATURES_SPACING, BEIGE);
}

void Board::renderGame() //Does all the drawing inside a raylib window loop 
{
	//Update width & height if the user resize window
	screen_width = GetScreenWidth();
	screen_height = GetScreenHeight();

	//Start Draw
	BeginDrawing();
	ClearBackground(BEIGE);

	if (this->m_passes <= 1)
	{
		//Detect moves
		checkMovesOrPasses();

		//If condition to speed up transition between Board and Play Again popup after the 2nd continuous PASS is pressed
		if (this->m_passes == 2)
		{
			//evaluateScore();
			goto jump_point;
		}

		//Draw Board
		drawGrid();
		drawPieces();
		drawCurrentPlayer();
		drawPassButton();
	}
	else if (this->m_passes == 2)
	{
	jump_point:

		//Evaluate score (once after every game)
		evaluateScore();

		//Draw Play Again Popup
		drawGameEndPopup();

		//Detect clicks on Play Again
		checkPlayAgainOrExit();
	}

	DrawFPS(0, 0);

	//End Draw
	EndDrawing();
}

void Board::loopGame() //Contains the entire game loop
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(1280, 960, "My-Go");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		this->renderGame();
	}

	CloseWindow();
}