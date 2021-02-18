/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */
// to prevent multiple compiling of this header
#pragma once

#include <string>
#include <vector>
#include <list>

#include <hge.h>
#include <hgerect.h>
#include <hgevector.h>

// this is for standard integer types like uint32_t (very useful)
#include <cstdint>

// this is for _ASSERT(_ASSERTE) macros which crash the program if error condition is detected
// showing message box with file and line of the problem (for debugging purposes)
#include <crtdbg.h>

#include "sprite_manager.h"

class Player;

class hgeSprite;

class WorldObject;


// World class
// contains the world and its physical rules (collisions, gravity etc)
// basically its a level of the game, other levels can be different child of world 
// class and have e.g. negative gravity, or different set of monsters, or no light, etc.
//
// We create and load world using this class. World is responsible for its
// playing rules. World defines conditions when player wins.
class World {
public:
    // we define world cell as unsigned integer. But in future this may be changed to a
    // more complicated struct or class etc. to have more control over world structure
    typedef uint32_t CellType;
    typedef std::list<WorldObject *> object_list_t;

protected:
    // this is the player, World does not own the Character and will not delete it on world's end
    Player *m_player;

    // Rectangular array of the world lined up row after row, this is for memory storage
    // simplicity, to allow worlds of arbitrary size and to avoid more complicated memory
    // management - we let std::vector do the memory job. Use At() function to access cells
    // by (Row,Col) coord
    std::vector<CellType> m_world_cells;

    // Leaving this as exercise for the reader - to organize sprites better
    hgeSprite *m_sprite_brick1;
    hgeSprite *m_sprite_sky;
    hgeSprite *m_sprite_spikes;
    hgeSprite *m_sprite_goal; // level end

    // this also delimits max world width in cells, increase if your world grows wider
    // actual world will be as wide as the widest line in your level file
    // -- this const contains no secret, moving it to public section
    static const int MAX_WORLD_WIDTH = 4096;

    // this contains creatures and objects and projectiles
    object_list_t m_objects;

public:
    // is the game running or paused (pause to animate player death for example)
    bool m_pause_flag;
    // marks the victory. If this is set, level should change
    bool m_level_goal_reached;

    HGE *m_hge;

    // This stores all loaded textures for the world, including blocks and creatures, but not player
    SpriteManager m_sprite_manager;

    // This represents camera position, actually this is top-left corner of the visible
    // window to the game world. Camera slowly moves right increasing X
    // making the world "slide" left
    // -- made this public to access it from the Player when rendering player
    hgeVector m_camera_pos;

    // pixel size of world cells
    static const int CELL_BOX_SIZE = 64;

    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 600;

    // world visible height will be 9 rows
    // roughly 600 pixels screen height divided by 64 pix cell size
    // this can be actually more than 9 if your game can also scroll vertically, but you
    // will be able to draw only currently visible 9 rows, but this also will require
    // writing code to auto detect map height in the input file
    static const int VISIBLE_ROWS = SCREEN_HEIGHT / CELL_BOX_SIZE;

    static const int VISIBLE_COLS = SCREEN_WIDTH / CELL_BOX_SIZE;

    uint32_t m_world_width;
    uint32_t m_world_height;

    // characters used in map file to represent various world cells
    enum {
        WORLD_CELL_EMPTY = ' ',
        WORLD_CELL_PLAYER_START = '@',
        WORLD_CELL_WALL1 = '#',
        WORLD_CELL_MONEY = '$',
        WORLD_CELL_SPIKES = '^',
        WORLD_CELL_GOAL = '!',                // a princess or other goal, that marks end of the level
        WORLD_CELL_ENEMY1 = 'A'                // enemy type 1 - edit this to add more enemies
    };

public:
    // Loads the default world from the filename provided
    World(Player *plr, const std::string &filename);

    virtual ~World();

    // default world can never be "won", you have to inherit the world class
    // and override Victory function to define own rules when player wins
    virtual bool Victory();

    // Animates the world, moves monsters, etc.
    virtual void Think();

    virtual void LoadWorld(const std::string &filename);

    // Returns pixels per second of free fall acceleration - adjust this to have things
    // fall harder or slower, make it negative to change gravity vector up
    // This can be modified to return different values depending if player picks up something
    // or flips a gravity switch (if you wish)
    virtual float GravityAccel() { return 10.0f; }

    // Draws the world in its current state. This function must not call or perform
    // any other game logic, only drawing
    virtual void Render();

    // Returns a read/writable reference to a world cell. You can read and write to it 
    // like if it was a real array element
    CellType &At(uint32_t row, uint32_t col);

    // return not reference but value. Since we can return non existing values beyond the
    // world limits, we cannot return an actual cell, so read only there we go
    inline CellType AtXY(float x, float y) {
      // always solid for ahead of the visible screen
      if (x > m_camera_pos.x + SCREEN_WIDTH) return WORLD_CELL_WALL1;
      // always not solid below the world
      if (y >= m_world_height * CELL_BOX_SIZE) return WORLD_CELL_EMPTY;
      // always not solid above the world
      if (y < 0) return WORLD_CELL_EMPTY;

      return At((uint32_t) (y / CELL_BOX_SIZE), (uint32_t) (x / CELL_BOX_SIZE));
    }

    // tests if rect rc is allowed to be in the world and does not collide a solid block
    virtual bool TestBlockCollisionAt(const hgeRect &rc);

    // tests if cell type is solid or pass-through
    inline bool IsSolidAtXY(float x, float y) {
      // always solid for ahead of the visible screen
      if (x > m_camera_pos.x + SCREEN_WIDTH) return true;
      // always not solid below the world
      if (y >= m_world_height * CELL_BOX_SIZE) return false;
      // always not solid above the world
      if (y < 0) return false;

      return IsSolid(
              At((uint32_t) (y / CELL_BOX_SIZE), (uint32_t) (x / CELL_BOX_SIZE))
      );
    }

    virtual bool IsSolid(CellType contents);

    virtual bool IsKillOnTouch(CellType contents);

    virtual void OnPlayerDied();

    // Scan world objects and find those whose bounding boxes touch the given box 'rc'
    // Result is placed in the 'result' list, can be 0 or 1 or multiple objects
    void FindIntersectingObjects(const hgeRect &rc, object_list_t &result);

    // Removes given object from the m_objects, make sure you are not iterating over
    // the m_objects list at this moment, or game will crash
    void RemoveObject(WorldObject *o);

    // Scans the world searching for value 'c' returns its world coordinates
    // used to find player starting position and finish cell
    hgeVector FindCellInWorld(CellType c);

    // This marks end of the level. Inform the calling GameState_Play about level end
    // and let it delete this world and move to next level
    virtual void GoalReached();
};


// Normal world for my game, normal rules apply
class WorldType1 : virtual public World {
public:
    bool Victory();
};
