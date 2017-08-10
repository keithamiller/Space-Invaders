// COMP710 GP 2D Framework 2017

// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "logmanager.h"

// Library includes:
#include <cassert>
#include <cmath>

Entity::Entity()
: m_pSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(false)
{

}

Entity::~Entity()
{

}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

void 
Entity::Process(float deltaTime)
{
	// W03.1: Generic position update, based upon velocity (and time).
		float x = GetPositionX() + GetHorizontalVelocity();
		float y = GetPositionY() - GetVerticalVelocity();
		SetPosition(x, y);

	// W03.1: Boundary checking and position capping. 
	//TODO 
		if (m_x == 0 && GetHorizontalVelocity() < 0)
		{
			m_velocityX = 0;
		}
		else if (m_x == 770 && GetHorizontalVelocity() > 0){
			m_velocityX = 0;
		}
}

void 
Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
	m_pSprite->Draw(backBuffer);
}

bool
Entity::IsCollidingWith(Entity* e)
{
	// W03.3: Generic Entity Collision routine.
	bool isColliding = false;
	float distanceBetweenEntities = 0;
	distanceBetweenEntities =
		std::sqrt(pow(e->GetPositionX() - GetPositionX(), 2)
		+ pow(e->GetPositionY() - GetPositionY(), 2))
		- e->m_pSprite->GetWidth() - m_pSprite->GetWidth();

	if (distanceBetweenEntities < 0){
		isColliding = true;
	}

	
	
	return (isColliding); // W03.4 Change return value!
}

void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

bool
Entity::IsDead() const
{
	return m_dead;
}

void
Entity::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}

float 
Entity::GetPositionX() const
{
	return (m_x);
}

float 
Entity::GetPositionY() const
{
	return (m_y);
}

float 
Entity::GetHorizontalVelocity() const
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity() const
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}
