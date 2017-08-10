// COMP710 GP 2D Framework 2017

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "entity.h"
#include <algorithm>

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>
#include <iostream>



// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, playerShip(new Entity())
, enemyContainer(0)
, bulletContainer(0)
{
	
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool 
Game::Initialise()
{
	const int width = 800;
	const int height = 600;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	// W03.1: Load the player ship sprite.
	Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");

	// W03.1: Create the player ship instance.

	playerShip->Initialise(pPlayerSprite);
	playerShip->SetPosition(400, 500);


	// W03.2: Spawn four rows of 14 alien enemies.

	// W03.2: Fill the container with these new enemies.

	for (int y = 50; y <= 200; y+=50){
		for (int x = 50; x <= 750; x += 50){
			SpawnEnemy(x, y);
		}
	}

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);
	
	if (m_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
		}
		
		Draw(*m_pBackBuffer);
	}

//	SDL_Delay(1);

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	// Update the game world simulation:

	// Ex003.5: Process each alien enemy in the container.
	std::vector<Entity*>::iterator enemyIter = enemyContainer.begin();
	while (enemyIter != enemyContainer.end())
	{
		//(*enemyIter)->SetDead(true);
		(*enemyIter)->Process(deltaTime);
		enemyIter++;
		
	}



	// W03.3: Process each bullet in the container.
	std::vector<Entity*>::iterator bulletIter = bulletContainer.begin();
	while (bulletIter != bulletContainer.end())
	{
		(*bulletIter)->Process(deltaTime);
		bulletIter++;
	}
			
	// W03.1: Update player...
	playerShip->Process(deltaTime);

	// W03.3: Check for bullet vs alien enemy collisions...
	bulletIter = bulletContainer.begin();
	enemyIter = enemyContainer.begin();
	// W03.3: For each bullet
	// W03.3: For each alien enemy
	// W03.3: Check collision between two entities.
	while (bulletIter != bulletContainer.end())
	{
		
		while (enemyIter != enemyContainer.end())
		{
			if ((*bulletIter)->IsCollidingWith(*enemyIter))
			{
				LogManager::GetInstance().Log("Has Collided");
				(*bulletIter)->SetDead(true);
				(*enemyIter)->SetDead(true);
			}
			enemyIter++;
		}
		bulletIter++;
	}

	

	// W03.3: If collided, destory both and spawn explosion.
	

	// W03.3: Remove any dead bullets from the container...
	bulletIter = bulletContainer.begin();
	while (bulletIter != bulletContainer.end())
	{
		if ((*bulletIter)->GetPositionY() < 0)
		{
			(*bulletIter)->SetDead(true);
		}
		if ((*bulletIter)->IsDead())
		{
			bulletIter = bulletContainer.erase(bulletIter);
		} else
		{
			bulletIter++;
		}
	}
	// W03.3: Remove any dead enemy aliens from the container...
	enemyIter = enemyContainer.begin();
	while (enemyIter != enemyContainer.end())
	{
		if ((*enemyIter)->IsDead()){
			enemyIter = enemyContainer.erase(enemyIter);
		}
		else{
			enemyIter++;
		}
	}
	// W03.3: Remove any dead explosions from the container...
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	// W03.2: Draw all enemy aliens in container...
	std::vector<Entity*>::iterator enemyIter = enemyContainer.begin();

	while (enemyIter != enemyContainer.end())
	{
		(*enemyIter)->Draw(backBuffer);
		enemyIter++;
	}

	std::vector<Entity*>::iterator bulletIter = bulletContainer.begin();

	while (bulletIter != bulletContainer.end())
	{
		(*bulletIter)->Draw(backBuffer);
		bulletIter++;
	}

	// W03.3: Draw all bullets in container...

	// W03.1: Draw the player ship...
	std::cout << playerShip->GetPositionX() << std::endl;
	playerShip->Draw(backBuffer);
	
	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void 
Game::MoveSpaceShipLeft()
{
	// W03.1: Tell the player ship to move left.
	playerShip->SetHorizontalVelocity(-10);        
}

// W03.1: Add the method to tell the player ship to move right...
void
Game::MoveSpaceShipRight()
{
	playerShip->SetHorizontalVelocity(10);
}

void
Game::StopSpaceShip()
{
	playerShip->SetHorizontalVelocity(0);
}


// W03.3: Space a Bullet in game.
void 
Game::FireSpaceShipBullet()
{
	// W03.3: Load the player bullet sprite. 
	Sprite* pBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");

	// W03.3: Create a new bullet object.
	Entity* bullet = new Entity();
	bullet->Initialise(pBulletSprite);
	bullet->SetPosition(playerShip->GetPositionX(), playerShip->GetPositionY());

	// W03.3: Set the bullets vertical velocity.
	bullet->SetVerticalVelocity(20);

	// W03.3: Add the new bullet to the bullet container.
	bulletContainer.push_back(bullet);
}

// W03.2: Spawn a Enemy in game.
void 
Game::SpawnEnemy(int x, int y)
{
	// W03.2: Load the alien enemy sprite file.
			Sprite* pEnemySprite = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");

	// W03.2: Create a new Enemy object. 
			Entity* enemy = new Entity();
			enemy->Initialise(pEnemySprite);
			enemy->SetPosition(x, y);

	// W03.2: Add the new Enemy to the enemy container.
			enemyContainer.push_back(enemy);
}			

