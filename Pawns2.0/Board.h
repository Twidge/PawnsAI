#ifndef BOARD_H
#define BOARD_H

#include "Move.h"
#include <vector>

const unsigned int G_NUMBER_OF_PAWNS = G_BOARD_SIZE;
const double G_ADJUST_SIZE = 0.1;
const std::string G_COEFFICIENTS_FILENAME = "coefficients.txt";

class Board
{
private :

	std::vector<Move>* m_allMoves;
	std::vector<Move>* m_gameSoFar;

	// For squares, first coordinate is file. Second coordinate is rank.
	Square m_squares[G_BOARD_SIZE][G_BOARD_SIZE];
	Piece m_whitePieces[G_NUMBER_OF_PAWNS];
	Piece m_blackPieces[G_NUMBER_OF_PAWNS];

	bool m_isWhiteTurn = true;
	double m_materialFactor = 1;
	bool m_movesPopulated = false;

public :

	// CONSTRUCTORS
	Board();
	Board(const Board&);

	// DESTRUCTOR

	~Board();

	// GETTERS

	const std::vector<Move>* GetAllMoves() const;
	Move GetMove(unsigned int) const;
	const std::vector<Move>* GetGameSoFar() const;
	Move GetGameMove(unsigned int) const;
	Piece GetWhitePiece(int) const;
	Piece GetBlackPiece(int) const;
	double GetMaterialFactor() const;
	bool IsWhiteTurn() const;
	bool MovesPopulated() const;

	// METHODS

	// Validity check for a move
	bool IsMoveValid(const Move&) const;

	// Figures out move valuation
	double ValuateMove(const Move&) const;

	// Execute a move and records it in game so far
	int ExecuteMove(const Move&);

	// Execute a move but does not record it in game so 
	int ReplayMove(const Move&);

	// Reset board
	void ResetBoard();

	// Reset game so far
	void ResetGameSoFar();

	// Add a value to materialFactor
	void AddToMaterialFactor(double);

	// Populates m_allMoves with all possible moves on the board
	void PopulateMoves();

	// Adjusts coefficients for each move
	void AdjustCoefficients(bool);

	// Writes out new move coefficients to the relevant file
	void WriteOutCoefficients() const;

	// OPERATOR OVERLOADS

	const Board& operator=(const Board&);
	friend ostream& operator<< (ostream&, const Board&);
};

#endif