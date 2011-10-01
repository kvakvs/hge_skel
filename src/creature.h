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
    void Render();
};


class WorldObject_Money: public virtual WorldObject
{
protected:
    hgeSprite * m_sprite[2];

public:
    WorldObject_Money(World * owner, float x, float y );
    virtual ~WorldObject_Money();

    virtual hgeSprite * GetSprite();
};


class WorldObject_Creature: public virtual WorldObject
{
};
