#include "stdafx.h"
#include "Game.h"
#include <string>

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	isGameOver = false;
	isWon = false;
	
	bricks.clear();

	int numberOfBricks = 5;
	int brickWidth = 10;
	int spacing = 2;
	int startX = 11;
	
	// TODO #2 - Add this brick and 4 more bricks to the vector
	for (int i = 0; i < numberOfBricks; i++)
	{
		Box newbrick;
		newbrick.width = brickWidth;
		newbrick.height = 2;
		newbrick.x_position = startX + i * (brickWidth + spacing);
		newbrick.y_position = 5;
		newbrick.doubleThick = true;
		newbrick.color = ConsoleColor::DarkGreen;

		bricks.push_back(newbrick);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (!isGameOver && !isWon)
	{
		if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
			paddle.x_position += 2;

		if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
			paddle.x_position -= 2;

		if (GetAsyncKeyState(VK_SPACE) & 0x1)
			ball.moving = !ball.moving;
	}
	
	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (std::vector<Box>::const_iterator it = bricks.begin(); it != bricks.end(); ++it)
	{
		it->Draw();
	}
	
	//TODO #6 game win trigger text
	if (isWon)
	{
		Console::ForegroundColor(ConsoleColor::Green);
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 14, WINDOW_HEIGHT / 2);
		std::cout << "You Win! Press 'R' to play again.";
	}
	//TODO 7 Game lose trigger text
	else if (isGameOver)
	{
		Console::ForegroundColor(ConsoleColor::Red);
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 15, WINDOW_HEIGHT / 2);
		std::cout << "You lose. Press 'R' to play again.";
	}
	
	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (std::vector<Box>::iterator it = bricks.begin(); it != bricks.end();)
	{
		if (it->Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (it->color == ConsoleColor::DarkGreen)
			{
				it->color = ConsoleColor::DarkGray;
				++it;
			}
			else if (it->color == ConsoleColor::DarkGray)
			{
				it->color = ConsoleColor::Black;
				++it;
			}
			else
			{
				it = bricks.erase(it);
			}
			break;
		}
		else
		{
			++it;
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.empty())
	{
		isWon = true;
		ball.moving = false;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		isGameOver = true;
		ball.moving = false;
	}
}
