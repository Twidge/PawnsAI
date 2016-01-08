// Pawns2.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Board.h"
#include <ctime>
#include "GameController.h"
#include "Player.h"

// MAIN

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int t_numberOfGames;
	char t_charChoice;

	// Our game controller

	GameController t_controller = GameController(Board());

	// Random seed

	srand(static_cast<unsigned int>(time(NULL)));

	cout << "Enter a number of games to loop: ";
	cin >> t_numberOfGames;

	// Run the specified number of games

	for (unsigned int l_game = 1; l_game <= t_numberOfGames; l_game++)
	{
		t_controller.RunGame(AIPlayer(), AIPlayer());

		if (l_game % 100 == 0)
		{
			cout << "Game " << l_game << " done.\n";
		}
	}

	cout << "Learning complete. Beginning final game." << endl;
	cin.get();

	cout << "Would you like to play a game? Y for yes, not Y for no.\n";
	cin >> t_charChoice;

	if (t_charChoice == 'Y' || t_charChoice == 'y')
	{
		t_controller.RunGame(HumanPlayer(), AIPlayer());
	}

	else
	{
		t_controller.RunGame(AIPlayer(), AIPlayer());
	}

	t_controller.WriteOutCoefficients();
	std::cin.get();

	return 0;
}