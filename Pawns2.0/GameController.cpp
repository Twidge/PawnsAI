#include "GameController.h"
#include "Board.h"
#include "Player.h"

// CONSTRUCTORS

GameController::GameController()
{
	m_gameBoard = new Board();
}

GameController::GameController(Board const& board)
{
	m_gameBoard = new Board(board);
}

// DESTRUCTOR

GameController::~GameController()
{
	delete m_gameBoard;
}

// METHODS

bool GameController::RunGame(Player const& playerOne, Player const& playerTwo)
{
	if (!m_gameBoard->MovesPopulated())
	{
		m_gameBoard->PopulateMoves();
	}

	m_gameBoard->ResetBoard();
	m_gameBoard->ResetGameSoFar();

	bool t_whiteWon;
	int t_gameState = 0;

	do
	{
		if (m_gameBoard->IsWhiteTurn())
		{
			t_gameState = playerOne.MakeMove(*m_gameBoard);
		}

		else
		{
			t_gameState = playerTwo.MakeMove(*m_gameBoard);
		}

		if (t_gameState == 1)
		{
			t_whiteWon = true;
		}

		else if (t_gameState == -1)
		{
			t_whiteWon = false;
		}

	} while (t_gameState == 0);

	m_gameBoard->AdjustCoefficients(t_whiteWon);

	return t_whiteWon;
}

void GameController::WriteOutCoefficients()
{
	m_gameBoard->WriteOutCoefficients();
}

// OPERATOR OVERLOADS

void GameController::operator= (GameController const& controller)
{
	m_gameBoard = new Board();
	*m_gameBoard = *(controller.m_gameBoard);
}