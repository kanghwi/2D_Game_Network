#include "game.h"

#define FPS 144

using namespace std;

Game game;

int SDL_main(int argc, char* argv[])
{
	cout << "Start" << endl;
	
	int startTime, endTime;

	while (!game.done) 
	{
		startTime = clock();

		game.update();
		
		Sleep(1000 / FPS - game.delayTime);

		endTime = clock();
		game.delayTime = (endTime - startTime) / 1000.f;
	}

	//Á¾·á
	cout << "End\n" << endl;
	return 0;
}
