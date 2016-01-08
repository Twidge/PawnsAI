#include "Player.h"
#include "Move.h"
#include "Board.h"
#include <limits>

const int G_A_VERY_SMALL_VALUE = INT_MIN;

/*
	*********
	PLAYER
	*********
*/

Player::Player()
{

}

int Player::MakeMove(Board& board) const
{
	return 0;
}

/*
	*********
	HUMANPLAYER
	*********
*/

// CONSTRUCTOR

HumanPlayer::HumanPlayer() : Player()
{

}

// Returns 1 for a white win, -1 for a black win and 0 for game in progress

int HumanPlayer::MakeMove(Board& board) const
{
	cout << board;

	int t_gameState = 0;
	vector<Move> t_validMoves = std::vector<Move>();

	// Figure out all valid moves

	for (unsigned int l_move = 0; l_move < board.GetAllMoves()->size(); l_move++)
	{
		if (board.IsMoveValid(board.GetMove(l_move)))
		{
			t_validMoves.push_back(board.GetMove(l_move));
		}
	}

	// If there are none, game is over!

	if (t_validMoves.size() == 0)
	{
		if (board.IsWhiteTurn())
		{
			return -1;
		}

		else
		{
			return 1;
		}
	}

	// Prompt human for input until they type in a valid move, then execute that move

	do
	{
		int t_fileFrom, t_rankFrom, t_fileTo, t_rankTo;

		cout << "Enter the file of the square you are moving from: ";
		cin >> t_fileFrom;

		cout << "Enter the rank of the square you are moving from: ";
		cin >> t_rankFrom;

		cout << "Enter the file of the square you are moving to: ";
		cin >> t_fileTo;

		cout << "Enter the rank of the square you are moving to: ";
		cin >> t_rankTo;

		// Adjust for the array indices

		t_fileFrom -= 1;
		t_rankFrom -= 1;
		t_fileTo -= 1;
		t_rankTo -= 1;

		Square t_newFrom = Square(t_fileFrom, t_rankFrom);
		Square t_newTo = Square(t_fileTo, t_rankTo);
		Move t_candidateMove = Move(t_newFrom, t_newTo, true);

		for (unsigned int l_validMove = 0; l_validMove < t_validMoves.size(); l_validMove++)
		{
			if (t_validMoves[l_validMove].GetFrom() == t_newFrom && t_validMoves[l_validMove].GetTo() == t_newTo)
			{
				t_gameState = board.ExecuteMove(t_candidateMove);
				return t_gameState;
			}
		}

		cout << "That move is not allowed. Try again!\n";

	} while (true);
}


/*
	*********
	AIPLAYER
	*********
*/

// CONSTRUCTOR

AIPlayer::AIPlayer() : Player()
{

}

// Returns 1 for a white win, -1 for a black win and 0 for game in progress

int AIPlayer::MakeMove(Board& board) const
{
	int t_gameState = 0;
	std::vector<Move> t_validMoves = std::vector<Move>();
	std::vector<Move> t_candidateMoves = std::vector<Move>();

	// Figure out all valid moves

	for (unsigned int l_move = 0; l_move < board.GetAllMoves()->size(); l_move++)
	{
		if (board.IsMoveValid(board.GetMove(l_move)))
		{
			t_validMoves.push_back(board.GetMove(l_move));
		}
	}

	// If there are none, game is over!

	if (t_validMoves.size() == 0)
	{
		if (board.IsWhiteTurn())
		{
			return -1;
		}

		else
		{
			return 1;
		}
	}

	// Figure out which of the valid moves are best

	double maxValue = G_A_VERY_SMALL_VALUE;

	for (unsigned int l_move = 0; l_move < t_validMoves.size(); l_move++)
	{
		double t_result = board.ValuateMove(t_validMoves[l_move]);

		if (t_result > maxValue)
		{
			maxValue = t_result;

			t_candidateMoves.clear();
			t_candidateMoves.push_back(t_validMoves[l_move]);
		}

		else if (t_result == maxValue)
		{
			t_candidateMoves.push_back(t_validMoves[l_move]);
		}

		else if (t_result < G_A_VERY_SMALL_VALUE)
		{
			cout << "ValuateMove returned a value below -10000." << endl;

			cout << "result is: " << t_result << endl;

			cin.get();
		}
	}

	int t_chosenMove = rand() % t_candidateMoves.size();

	/*
	cout << "Move chosen: " << endl;
	cout << t_candidateMoves[t_chosenMove];
	cin.get();
	*/

	t_gameState = board.ExecuteMove(t_candidateMoves[t_chosenMove]);

	/*
	cout << "bar is: " << bar << endl;
	cin.get();
	*/

	/*
	cout << "link appended" << endl;
	cin.get();
	*/

	return t_gameState;
}