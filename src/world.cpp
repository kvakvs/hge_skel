// World class
// contains the world and its physical rules (collisions, gravity etc)
// basically its a level of the game, other levels can be different child of world 
// class and have e.g. negative gravity, or different set of monsters, or no light, etc.
#include "world.h"
#include "player.h"


World::World( Player * plr, const std::string & filename )
	: m_player(plr), m_world_width(0), m_world_height(0)
{
	LoadWorld( filename );
}


void World::LoadWorld( const std::string & filename )
{
	m_world_width = 0;
	m_world_height = 0;
}


void World::Think()
{

}


World::CellType & World::At( uint32_t row, uint32_t col )
{
	_ASSERTE( row >= 0 && row < m_world_height );
	_ASSERTE( col >= 0 && col < m_world_width );
	return m_world_cells[ row * m_world_width + col ];
}
