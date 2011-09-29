// World class
// contains the world and its physical rules (collisions, gravity etc)
// basically its a level of the game, other levels can be different child of world 
// class and have e.g. negative gravity, or different set of monsters, or no light, etc.
#include "world.h"
#include "player.h"

#include <fstream>


World::World( Player * plr, const std::string & filename )
	: m_player(plr), m_world_width(0), m_world_height(0)
{
	LoadWorld( filename );
}


void World::LoadWorld( const std::string & filename )
{
	// Since we need to know exact world size to resize the m_world_cells array 
	// first we read the level file to determine the max length of all lines
	// second we read it again and put values in the m_world_cells
	std::ifstream f;
	f.open( filename );
	
	uint32_t	line = 0;
	size_t		max_line_width = 0;
	char		line_buf[MAX_WORLD_WIDTH];

	while( line < WORLD_HEIGHT || f.eof() || f.fail() )
	{
		f.getline( line_buf, sizeof line_buf );
		size_t line_width = strlen( line_buf );
		if( line_width > max_line_width ) {
			max_line_width = line_width;
		}
		line++;
	}
	f.close();

	// this can be actually more than 9 if your game can also scroll vertically
	// but for now we scroll horizontally only
	m_world_height = WORLD_HEIGHT;
	m_world_width = max_line_width;
	
	// resize the world
	m_world_cells.resize( m_world_height * m_world_width );

	// fill the world with empties (ASCII space, value 32)
	std::fill( m_world_cells.begin(), m_world_cells.end(), WORLD_CELL_EMPTY );

	// Now its time to properly load the world
	line = 0;
	f.open( filename );
	while( line < m_world_height || f.eof() || f.fail() )
	{
		f.getline( line_buf, sizeof line_buf );
		
		for( int col = 0; line_buf[col]; ++col ) {
			this->At( line, col ) = line_buf[col];
		}

		line++;
	}
	f.close();
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
