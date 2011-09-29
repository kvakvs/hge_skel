// to prevent multiple compiling of this header
#pragma once

#include <string>
#include <vector>

// this is for standard integer types like uint32_t (very useful)
#include <stdint.h>

// this is for _ASSERT(_ASSERTE) macros which crash the program if error condition is detected
// showing message box with file and line of the problem (for debugging purposes)
#include <crtdbg.h> 

class Player;

// World class
// contains the world and its physical rules (collisions, gravity etc)
// basically its a level of the game, other levels can be different child of world 
// class and have e.g. negative gravity, or different set of monsters, or no light, etc.
//
// We create and load world using this class. World is responsible for its
// playing rules. World defines conditions when player wins.
class World
{
protected:
	// this is the player, World does not own the Character and will not delete it on world's end
	Player * m_player;
	
	// we define world cell as unsigned integer. But in future this may be changed to a
	// more complicated struct or class etc. to have more control over world structure
	typedef uint32_t CellType;

	// Rectangular array of the world lined up row after row, this is for memory storage
	// simplicity, to allow worlds of arbitrary size and to avoid more complicated memory
	// management - we let std::vector do the memory job. Use At() function to access cells
	// by (Row,Col) coord
	std::vector <CellType> m_world_cells;

	uint32_t m_world_width;
	uint32_t m_world_height;

public:
	// Loads the default world from the filename provided
	World( Player * plr, const std::string & filename );
	virtual ~World() {}

	// Returns a read/writable reference to a world cell. You can read and write to it 
	// like if it was a real array element
	CellType & At( uint32_t row, uint32_t col );

	// default world can never be "won", you have to inherit the world class
	// and override Victory function to define own rules when player wins
	virtual bool Victory() { return false; }

	// Animates the world, moves monsters, etc.
	virtual void Think();

	virtual void LoadWorld( const std::string & filename );

	// Returns pixels per second of free fall acceleration - adjust this to have things
	// fall harder or slower, make it negative to change gravity vector up
	// This can be modified to return different values depending if player picks up something
	// or flips a gravity switch (if you wish)
	virtual float GravityAccel() { return 20.0f; }
};


// Normal world for my game, normal rules apply
class WorldType1: virtual public World
{
public:
	bool Victory();
};
