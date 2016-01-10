#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

class Board;
class Player;

class GameController
{
private :

	Board* m_gameBoard;

public :

	// CONSTRUCTORS

	GameController();
	GameController(Board const&);

	// DESTRUCTOR

	~GameController();

	// METHODS

	bool RunGame(Player const&, Player const&);
	void WriteOutCoefficients();

	// OPERATOR OVERLOADS

	void operator= (GameController const&);
};

#endif