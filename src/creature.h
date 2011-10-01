/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */ 
// to prevent multiple compiling of this header
#pragma once

#include <hgerect.h>
#include <hgesprite.h>

class World;
class Player;

// base Creature class
// represents a walking or stationary creature which is able to kill player
// on touch.
// P.S. for simplicity we will handle powerups same way as creatures, just they are harmless
//
// TODO: more various creature behaviours like ability to shoot, jump, fly?

class WorldObject
{
protected:
    World       * m_world;

public:
    hgeRect     m_box;

    WorldObject( World * owner, float x, float y );
    virtual ~WorldObject() {}

    virtual hgeSprite * GetSprite() { return NULL; }
    virtual void Render();

	virtual bool CanKillPlayerOnTouch() { return false; }
	
	// Perform interaction between creature/object and the player. Return false
	// if the object is consumed or destroyed by player
	virtual bool TouchPlayer( Player * pl ) { return true; }

	virtual void Think() {}
};


class WorldObject_Money: public virtual WorldObject
{
protected:
    hgeSprite * m_sprite[2];

public:
    WorldObject_Money(World * owner, float x, float y );
    virtual ~WorldObject_Money();

    virtual hgeSprite * GetSprite();
	virtual bool TouchPlayer( Player * pl );
};


class WorldObject_Enemy1: public virtual WorldObject
{
protected:
    hgeSprite * m_sprite[2][2];
	
	static const int MOVE_SPEED = 120;

    enum {
        FACING_RIGHT = 0,
        FACING_LEFT = 1
    };

	int m_facing;
public:
	WorldObject_Enemy1( World * owner, float x, float y );
	virtual ~WorldObject_Enemy1();
	virtual hgeSprite * GetSprite();
	virtual bool CanKillPlayerOnTouch() { return true; }

	virtual void Think();
};
