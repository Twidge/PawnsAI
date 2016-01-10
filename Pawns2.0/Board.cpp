#include "Board.h"
#include <cmath>
#include <fstream>

// CONSTRUCTORS

Board::Board()
{
	for (int l_file = 0; l_file < G_BOARD_SIZE; l_file++)
	{
		for (int l_rank = 0; l_rank < G_BOARD_SIZE; l_rank++)
		{
			m_squares[l_file][l_rank] = Square(l_file, l_rank);
		}

		m_whitePieces[l_file] = Piece(true, m_squares[l_file][1]);
		m_blackPieces[l_file] = Piece(false, m_squares[l_file][6]);
	}

	m_allMoves = new std::vector<Move>();
	m_gameSoFar = new std::vector<Move>();
}

Board::Board(const Board& board)
{
	for (int l_file = 0; l_file < G_BOARD_SIZE; l_file++)
	{
		for (int l_rank = 0; l_rank < G_BOARD_SIZE; l_rank++)
		{
			m_squares[l_file][l_rank] = board.m_squares[l_file][l_rank];
		}

		m_whitePieces[l_file] = board.m_whitePieces[l_file];
		m_blackPieces[l_file] = board.m_blackPieces[l_file];
	}

	m_isWhiteTurn = board.m_isWhiteTurn;
	m_materialFactor = board.m_materialFactor;
	m_movesPopulated = board.m_movesPopulated;

	m_allMoves = new std::vector<Move>();
	*m_allMoves = *board.m_allMoves;

	m_gameSoFar = new std::vector<Move>();
	*m_gameSoFar = *board.m_gameSoFar;
}

// DESTRUCTOR

Board::~Board()
{
	delete m_allMoves;
	delete m_gameSoFar;
}

// GETTERS

const std::vector<Move>* Board::GetAllMoves() const
{
	return m_allMoves;
}

Move Board::GetMove(unsigned int move) const
{
	return (*m_allMoves)[move];
}

const std::vector<Move>* Board::GetGameSoFar() const
{
	return m_gameSoFar;
}

Move Board::GetGameMove(unsigned int move) const
{
	return (*m_gameSoFar)[move];
}

Piece Board::GetWhitePiece(int x) const
{
	return m_whitePieces[x];
}

Piece Board::GetBlackPiece(int x) const
{
	return m_blackPieces[x];
}

double Board::GetMaterialFactor() const
{
	return m_materialFactor;
}

bool Board::IsWhiteTurn() const
{
	return m_isWhiteTurn;
}

bool Board::MovesPopulated() const
{
	return m_movesPopulated;
}

// Check if a move is valid

bool Board::IsMoveValid(const Move& move) const
{
	bool t_sourceCheck = false;
	
	// Return false if the move's colour does not correspond to the player whose turn it is

	if (move.IsWhite() != m_isWhiteTurn)
	{
		return false;
	}

	// Return false if a piece of the move's colour is not on the source square

	if (move.IsWhite())
	{
		for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
		{
			if (m_whitePieces[l_piece].GetSquare() == move.GetFrom() && m_whitePieces[l_piece].IsOnBoard())
			{
				t_sourceCheck = true;
				break;
			}
		}
	}

	if (!move.IsWhite())
	{
		for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
		{
			if (m_blackPieces[l_piece].GetSquare() == move.GetFrom() && m_blackPieces[l_piece].IsOnBoard())
			{
				t_sourceCheck = true;
				break;
			}
		}
	}

	if (!t_sourceCheck)
	{
		return false;
	}

	// If file is not the same, check it satisfies the following:
	// 1. It's a one-square diagonal move
	// 2. There's an opposing piece on the target square

	if (move.GetFrom().GetFile() - move.GetTo().GetFile() != 0)
	{
		// First case - if the move is for a white piece
		
		if (move.IsWhite())
		{
			// If not a one-square diagonal move up the board, return false

			if (move.GetFrom().GetFile() - move.GetTo().GetFile() != 1 && move.GetFrom().GetFile() - move.GetTo().GetFile() != -1)
			{
				return false;
			}

			if (move.GetTo().GetRank() - move.GetFrom().GetRank() != 1)
			{
				return false;
			}

			// Having passed that check, if there's a black piece on the target square return true (else return false)

			for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
			{
				if (m_blackPieces[l_piece].GetSquare() == move.GetTo() && m_blackPieces[l_piece].IsOnBoard())
				{
					return true;
				}
			}

			return false;
		}

		// Same again but if the move is for a black piece

		if (!move.IsWhite())
		{
			// If not a one-square diagonal move down the board, return false

			if (move.GetFrom().GetFile() - move.GetTo().GetFile() != 1 && move.GetFrom().GetFile() - move.GetTo().GetFile() != -1)
			{
				return false;
			}

			if (move.GetFrom().GetRank() - move.GetTo().GetRank() != 1)
			{
				return false;
			}

			// Having passed that check, if there's a white piece on the target square return true (else return false)

			for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
			{
				if (m_whitePieces[l_piece].GetSquare() == move.GetTo() && m_whitePieces[l_piece].IsOnBoard())
				{
					return true;
				}
			}

			return false;
		}
	}

	// If the file is the same, check the move satisfies the following:
	// 1. It is either one or two squares in the appropriate direction (depending on whether the piece is on its starting square)
	// 2. There are no pieces in the way

	if (move.GetFrom().GetFile() == move.GetTo().GetFile())
	{
		// First case - move is for a white piece

		if (move.IsWhite())
		{
			if (move.GetTo().GetRank() - move.GetFrom().GetRank() > 2 || move.GetTo().GetRank() - move.GetFrom().GetRank() <= 0)
			{
				return false;
			}

			if (move.GetTo().GetRank() - move.GetFrom().GetRank() == 2 && move.GetFrom().GetRank() != 1)
			{
				return false;
			}

			// Now for each intermediate square, return false if there's a piece on it

			for (int l_squareBetween = move.GetFrom().GetRank() + 1; l_squareBetween <= move.GetTo().GetRank(); l_squareBetween++)
			{
				for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
				{
					if (m_whitePieces[l_piece].GetSquare() == Square(move.GetFrom().GetFile(), l_squareBetween)
						&& m_whitePieces[l_piece].IsOnBoard())
					{
						return false;
					}

					if (m_blackPieces[l_piece].GetSquare() == Square(move.GetFrom().GetFile(), l_squareBetween)
						&& m_blackPieces[l_piece].IsOnBoard())
					{
						return false;
					}
				}
			}

			return true;
		}

		// Same again, but if the move is for a black piece

		if (!move.IsWhite())
		{
			if (move.GetFrom().GetRank() - move.GetTo().GetRank() > 2 || move.GetFrom().GetRank() - move.GetTo().GetRank() <= 0)
			{
				return false;
			}

			if (move.GetFrom().GetRank() - move.GetTo().GetRank() == 2 && move.GetFrom().GetRank() != (G_BOARD_SIZE - 1) - 1)
			{
				return false;
			}

			// Now for each intermediate square, return false if there's a piece on it

			for (int l_squareBetween = move.GetFrom().GetRank() - 1; l_squareBetween >= move.GetTo().GetRank(); l_squareBetween--)
			{
				for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
				{
					if (m_whitePieces[l_piece].GetSquare() == Square(move.GetFrom().GetFile(), l_squareBetween)
						&& m_whitePieces[l_piece].IsOnBoard())
					{
						return false;
					}

					if (m_blackPieces[l_piece].GetSquare() == Square(move.GetFrom().GetFile(), l_squareBetween)
						&& m_whitePieces[l_piece].IsOnBoard())
					{
						return false;
					}
				}
			}

			return true;
		}
	}
}

// Check the "value" for a given move (used to find out which move is best in a given situation)

double Board::ValuateMove(const Move& move) const
{
	double t_valuation = 0;

	if (move.IsWhite())
	{
		for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
		{
			if (m_whitePieces[l_piece].IsOnBoard())
			{
				t_valuation += move.GetCoefficient(m_whitePieces[l_piece].GetSquare().GetFile(), m_whitePieces[l_piece].GetSquare().GetRank());
			}

			if (m_blackPieces[l_piece].IsOnBoard())
			{
				t_valuation -= move.GetCoefficient(m_blackPieces[l_piece].GetSquare().GetFile(), m_blackPieces[l_piece].GetSquare().GetRank());
			}
		}
	}

	else
	{
		for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
		{
			if (m_whitePieces[l_piece].IsOnBoard())
			{
				t_valuation -= move.GetCoefficient(m_whitePieces[l_piece].GetSquare().GetFile(), m_whitePieces[l_piece].GetSquare().GetRank());
			}

			if (m_blackPieces[l_piece].IsOnBoard())
			{
				t_valuation += move.GetCoefficient(m_blackPieces[l_piece].GetSquare().GetFile(), m_blackPieces[l_piece].GetSquare().GetRank());
			}
		}
	}

	return t_valuation;
}

// Executes a given move on the board. Returns 1 if white wins after this move, -1 if black wins and 0 if game is ongoing.

int Board::ExecuteMove(const Move& move)
{
	int t_gameStatus = 0;

	if (move.IsWhite())
	{
		for (int l_whitePiece = 0; l_whitePiece < G_NUMBER_OF_PAWNS; l_whitePiece++)
		{
			if (m_whitePieces[l_whitePiece].GetSquare() == move.GetFrom() && m_whitePieces[l_whitePiece].IsOnBoard())
			{
				for (int l_blackPiece = 0; l_blackPiece < G_NUMBER_OF_PAWNS; l_blackPiece++)
				{
					if (m_blackPieces[l_blackPiece].GetSquare() == move.GetTo() && m_blackPieces[l_blackPiece].IsOnBoard())
					{
						m_blackPieces[l_blackPiece].SetOnBoard(false);
					}
				}

				m_whitePieces[l_whitePiece].SetSquare(move.GetTo());

				if (move.GetTo().GetRank() == G_BOARD_SIZE - 1)
				{
					t_gameStatus = 1;
				}

				break;
			}
		}
	}

	else
	{
		for (int l_blackPiece = 0; l_blackPiece < G_NUMBER_OF_PAWNS; l_blackPiece++)
		{
			if (m_blackPieces[l_blackPiece].GetSquare() == move.GetFrom() && m_blackPieces[l_blackPiece].IsOnBoard())
			{
				for (int l_whitePiece = 0; l_whitePiece < G_NUMBER_OF_PAWNS; l_whitePiece++)
				{
					if (m_whitePieces[l_whitePiece].GetSquare() == move.GetTo() && m_whitePieces[l_whitePiece].IsOnBoard())
					{
						m_whitePieces[l_whitePiece].SetOnBoard(false);
					}
				}

				m_blackPieces[l_blackPiece].SetSquare(move.GetTo());

				if (move.GetTo().GetRank() == 0)
				{
					t_gameStatus = -1;
				}

				break;
			}
		}
	}

	// Swap colour whose turn it is!

	if (m_isWhiteTurn)
	{
		m_isWhiteTurn = false;
	}

	else
	{
		m_isWhiteTurn = true;
	}

	// Add the move to the game so far

	m_gameSoFar->push_back(move);

	return t_gameStatus;
}

// Execute a move but does not record it in game so 
int Board::ReplayMove(const Move& move)
{
	int t_gameStatus = 0;

	if (move.IsWhite())
	{
		for (int l_whitePiece = 0; l_whitePiece < G_NUMBER_OF_PAWNS; l_whitePiece++)
		{
			if (m_whitePieces[l_whitePiece].GetSquare() == move.GetFrom() && m_whitePieces[l_whitePiece].IsOnBoard())
			{
				for (int l_blackPiece = 0; l_blackPiece < G_NUMBER_OF_PAWNS; l_blackPiece++)
				{
					if (m_blackPieces[l_blackPiece].GetSquare() == move.GetTo() && m_blackPieces[l_blackPiece].IsOnBoard())
					{
						m_blackPieces[l_blackPiece].SetOnBoard(false);
					}
				}

				m_whitePieces[l_whitePiece].SetSquare(move.GetTo());

				if (move.GetTo().GetRank() == G_BOARD_SIZE - 1)
				{
					t_gameStatus = 1;
				}

				break;
			}
		}
	}

	else
	{
		for (int l_blackPiece = 0; l_blackPiece < G_NUMBER_OF_PAWNS; l_blackPiece++)
		{
			if (m_blackPieces[l_blackPiece].GetSquare() == move.GetFrom() && m_blackPieces[l_blackPiece].IsOnBoard())
			{
				for (int l_whitePiece = 0; l_whitePiece < G_NUMBER_OF_PAWNS; l_whitePiece++)
				{
					if (m_whitePieces[l_whitePiece].GetSquare() == move.GetTo() && m_whitePieces[l_whitePiece].IsOnBoard())
					{
						m_whitePieces[l_whitePiece].SetOnBoard(false);
					}
				}

				m_blackPieces[l_blackPiece].SetSquare(move.GetTo());

				if (move.GetTo().GetRank() == 0)
				{
					t_gameStatus = -1;
				}

				break;
			}
		}
	}

	// Swap colour whose turn it is!

	if (m_isWhiteTurn)
	{
		m_isWhiteTurn = false;
	}

	else
	{
		m_isWhiteTurn = true;
	}

	// Add the move to the game so far

	return t_gameStatus;
}

// Reset the board

void Board::ResetBoard()
{
	for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
	{
		m_whitePieces[l_piece].SetOnBoard(true);
		m_whitePieces[l_piece].SetSquare(Square(l_piece, 1));

		m_blackPieces[l_piece].SetOnBoard(true);
		m_blackPieces[l_piece].SetSquare(Square(l_piece, 6));
	}

	m_isWhiteTurn = true;
}

// Reset the game so far

void Board::ResetGameSoFar()
{
	m_gameSoFar->clear();
}

// Add a value to materialFactor

void Board::AddToMaterialFactor(double toAdd)
{
	m_materialFactor += toAdd;
}

// PopulateMoves should be called once to generate a candidate move list. Each move is then checked each turn for validity.

void Board::PopulateMoves()
{
	if (!m_movesPopulated)
	{
		// Populate all white moves:
		// 1. All moves one square straight forward
		// 2. All moves one square forward and one square right/left
		// 3. All moves two squares forward starting on rank 1

		for (int l_file = 0; l_file < G_BOARD_SIZE; l_file++)
		{
			for (int l_rank = 1; l_rank <= (G_BOARD_SIZE - 1) - 1; l_rank++)
			{
				m_allMoves->push_back(Move(Square(l_file, l_rank), Square(l_file, l_rank + 1), true));

				if (l_file != 0)
				{
					m_allMoves->push_back(Move(Square(l_file, l_rank), Square(l_file - 1, l_rank + 1), true));
				}

				if (l_file != G_BOARD_SIZE)
				{
					m_allMoves->push_back(Move(Square(l_file, l_rank), Square(l_file + 1, l_rank + 1), true));
				}

				if (l_rank == 1)
				{
					m_allMoves->push_back(Move(Square(l_file, l_rank), Square(l_file, l_rank + 2), true));
				}
			}
		}

		// Populate all black moves: see all white moves, but in reverse

		for (int l_file = 0; l_file < G_BOARD_SIZE; l_file++)
		{
			for (int l_rank = 1; l_rank <= (G_BOARD_SIZE - 1) - 1; l_rank++)
			{
				m_allMoves->push_back(Move(Square(l_file, l_rank), Square(l_file, l_rank - 1), false));

				if (l_file != 0)
				{
					m_allMoves->push_back(Move(Square(l_file, l_rank), Square(l_file - 1, l_rank - 1), false));
				}

				if (l_file != G_BOARD_SIZE)
				{
					m_allMoves->push_back(Move(Square(l_file, l_rank), Square(l_file + 1, l_rank - 1), false));
				}

				if (l_rank == (G_BOARD_SIZE - 1) - 1)
				{
					m_allMoves->push_back(Move(Square(l_file, l_rank), Square(l_file, l_rank - 2), false));
				}
			}
		}

		ifstream t_fileReader;

		t_fileReader.open(G_COEFFICIENTS_FILENAME);

		if (t_fileReader.is_open())
		{
			double value;

			for (unsigned int l_move = 0; l_move < m_allMoves->size(); l_move++)
			{
				for (unsigned int l_file = 0; l_file < G_BOARD_SIZE; l_file++)
				{
					for (unsigned int l_rank = 0; l_rank < G_BOARD_SIZE; l_rank++)
					{
						t_fileReader >> value;

						(*m_allMoves)[l_move].AddToCoefficient(l_file, l_rank, value);
					}
				}
			}
		}

		m_movesPopulated = true;
	}
}

/*
AdjustCoefficients() takes a completed game and:
1. For each move the winning player made, adjusts the entries in the move's coefficient matrix upwards if a piece appeared on that
square.
2. For each move the losing player made, adjusts the entries in the move's coefficient matrix downwards if a piece appeared on that
square.
*/

void Board::AdjustCoefficients(bool didWhiteWin)
{
	ResetBoard();

	for (unsigned int l_gameMove = 0; l_gameMove < m_gameSoFar->size(); l_gameMove++)
	{
		if (didWhiteWin)
		{
			if ((*m_gameSoFar)[l_gameMove].IsWhite())
			{
				for (unsigned int l_move = 0; l_move < m_allMoves->size(); l_move++)
				{
					if ((*m_allMoves)[l_move].GetFrom() == (*m_gameSoFar)[l_gameMove].GetFrom()
						&& (*m_allMoves)[l_move].GetTo() == (*m_gameSoFar)[l_gameMove].GetTo()
						&& (*m_allMoves)[l_move].IsWhite() == (*m_gameSoFar)[l_gameMove].IsWhite())
					{
						for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
						{
							(*m_allMoves)[l_move].AddToCoefficient(m_whitePieces[l_piece].GetSquare().GetFile(), m_whitePieces[l_piece].GetSquare().GetRank(), G_ADJUST_SIZE);
							(*m_allMoves)[l_move].AddToCoefficient(m_blackPieces[l_piece].GetSquare().GetFile(), m_blackPieces[l_piece].GetSquare().GetRank(), -1 * G_ADJUST_SIZE);
						}

						break;
					}
				}
			}

			else
			{
				for (unsigned int l_move = 0; l_move < m_allMoves->size(); l_move++)
				{
					if ((*m_allMoves)[l_move].GetFrom() == (*m_gameSoFar)[l_gameMove].GetFrom()
						&& (*m_allMoves)[l_move].GetTo() == (*m_gameSoFar)[l_gameMove].GetTo()
						&& (*m_allMoves)[l_move].IsWhite() == (*m_gameSoFar)[l_gameMove].IsWhite())
					{
						for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
						{
							(*m_allMoves)[l_move].AddToCoefficient(m_whitePieces[l_piece].GetSquare().GetFile(), m_whitePieces[l_piece].GetSquare().GetRank(), G_ADJUST_SIZE);
							(*m_allMoves)[l_move].AddToCoefficient(m_blackPieces[l_piece].GetSquare().GetFile(), m_blackPieces[l_piece].GetSquare().GetRank(), -1 * G_ADJUST_SIZE);
						}

						break;
					}
				}
			}
		}

		else
		{
			if ((*m_gameSoFar)[l_gameMove].IsWhite())
			{
				for (unsigned int l_move = 0; l_move < m_allMoves->size(); l_move++)
				{
					if ((*m_allMoves)[l_move].GetFrom() == (*m_gameSoFar)[l_gameMove].GetFrom()
						&& (*m_allMoves)[l_move].GetTo() == (*m_gameSoFar)[l_gameMove].GetTo()
						&& (*m_allMoves)[l_move].IsWhite() == (*m_gameSoFar)[l_gameMove].IsWhite())
					{
						for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
						{
							(*m_allMoves)[l_move].AddToCoefficient(m_whitePieces[l_piece].GetSquare().GetFile(), m_whitePieces[l_piece].GetSquare().GetRank(), -1 * G_ADJUST_SIZE);
							(*m_allMoves)[l_move].AddToCoefficient(m_blackPieces[l_piece].GetSquare().GetFile(), m_blackPieces[l_piece].GetSquare().GetRank(), G_ADJUST_SIZE);
						}

						break;
					}
				}
			}

			else
			{
				for (unsigned int l_move = 0; l_move < m_allMoves->size(); l_move++)
				{
					if ((*m_allMoves)[l_move].GetFrom() == (*m_gameSoFar)[l_gameMove].GetFrom()
						&& (*m_allMoves)[l_move].GetTo() == (*m_gameSoFar)[l_gameMove].GetTo()
						&& (*m_allMoves)[l_move].IsWhite() == (*m_gameSoFar)[l_gameMove].IsWhite())
					{
						for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
						{
							(*m_allMoves)[l_move].AddToCoefficient(m_whitePieces[l_piece].GetSquare().GetFile(), m_whitePieces[l_piece].GetSquare().GetRank(), -1 * G_ADJUST_SIZE);
							(*m_allMoves)[l_move].AddToCoefficient(m_blackPieces[l_piece].GetSquare().GetFile(), m_blackPieces[l_piece].GetSquare().GetRank(), G_ADJUST_SIZE);
						}

						break;
					}
				}
			}
		}

		ReplayMove((*m_gameSoFar)[l_gameMove]);
	}
}

// Writes out coefficients to the relevant file

void Board::WriteOutCoefficients() const
{
	ofstream t_fileWriter;

	t_fileWriter.open(G_COEFFICIENTS_FILENAME, std::ofstream::out | std::ofstream::trunc);

	if (t_fileWriter.is_open())
	{
		for (unsigned int l_move = 0; l_move < m_allMoves->size(); l_move++)
		{
			for (unsigned int l_file = 0; l_file < G_BOARD_SIZE; l_file++)
			{
				for (unsigned int l_rank = 0; l_rank < G_BOARD_SIZE; l_rank++)
				{
					t_fileWriter << (*m_allMoves)[l_move].GetCoefficient(l_file, l_rank) << "\n";
				}
			}
		}
	}

	else
	{
		std::cout << "Error in WriteOutCoefficients(): file could not be opened.\n";
	}

	t_fileWriter.close();
}

// OPERATOR OVERLOADS

const Board& Board::operator= (const Board& board)
{
	for (int l_file = 0; l_file < G_BOARD_SIZE; l_file++)
	{
		for (int l_rank = 0; l_rank < G_BOARD_SIZE; l_rank++)
		{
			m_squares[l_file][l_rank] = board.m_squares[l_file][l_rank];
		}
	}

	for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
	{
		m_whitePieces[l_piece] = board.m_whitePieces[l_piece];
		m_blackPieces[l_piece] = board.m_blackPieces[l_piece];
	}

	m_isWhiteTurn = board.m_isWhiteTurn;

	m_materialFactor = board.m_materialFactor;

	delete m_allMoves;
	m_allMoves = new std::vector<Move>();
	m_allMoves = board.m_allMoves;

	delete m_gameSoFar;
	m_gameSoFar = new std::vector<Move>();
	m_gameSoFar = board.m_gameSoFar;

	return *this;
}

ostream& operator<< (ostream& os, const Board& board)
{
	for (int l_file = G_BOARD_SIZE - 1; l_file >= 0; l_file--)
	{
		for (int l_rank = 0; l_rank < G_BOARD_SIZE; l_rank++)
		{
			for (int l_piece = 0; l_piece < G_NUMBER_OF_PAWNS; l_piece++)
			{
				if (board.m_whitePieces[l_piece].GetSquare() == Square(l_rank, l_file) && board.m_whitePieces[l_piece].IsOnBoard())
				{
					cout << "W";
					break;
				}

				else if (board.m_blackPieces[l_piece].GetSquare() == Square(l_rank, l_file) && board.m_blackPieces[l_piece].IsOnBoard())
				{
					cout << "B";
					break;
				}

				if (l_piece == (G_BOARD_SIZE - 1))
				{
					cout << ".";
				}
			}

			cout << " ";
		}

		cout << "\n";
	}

	return os;
}