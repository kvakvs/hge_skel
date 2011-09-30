// Player class
// controls keyboard interaction and game rules
#include "player.h"
#include "world.h"


Player::Player()
	: m_lives(INITIAL_LIVES_COUNT)
	, m_position(0, 0, World::CELL_BOX_SIZE, World::CELL_BOX_SIZE)
{
	m_character_right[0] = m_sprite_manager.GetSprite( "textures/mario_r1.png" );
	m_character_right[1] = m_sprite_manager.GetSprite( "textures/mario_r2.png" );
}


Player::~Player()
{
	delete m_character_right[0];
	delete m_character_right[1];
}


void Player::EnterWorld( World * w )
{
	// we need a simple way to determine player starting position
	// we scan the world for '@' and use that as start
	for( uint32_t r = 0; r < w->m_world_height; ++r ) {
		for( uint32_t c = 0; c < w->m_world_width; ++c ) {
			if( w->At(r,c) == World::WORLD_CELL_PLAYER_START )
			{
				// found the @ character, set player position
				m_position.Set(
					c * World::CELL_BOX_SIZE, r * World::CELL_BOX_SIZE,
					(c+1) * World::CELL_BOX_SIZE - 1, (r+1) * World::CELL_BOX_SIZE - 1
					);
			}
		}
	}
}


hgeVector Player::GetPos()
{
	return hgeVector(m_position.x1, m_position.y1);
}


void Player::Die()
{
	// TODO: invent the way for player to inform the gamestate or the world about
	// level game restart or scroll back to allow player to continue
	m_lives--;
}


hgeSprite * Player::GetSprite()
{
	uint32_t milliseconds = GetTickCount();
	
	// we want frames to change every 333 msec from 0 to 1
	// total of 2 frames, hence the modulo of 2
	uint32_t f = (milliseconds / 333) % 2;

	return m_character_right[f];
}


void Player::Render( World * world )
{
	hgeSprite * spr = GetSprite();
	if( ! spr ) return;

	hgeVector pos = GetPos();

	spr->Render( pos.x - world->m_camera_pos.x, pos.y - world->m_camera_pos.y );
}
