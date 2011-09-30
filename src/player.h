// to prevent multiple compiling of this header
#pragma once

#include <hge.h>
#include <hgerect.h>
#include <hgevector.h>
#include <hgesprite.h>

#include "sprite_manager.h"


class World;

// Player class
// controls keyboard interaction and game rules
class Player
{
protected:
	// Horizontal and vertical speeds of player.
	// You can use acceleration to affect this value when player presses buttons
	// to simulate inertia. Or you can write exact speeds when player presses buttons
	// to do instant turns and direction changes.
	hgeVertex	m_speed;

	// Hack this value for infinite lives
	int			m_lives;

	SpriteManager	m_sprite_manager;

	hgeSprite	* m_character_right[2];

public:
	// A bounding box for character sprite used for rendering and collisions
	// for simplicity player has same size as world cells
	hgeRect		m_position;

	const static int INITIAL_LIVES_COUNT = 3;

public:
	Player();
	virtual ~Player();

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
	virtual void Die();

	// Calculates current sprite to render, using last direction and time to find the frame
	virtual hgeSprite * GetSprite();

	// Draws character over the world, using World's camera to calculate positions
	virtual void Render( World * world );

	// called by gamestate when player enters the world and game begins
	void EnterWorld( World * w );
};
