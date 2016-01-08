#include <vector>

class Board;
class Move;

class Player
{
public :

	// CONSTRUCTORS

	Player();
	Player(Player const&);

	// METHODS

	virtual int MakeMove(Board&) const;

private :
};

class HumanPlayer : public Player
{
public :

	// CONSTRUCTORS

	HumanPlayer();
	HumanPlayer(HumanPlayer const&);

	// METHODS

	virtual int MakeMove(Board&) const;

private :

};

class AIPlayer : public Player
{
public :

	// CONSTRUCTORS

	AIPlayer();
	AIPlayer(AIPlayer const&);

	// METHODS

	virtual int MakeMove(Board&) const;

private :

};