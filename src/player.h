// to prevent multiple compiling of this header
#pragma once

#include <hge.h>
#include <hgerect.h>
#include <hgevector.h>

// Player class
// controls keyboard interaction and game rules
class Player
{
protected:
	// A bounding box for character sprite used for rendering and collisions
	hgeRect		m_position;

	// Horizontal and vertical speeds of player.
	// You can use acceleration to affect this value when player presses buttons
	// to simulate inertia. Or you can write exact speeds when player presses buttons
	// to do instant turns and direction changes.
	hgeVertex	m_speed;

	// Hack this value for infinite lives
	int			m_lives;

public:
	const static int INITIAL_LIVES_COUNT = 3;

	Player(): m_lives(INITIAL_LIVES_COUNT)
	{
	}

	// player's vertical speed will be instantly set to this value on jump, and then
	// will be gradually reduced by World::GravityAccel every frame to simulate proper
	// jumping.
	// This can be modified to return different values depending if player picks up 
	// some bonus for higher jumps or eats some weakening mushroom
	float JumpAccel() {
		return 50.0f;
	}

	hgeVector GetPos();

	// this one should reduce lives count
	void Die();
};
