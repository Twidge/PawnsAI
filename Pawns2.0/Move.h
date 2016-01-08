#ifndef MOVE_H
#define MOVE_H

#include "Piece.h"

const unsigned int G_BOARD_SIZE = 8;

class Move
{
private :

	Square m_from;
	Square m_to;
	bool m_isWhite;
	double m_coefficients[G_BOARD_SIZE][G_BOARD_SIZE];

public :

	// CONSTRUCTORS
	Move();
	Move(const Square&, const Square&, bool);
	Move(const Move&);

	// GETTERS
	const Square& GetFrom() const;
	const Square& GetTo() const;
	bool IsWhite() const;
	double GetCoefficient(int, int) const;

	// METHODS
	void AddToCoefficient(int, int, double);

	// OPERATOR OVERLOADS
	const Move& operator=(const Move&);
	friend ostream& operator<< (ostream&, const Move&);
};

#endif