#include "Move.h"

// CONSTRUCTORS

Move::Move(Square const& squareFrom, Square const& squareTo, bool whiteMove)
{
	m_from = squareFrom;
	m_to = squareTo;
	m_isWhite = whiteMove;

	for (int l_firstIndex = 0; l_firstIndex < G_BOARD_SIZE; l_firstIndex++)
	{
		for (int l_secondIndex = 0; l_secondIndex < G_BOARD_SIZE; l_secondIndex++)
		{
			m_coefficients[l_firstIndex][l_secondIndex] = 0;
		}
	}
}

Move::Move()
{
}

Move::Move(const Move& move)
{
	m_from = move.m_from;
	m_to = move.m_to;
	m_isWhite = move.m_isWhite;

	// Can probably just do coefficients = foo.coefficients

	for (int l_firstIndex = 0; l_firstIndex < G_BOARD_SIZE; l_firstIndex++)
	{
		for (int l_secondIndex = 0; l_secondIndex < G_BOARD_SIZE; l_secondIndex++)
		{
			m_coefficients[l_firstIndex][l_secondIndex] = move.m_coefficients[l_firstIndex][l_secondIndex];
		}
	}
}

// ASSIGNMENT OPERATOR

const Move& Move::operator=(const Move& move)
{
	m_from = move.m_from;
	m_to = move.m_to;
	m_isWhite = move.m_isWhite;

	for (int l_firstIndex = 0; l_firstIndex < G_BOARD_SIZE; l_firstIndex++)
	{
		for (int l_secondIndex = 0; l_secondIndex < G_BOARD_SIZE; l_secondIndex++)
		{
			m_coefficients[l_firstIndex][l_secondIndex] = move.m_coefficients[l_firstIndex][l_secondIndex];
		}
	}

	return *this;
}

// FRIENDS

ostream& operator<< (ostream& os, const Move& move)
{
	os << "From: " << move.GetFrom();
	os << "To: " << move.GetTo();

	if (move.IsWhite())
	{
		os << "Side: White\n";
	}

	else
	{
		os << "Side: Black\n";
	}

	/*
	for (int i = 0; i < 8; i++)
	{
		os << "Coefficients for rank " << i << ":";

		for (int j = 0; j < 8; j++)
		{
			os << " " << move.GetCoefficient(j, i);
		}

		os << "\n";
	}
	*/

	return os;
}

// GETTERS

const Square& Move::GetFrom() const
{
	return m_from;
}

const Square& Move::GetTo() const
{
	return m_to;
}

bool Move::IsWhite() const
{
	return m_isWhite;
}

double Move::GetCoefficient(int x, int y) const
{
	return m_coefficients[x][y];
}

// SETTERS

void Move::AddToCoefficient(int x, int y, double z)
{
	m_coefficients[x][y] += z;
}