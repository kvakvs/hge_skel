// Player class
// controls keyboard interaction and game rules
#include "player.h"
#include "world.h"

#undef min
#include <algorithm>


Player::Player()
	: m_lives(INITIAL_LIVES_COUNT)
	, m_position(0, 0, World::CELL_BOX_SIZE-1, World::CELL_BOX_SIZE-1), m_speed(0,0)
	, m_last_facing(FACING_RIGHT)
{
	m_character_right[0][0] = m_sprite_manager.GetSprite( "textures/mario_r1.png" );
	m_character_right[0][1] = m_sprite_manager.GetSprite( "textures/mario_r2.png" );
	m_character_right[1][0] = m_sprite_manager.GetSprite( "textures/mario_l1.png" );
	m_character_right[1][1] = m_sprite_manager.GetSprite( "textures/mario_l2.png" );

	// this will not create another HGE, instead we will get the global
	// unique HGE object which is already started
	m_hge = hgeCreate( HGE_VERSION );
}


Player::~Player()
{
	delete m_character_right[0][0];
	delete m_character_right[0][1];
	delete m_character_right[1][0];
	delete m_character_right[1][1];
}


void Player::EnterWorld( World * w )
{
	m_world = w;

	// we need a simple way to determine player starting position
	// we scan the world for '@' and use that as start
	for( uint32_t r = 0; r < w->m_world_height; ++r ) {
		for( uint32_t c = 0; c < w->m_world_width; ++c ) {
			if( w->At(r,c) == World::WORLD_CELL_PLAYER_START )
			{
				// found the @ character, set player position
				MoveTo( c * World::CELL_BOX_SIZE, r * World::CELL_BOX_SIZE );
			}
		}
	}
}


hgeRect Player::GetScreenPosition()
{
	hgeRect spos = m_position;
	spos.x1 -= m_world->m_camera_pos.x;
	spos.x2 -= m_world->m_camera_pos.x;
	spos.y1 -= m_world->m_camera_pos.y;
	spos.y2 -= m_world->m_camera_pos.y;
	return spos;
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

	return m_character_right[m_last_facing][f];
}


void Player::Render( World * world )
{
	hgeSprite * spr = GetSprite();
	if( ! spr ) return;

	hgeRect pos = GetScreenPosition();

	spr->Render( pos.x1, pos.y1 );

	m_hge->Gfx_RenderLine( pos.x1, pos.y1, pos.x2, pos.y1 );
	m_hge->Gfx_RenderLine( pos.x2, pos.y1, pos.x2, pos.y2 );
	m_hge->Gfx_RenderLine( pos.x2, pos.y2, pos.x1, pos.y2 );
	m_hge->Gfx_RenderLine( pos.x1, pos.y1, pos.x1, pos.y2 );
}


void Player::Think()
{
	float delta = m_hge->Timer_GetDelta();

	if( m_hge->Input_GetKeyState( HGEK_RIGHT )
		|| m_hge->Input_GetKeyState( HGEK_D ) )
	{
		m_last_facing = FACING_RIGHT;
		m_speed.x = BASE_MOVING_SPEED;
	} 
	else if( m_hge->Input_GetKeyState( HGEK_LEFT )
		|| m_hge->Input_GetKeyState( HGEK_A ) )
	{
		m_last_facing = FACING_LEFT;
		m_speed.x = -BASE_MOVING_SPEED;
	}
	else {
		// stop moving if neither right nor left pressed, but preserve facing
		m_speed.x = 0;
	}

	float dy = m_speed.y * delta;
	float dx = m_speed.x * delta;

	bool solid_under_bottom_left = m_world->IsSolidAt( m_position.x1, m_position.y2+dy );
	bool solid_under_bottom_right = m_world->IsSolidAt( m_position.x2, m_position.y2+dy );
	bool standing_flag = solid_under_bottom_left || solid_under_bottom_right;

	if( standing_flag )
	{
		if (m_hge->Input_GetKeyState( HGEK_W )
			|| m_hge->Input_GetKeyState( HGEK_SPACE )
			|| m_hge->Input_GetKeyState( HGEK_UP ))
		{
			m_speed.y = -JumpSpeed();
		}
	}

	//-----------------------------------------------------------------
	// Get speed affected by gravity if our feet are not standing firm
	//-----------------------------------------------------------------

	// gravity drags us down, man (to the limit of MAX_FALL_SPEED)
	m_speed.y = std::min( m_speed.y + m_world->GravityAccel(), (float)MAX_FALL_SPEED );
	dy = m_speed.y * delta;

	// if we are standing firmly on feet
	if (m_speed.y > 0 )
	{
		// stop falling
		if( standing_flag )
		{
			m_speed.y = 0;
		}
		else {
			hgeRect future_pos = m_position;
			future_pos.y1 += m_speed.y * delta;
			future_pos.y2 += m_speed.y * delta;
			if( m_world->TestBlockCollisionAt( future_pos ) ) m_position = future_pos;
		}
	}
	else
	// jumping/flying up
	if( m_speed.y < 0) {
		// 2 is a magic number to allow sliding down/jumping through tight 1-block wide holes
		bool solid_above_top_left = m_world->IsSolidAt( m_position.x1+2, m_position.y1-dy );
		bool solid_above_top_right = m_world->IsSolidAt( m_position.x2-2, m_position.y1-dy );
		bool hitting_the_ceiling = solid_above_top_left || solid_above_top_right;
		if( hitting_the_ceiling ) {
			// stop flying, hit the ceiling
			m_speed.y = 0;
		} else {
			hgeRect future_pos = m_position;
			future_pos.y1 += m_speed.y * delta;
			future_pos.y2 += m_speed.y * delta;
			if( m_world->TestBlockCollisionAt( future_pos ) ) m_position = future_pos;
		}
	}

	// moving left, test if we hit the wall
	if( m_speed.x < 0) {
		bool solid_top_left = m_world->IsSolidAt( m_position.x1-dx, m_position.y1 );
		bool solid_bottom_left = m_world->IsSolidAt( m_position.x1-dx, m_position.y2 );
		bool hitting_left_wall = solid_top_left || solid_bottom_left;
		if( hitting_left_wall ) {
			// stop moving, hit the wall
			m_speed.x = 0;
		} else {
			hgeRect future_pos = m_position;
			future_pos.x1 += m_speed.x * delta;
			future_pos.x2 += m_speed.x * delta;
			if( m_world->TestBlockCollisionAt( future_pos ) ) m_position = future_pos;
		}
	}
	else
	// moving right, test if we hit the wall
	if( m_speed.x > 0) {
		bool solid_top_right = m_world->IsSolidAt( m_position.x2+dx, m_position.y1 );
		bool solid_bottom_right = m_world->IsSolidAt( m_position.x2+dx, m_position.y2 );
		bool hitting_right_wall = solid_top_right || solid_bottom_right;
		if( hitting_right_wall ) {
			// stop moving, hit the wall
			m_speed.x = 0;
		} else {
			hgeRect future_pos = m_position;
			future_pos.x1 += m_speed.x * delta;
			future_pos.x2 += m_speed.x * delta;
			if( m_world->TestBlockCollisionAt( future_pos ) ) m_position = future_pos;
		}
	}
}

void Player::MoveTo( float x, float y )
{
	m_position.Set(
		x, y,
		x + World::CELL_BOX_SIZE - 1.0f, 
		y + World::CELL_BOX_SIZE - 1.0f );
}