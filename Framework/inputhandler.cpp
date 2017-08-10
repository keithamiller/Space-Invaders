// COMP710 GP 2D Framework 2017

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "logmanager.h"
#include "iostream"
#include <string>
using namespace std;

// Library includes:
#include <cassert>

InputHandler::InputHandler()
: m_pGameController(0)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool 
InputHandler::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);

	if (!m_pGameController)
	{
		LogManager::GetInstance().Log("No controller detected!");
	}

	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	// W03.1: Receive Input Events below...
	SDL_Event e;



 	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			game.Quit();
		}
		// W03.1: Tell the game to move the space ship right...
		else if (e.type == SDL_JOYHATMOTION)
		{
			switch (e.jhat.value){
			case SDL_HAT_LEFT:
				game.MoveSpaceShipLeft();
				break;
			case SDL_HAT_RIGHT:
				game.MoveSpaceShipRight();
				break;
			case SDL_HAT_CENTERED:
				game.StopSpaceShip();
				break;
			}
		}
		else if (e.type == SDL_JOYBUTTONDOWN)
		{
			char button[80];
			sprintf(button, "%d", e.jbutton.button);
			LogManager::GetInstance().Log(button);
			// W03.3: Tell the game to fire a player bullet...
			switch (e.jbutton.button){
			case 0:
				game.FireSpaceShipBullet();
				break;
			}
			
		}
		
	}
}
