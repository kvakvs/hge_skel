/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */ 
// to prevent multiple compiling of this header
#pragma once

#include <hge.h>
#include <hgerect.h>
#include <hgevector.h>
#include <hgesprite.h>

#include "sprite_manager.h"


class World;
class MyGame;

// Player class
// controls keyboard interaction and game rules
class Player
{
protected:
    // Horizontal and vertical speeds of player.
    // You can use acceleration to affect this value when player presses buttons
    // to simulate inertia. Or you can write exact speeds when player presses buttons
    // to do instant turns and direction changes.
    hgeVector   m_speed;

    // pixels per second when player is moving (remember 64 is cell size)
    static const int BASE_MOVING_SPEED = 120;

    // Hack this value for infinite lives
    int     m_lives;
    // Hack this value for infinite gold, u jelly american treasury?
    int     m_money;

    // flag to render death animation/effects/blood splash when player died
    bool    m_is_dead;

    // world we have entered, must not be NULL when game is active
    World * m_world;

    enum {
        FACING_RIGHT = 0,
        FACING_LEFT = 1
    };

    // make box slightly smaller for the player
    //static const int PADDING_LEFT = 12;
    //static const int PADDING_RIGHT = 12;
    static const int MAX_FALL_SPEED = 300;

    // set this to 0 to render sprite facing right, 1 to render sprite facing left
    // used in GetSprite to return the correct facing
    int     m_last_facing;

    SpriteManager   m_sprite_manager;

    hgeSprite   * m_character_right[2][2];

    HGE * m_hge;

public:
    // A bounding box for character sprite used for rendering and collisions
    // for simplicity player has same size as world cells
    hgeRect     m_position;

    const static int INITIAL_LIVES_COUNT = 3;

public:
    Player();
    virtual ~Player();

    // player's vertical speed will be instantly set to this value on jump, and then
    // will be gradually reduced by World::GravityAccel every frame to simulate proper
    // jumping.
    // This can be modified to return different values depending if player picks up 
    // some bonus for higher jumps or eats some weakening mushroom
    float JumpSpeed() {
        return 420.0f;
    }

    //hgeVector GetPos();
    // returns position projected from world to screen coordinates
    hgeRect GetScreenPosition();

    // this one should reduce lives count
    virtual void Die();

    // Calculates current sprite to render, using last direction and time to find the frame
    virtual hgeSprite * GetSprite();

    // Draws character over the world, using World's camera to calculate positions
    // MyGame pointer is only needed to access the font, otherwise we don't need it
    virtual void Render( World * world );

    // called by gamestate when player enters the world and game begins
    virtual void EnterWorld( World * w );

    // Here check keyboard
    virtual void Think();

    void MoveTo( float x, float y );

    // Recovery after death
    virtual void Respawn();
};
