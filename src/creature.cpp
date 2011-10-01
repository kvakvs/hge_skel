/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */ 
// base Creature class
// represents a walking or stationary creature which is able to kill player
// on touch.
//
// TODO: more various creature behaviours like ability to shoot, jump, fly?
#include "creature.h"
#include "world.h"


WorldObject::WorldObject( World * owner, float x, float y ): m_world(owner)
{
    m_box.Set( x, y, x + World::CELL_BOX_SIZE - 1.0f, y + World::CELL_BOX_SIZE - 1.0f );
}

void WorldObject::Render()
{
    hgeSprite * spr = GetSprite();
    if( ! spr ) return; // can't draw a NULL sprite

    hgeRect box(    m_box.x1 - m_world->m_camera_pos.x,
                    m_box.y1 - m_world->m_camera_pos.y,
                    m_box.x2 - m_world->m_camera_pos.x,
                    m_box.y2 - m_world->m_camera_pos.y );

    spr->Render4V(  box.x1, box.y1,
                    box.x2, box.y1,
                    box.x2, box.y2,
                    box.x1, box.y2 );
}

WorldObject_Money::WorldObject_Money( World * owner, float x, float y ) : WorldObject( owner, x, y )
{
    m_sprite[0] = owner->m_sprite_manager.GetSprite( "textures/coin1.png" );
    m_sprite[1] = owner->m_sprite_manager.GetSprite( "textures/coin2.png" );
}


WorldObject_Money::~WorldObject_Money()
{
    delete m_sprite[0];
    delete m_sprite[1];
}

hgeSprite * WorldObject_Money::GetSprite()
{
    uint32_t milliseconds = GetTickCount();
    // we want frames to change every 250 msec from 0 to 1
    // total of 2 frames, hence the modulo of 2
    uint32_t f = (milliseconds / 250) % 2;

    return m_sprite[f];
}
