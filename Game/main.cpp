#include "Game.h"

int main()
{
	std::shared_ptr<Game> game = std::make_shared<Game>(); // initialize the game as a smart pointer

	game->initialize(); // initializes the window and other game functionalities.

  return 0;
}
