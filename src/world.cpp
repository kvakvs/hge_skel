/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */ 
// World class
// contains the world and its physical rules (collisions, gravity etc)
// basically its a level of the game, other levels can be different child of world 
// class and have e.g. negative gravity, or different set of monsters, or no light, etc.
#include "world.h"
#include "player.h"
#include "creature.h"

#include <fstream>

#include <hgesprite.h>


World::World( Player * plr, const std::string & filename )
    : m_player(plr), m_world_width(0), m_world_height(0)
    , m_camera_pos(0, 0), m_pause_flag(false), m_level_goal_reached(false)
{
    LoadWorld( filename );

    // this will not create another HGE, instead we will get the global
    // unique HGE object which is already started
    m_hge = hgeCreate( HGE_VERSION );

    m_sprite_brick1 = m_sprite_manager.GetSprite("textures/brick1.png");
    m_sprite_sky = m_sprite_manager.GetSprite("textures/sky1.png");
    m_sprite_spikes = m_sprite_manager.GetSprite("textures/spikes.png");
	m_sprite_goal = m_sprite_manager.GetSprite("textures/goal_princess.png");
}


World::~World()
{
	delete m_sprite_goal;
    delete m_sprite_spikes;
    delete m_sprite_brick1;
    delete m_sprite_sky;

    // clear world contents 
    for( object_list_t::iterator i = m_objects.begin(); i != m_objects.end(); ++i ) {
        delete (*i);
    }
    m_objects.clear();

    m_hge->Release();
}


void World::LoadWorld( const std::string & filename )
{
    // Since we need to know exact world size to resize the m_world_cells array 
    // first we read the level file to determine the max length of all lines
    // second we read it again and put values in the m_world_cells
    std::ifstream f;
    f.open( filename );
    
    uint32_t    row = 0;
    size_t      max_line_width = 0;
    char        line_buf[MAX_WORLD_WIDTH];

    while( row < VISIBLE_ROWS || f.eof() || f.fail() )
    {
        f.getline( line_buf, sizeof line_buf );
        size_t line_width = strlen( line_buf );
        if( line_width > max_line_width ) {
            max_line_width = line_width;
        }
        row++;
    }
    f.close();

    // this can be actually more than 9 if your game can also scroll vertically
    // but for now we scroll horizontally only
    m_world_height = VISIBLE_ROWS;
    m_world_width = max_line_width;
    
    // resize the world
    m_world_cells.resize( m_world_height * m_world_width );

    // fill the world with empties (ASCII space, value 32)
    std::fill( m_world_cells.begin(), m_world_cells.end(), WORLD_CELL_EMPTY );

    // Now its time to properly load the world
    row = 0;
    f.open( filename );
    while( row < m_world_height || f.eof() || f.fail() )
    {
        f.getline( line_buf, sizeof line_buf );
        
        for( int col = 0; line_buf[col]; ++col ) {
            char contents = line_buf[col];
            if( contents == WORLD_CELL_MONEY ) {
                WorldObject * o = new WorldObject_Money( this, col * CELL_BOX_SIZE, row * CELL_BOX_SIZE );
                m_objects.push_back( o );
            }
            else if( contents == WORLD_CELL_ENEMY1 ) {
                WorldObject * o = new WorldObject_Enemy1( this, col * CELL_BOX_SIZE, row * CELL_BOX_SIZE );
                m_objects.push_back( o );
            }
			else if( contents == WORLD_CELL_GOAL ) {
                WorldObject * o = new WorldObject_LevelEnd( this, col * CELL_BOX_SIZE, row * CELL_BOX_SIZE );
                m_objects.push_back( o );
            }
            else {
                this->At( row, col ) = contents;
            }
        }

        row++;
    }
    f.close();
}


void World::Think()
{
    if( m_pause_flag ) return;

    float d = m_hge->Timer_GetDelta();
    m_camera_pos.x += d * 16;

    // test if player was pushed out of screen fully (to be pushed out partially is allowed)
    if ((m_player->m_position.x2 - m_camera_pos.x) < CELL_BOX_SIZE)
    {
        m_player->Die();
        OnPlayerDied();
    }

    for( object_list_t::iterator i = m_objects.begin(); i != m_objects.end(); ++i )
    {
        (*i)->Think();
    }
}

void World::OnPlayerDied()
{
    // play some animations and maybe restart the game
    m_pause_flag = true;
}


World::CellType & World::At( uint32_t row, uint32_t col )
{
    _ASSERTE( row >= 0 && row < m_world_height );
    _ASSERTE( col >= 0 && col < m_world_width );
    return m_world_cells[ row * m_world_width + col ];
}


void World::Render()
{
    // draw the sky, clearing is not needed anymore
    //m_hge->Gfx_Clear( ARGB(255, 80, 160, 190 ) );
    m_sprite_sky->Render( 0, 0 );

    // Assume world does not scroll vertically so we render 9 visible rows wholly, 
    // and carefully calculate visible columns to allow them to seamlessly slide as
    // the camera moves

    // calculate leftmost visible world column
    const uint32_t vis_column = (int)m_camera_pos.x / CELL_BOX_SIZE;

    // render one extra column incase if leftmost is partially visible, to avoid gaps
    // on the right side
    const uint32_t right_end_column = vis_column + VISIBLE_COLS + 1;
    

    // Now draw the visible window into the world
    for( uint32_t r = 0; r < VISIBLE_ROWS; ++r )
    {
        for( uint32_t c = vis_column; c <= right_end_column; ++c )
        {
            CellType cell_contents = this->At( r, c );
            switch( cell_contents )
            {
            case WORLD_CELL_WALL1:
                // find position in world and render it
                m_sprite_brick1->Render(
                                    c * CELL_BOX_SIZE - m_camera_pos.x,
                                    r * CELL_BOX_SIZE - m_camera_pos.y
                                    );
                break;
            case WORLD_CELL_SPIKES:
                // find position in world and render it
                m_sprite_spikes->Render(
                                    c * CELL_BOX_SIZE - m_camera_pos.x,
                                    r * CELL_BOX_SIZE - m_camera_pos.y
                                    );
                break;
            } // end switch
        } // end for columns
    } // end for rows

    // Now draw creatures and other stuff (no clipping or other optimizations
    // here we attempt to draw everything)
    for( object_list_t::iterator i = m_objects.begin(); i != m_objects.end(); ++i )
    {
        (*i)->Render();
    }
}


bool World::TestBlockCollisionAt( const hgeRect & rc )
{
    // we simplify calculation by only testing 4 corners
    // using as advantage the fact, that player has same size as world blocks
    if( IsSolidAtXY(rc.x1, rc.y1) ) return false;
    if( IsSolidAtXY(rc.x1, rc.y2) ) return false;
    if( IsSolidAtXY(rc.x2, rc.y1) ) return false;
    if( IsSolidAtXY(rc.x2, rc.y2) ) return false;

    return true;
}


bool World::IsSolid( CellType contents )
{
    return contents == WORLD_CELL_WALL1;
}


bool World::IsKillOnTouch( CellType contents )
{
    return contents == WORLD_CELL_SPIKES;
}


void World::FindIntersectingObjects( const hgeRect & rc, object_list_t & result )
{
	result.clear(); // just in case
	for( object_list_t::iterator i = m_objects.begin(); i != m_objects.end(); ++i )
	{
		if( (*i)->m_box.Intersect( & rc ) ) {
			result.push_back( *i );
		}
	}
}


void World::RemoveObject( WorldObject * o )
{
	// Linear search for the object. On a large world this can be speeded up if
	// you change to using std::map for fast key searches.
	for( object_list_t::iterator i = m_objects.begin(); i != m_objects.end(); ++i )
	{
		if (*i == o) {
			m_objects.erase( i );
			return;
		}
	}
}


hgeVector World::FindCellInWorld( CellType celltype )
{
    // we need a simple way to determine player starting position
    // we scan the world for '@' and use that as start
    for( uint32_t r = 0; r < m_world_height; ++r ) {
        for( uint32_t c = 0; c < m_world_width; ++c ) {
            if( this->At(r,c) == celltype ) {
                return hgeVector( c * CELL_BOX_SIZE, r * CELL_BOX_SIZE );
            }
        }
    }

	return hgeVector(-1, -1);
}


void World::GoalReached()
{
	m_level_goal_reached = true;
}

bool World::Victory()
{
	return m_level_goal_reached;
}