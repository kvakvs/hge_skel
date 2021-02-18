/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */
// Player class
// controls keyboard interaction and game rules
#include "player.h"
#include "world.h"
#include "game.h"
#include "creature.h"

#undef min

#include <algorithm>


Player::Player()
        : m_lives(INITIAL_LIVES_COUNT), m_position(0, 0, World::CELL_BOX_SIZE - 1, World::CELL_BOX_SIZE - 1),
          m_speed(0, 0), m_last_facing(FACING_RIGHT), m_is_dead(false), m_money(0) {
  m_sprite[0][0] = m_sprite_manager.GetSprite("textures/mario_r1.png");
  m_sprite[0][1] = m_sprite_manager.GetSprite("textures/mario_r2.png");
  m_sprite[1][0] = m_sprite_manager.GetSprite("textures/mario_l1.png");
  m_sprite[1][1] = m_sprite_manager.GetSprite("textures/mario_l2.png");

  // this will not create another HGE, instead we will get the global
  // unique HGE object which is already started
  m_hge = hgeCreate(HGE_VERSION);
}


Player::~Player() {
  delete m_sprite[0][0];
  delete m_sprite[0][1];
  delete m_sprite[1][0];
  delete m_sprite[1][1];
}


void Player::EnterWorld(World *w) {
  m_world = w;

  // we need a simple way to determine player starting position
  // we scan the world for '@' and use that as start
  hgeVector player_start = w->FindCellInWorld(World::WORLD_CELL_PLAYER_START);

  // found the @ character, set player position
  MoveTo(player_start.x, player_start.y);
}


hgeRect Player::GetScreenPosition() {
  hgeRect spos = m_position;
  spos.x1 -= m_world->m_camera_pos.x;
  spos.x2 -= m_world->m_camera_pos.x;
  spos.y1 -= m_world->m_camera_pos.y;
  spos.y2 -= m_world->m_camera_pos.y;
  return spos;
}


hgeSprite *Player::GetSprite() {
  // As an exercise for the reader:
  // Do not return animated frame, if horizontal speed is zero

  uint32_t milliseconds = GetTickCount();
  // we want frames to change every 333 msec from 0 to 1
  // total of 2 frames, hence the modulo of 2
  uint32_t f = (milliseconds / 333) % 2;

  return m_sprite[m_last_facing][f];
}


// MyGame pointer is only needed to access the font, otherwise we don't need it
void Player::Render(World *world) {
  MyGame *game = MyGame::m_game;

  // print with black shadow
  game->m_font->SetColor(ARGB(255, 0, 0, 0));
  game->m_font->printfb(2, 2, World::SCREEN_WIDTH, 40, HGETEXT_RIGHT, "Score: %d Lives: %d", m_money, m_lives);
  game->m_font->SetColor(ARGB(255, 255, 255, 255));
  game->m_font->printfb(0, 0, World::SCREEN_WIDTH, 40, HGETEXT_RIGHT, "Score: %d Lives: %d", m_money, m_lives);

  if (m_is_dead) {
    // test for >0 instead of >=0 because respawn requires at least 1 life to resurrect
    if (m_lives > 0) {
      game->m_font->printfb(0, World::SCREEN_HEIGHT / 2,
                            World::SCREEN_WIDTH, 40, HGETEXT_CENTER,
                            "You have died. Press ENTER to respawn.");
    } else {
      game->m_font->printfb(0, World::SCREEN_HEIGHT / 2,
                            World::SCREEN_WIDTH, 40, HGETEXT_CENTER,
                            "You have died. Game Over.");
    }
    // no rendering when we're dead
    // should display blood and all those guts around the death place or the corpse
    return;
  }

  hgeSprite *spr = GetSprite();
  if (!spr) return;

  hgeRect pos = GetScreenPosition();

  spr->Render(pos.x1, pos.y1);

  m_hge->Gfx_RenderLine(pos.x1, pos.y1, pos.x2, pos.y1, hgeColor32::WHITE());
  m_hge->Gfx_RenderLine(pos.x2, pos.y1, pos.x2, pos.y2, hgeColor32::WHITE());
  m_hge->Gfx_RenderLine(pos.x2, pos.y2, pos.x1, pos.y2, hgeColor32::WHITE());
  m_hge->Gfx_RenderLine(pos.x1, pos.y1, pos.x1, pos.y2, hgeColor32::WHITE());
}


void Player::Think() {
  if (m_is_dead) {
    // press ENTER when dead leads to respawn
    if (m_hge->Input_GetKeyState(HGEK_ENTER)) {
      Respawn();
    }
    return;
  }
  if (m_world->m_pause_flag) {
    // check for pause key to unpause
    return;
  }

  float delta = m_hge->Timer_GetDelta();

  if (m_hge->Input_GetKeyState(HGEK_RIGHT)
      || m_hge->Input_GetKeyState(HGEK_D)) {
    m_last_facing = FACING_RIGHT;
    m_speed.x = BASE_MOVING_SPEED;
  } else if (m_hge->Input_GetKeyState(HGEK_LEFT)
             || m_hge->Input_GetKeyState(HGEK_A)) {
    m_last_facing = FACING_LEFT;
    m_speed.x = -BASE_MOVING_SPEED;
  } else {
    // stop moving if neither right nor left pressed, but preserve facing
    m_speed.x = 0;
  }

  float dy = m_speed.y * delta;
  float dx = m_speed.x * delta;

  bool solid_under_bottom_left = m_world->IsSolidAtXY(m_position.x1, m_position.y2 + dy);
  bool solid_under_bottom_right = m_world->IsSolidAtXY(m_position.x2, m_position.y2 + dy);
  bool standing_flag = solid_under_bottom_left || solid_under_bottom_right;

  if (standing_flag) {
    if (m_hge->Input_GetKeyState(HGEK_W)
        || m_hge->Input_GetKeyState(HGEK_SPACE)
        || m_hge->Input_GetKeyState(HGEK_UP)) {
      m_speed.y = -JumpSpeed();
    }
  }

  //-----------------------------------------------------------------
  // Get speed affected by gravity if our feet are not standing firm
  //-----------------------------------------------------------------

  // gravity drags us down, man (to the limit of MAX_FALL_SPEED)
  m_speed.y = std::min(m_speed.y + m_world->GravityAccel(), (float) MAX_FALL_SPEED);
  dy = m_speed.y * delta;

  // if we are falling
  if (m_speed.y > 0) {
    // if we were falling but now we stand firmly on feet - stop falling
    if (standing_flag) {
      m_speed.y = 0;
    } else {
      hgeRect future_pos = m_position;
      future_pos.y1 += m_speed.y * delta;
      future_pos.y2 += m_speed.y * delta;
      if (m_world->TestBlockCollisionAt(future_pos)) m_position = future_pos;
    }
  } else
    // jumping/flying up
  if (m_speed.y < 0) {
    // 2 is a magic number to allow sliding down/jumping through tight 1-block wide holes
    bool solid_above_top_left = m_world->IsSolidAtXY(m_position.x1 + 2, m_position.y1 - dy);
    bool solid_above_top_right = m_world->IsSolidAtXY(m_position.x2 - 2, m_position.y1 - dy);
    bool hitting_the_ceiling = solid_above_top_left || solid_above_top_right;
    if (hitting_the_ceiling) {
      // stop flying, hit the ceiling
      m_speed.y = 0;
      // doesn't work: m_speed.y = -m_speed.y;
    } else {
      hgeRect future_pos = m_position;
      future_pos.y1 += m_speed.y * delta;
      future_pos.y2 += m_speed.y * delta;
      if (m_world->TestBlockCollisionAt(future_pos)) m_position = future_pos;
    }
  }

  // moving left, test if we hit the wall
  if (m_speed.x < 0) {
    bool solid_top_left = m_world->IsSolidAtXY(m_position.x1 - dx, m_position.y1);
    bool solid_bottom_left = m_world->IsSolidAtXY(m_position.x1 - dx, m_position.y2);
    bool hitting_left_wall = solid_top_left || solid_bottom_left;
    if (hitting_left_wall) {
      // stop moving, hit the wall
      m_speed.x = 0;
    } else {
      hgeRect future_pos = m_position;
      future_pos.x1 += m_speed.x * delta;
      future_pos.x2 += m_speed.x * delta;
      if (m_world->TestBlockCollisionAt(future_pos)) m_position = future_pos;
    }
  } else
    // moving right, test if we hit the wall
  if (m_speed.x > 0) {
    bool solid_top_right = m_world->IsSolidAtXY(m_position.x2 + dx, m_position.y1);
    bool solid_bottom_right = m_world->IsSolidAtXY(m_position.x2 + dx, m_position.y2);
    bool hitting_right_wall = solid_top_right || solid_bottom_right;
    if (hitting_right_wall) {
      // stop moving, hit the wall
      m_speed.x = 0;
    } else {
      hgeRect future_pos = m_position;
      future_pos.x1 += m_speed.x * delta;
      future_pos.x2 += m_speed.x * delta;
      if (m_world->TestBlockCollisionAt(future_pos)) m_position = future_pos;
    }
  }

  // if we fall below the world
  if (m_position.y1 >= m_world->m_world_height * World::CELL_BOX_SIZE) {
    Die();
    return;
  }

  // if we step at least 25% cell deep into the spikes, we die
  World::CellType left_foot = m_world->AtXY(m_position.x1, m_position.y2 - World::CELL_BOX_SIZE * 0.25f);
  World::CellType right_foot = m_world->AtXY(m_position.x2, m_position.y2 - World::CELL_BOX_SIZE * 0.25f);
  if (m_world->IsKillOnTouch(left_foot)
      || m_world->IsKillOnTouch(right_foot)) {
    Die();
    return;
  }

  World::object_list_t touching_objects;

  // shrink our box slightly for more realistic collisions
  hgeRect shrunken_position = m_position;
  shrunken_position.x1 += 12; // 20% from left
  shrunken_position.x2 -= 12; // 20% from right
  shrunken_position.y1 += 20; // 30% from top
  shrunken_position.y2 -= 10; // 15% from bottom

  m_world->FindIntersectingObjects(shrunken_position, touching_objects);

  // scan what we have found
  for (World::object_list_t::iterator i = touching_objects.begin(); i != touching_objects.end(); ++i) {
    // if it kills us - ouch
    if ((*i)->CanKillPlayerOnTouch()) {
      Die();
      return;
    } else {
      if (false == (*i)->TouchPlayer(this)) {
        // if touch player returned false - we consume the object
        m_world->RemoveObject(*i);
      }
    }
  }
  // we do not own the contents of 'touching_objects', so we don't delete
  // its contents and the list clears itself automatically
}


void Player::MoveTo(float x, float y) {
  m_position.Set(
          x, y,
          x + World::CELL_BOX_SIZE - 1.0f,
          y + World::CELL_BOX_SIZE - 1.0f);
}


void Player::Die() {
  // can't die twice in a moment
  if (m_is_dead) return;

  // TODO: invent the way for player to inform the gamestate or the world about
  // level game restart or scroll back to allow player to continue
  m_is_dead = true;
  m_world->OnPlayerDied();
}


void Player::Respawn() {
  m_lives--;
  if (m_lives >= 0) {
    // Here add actions to move player to start location,
    // to scroll window slightly back to allow playing, to give temporary immunity
    // to monsters and strip the player of all buffs
    m_is_dead = false;
    m_world->m_pause_flag = false;

    EnterWorld(m_world);
    m_world->m_camera_pos.x = 0; // reset the camera to the start
  } else {
    // out of spare lives - Game over
    // As an exercise for the reader: replace this game over text with a proper
    // gamestate which will show some animation or the Game Over text or show hi-scores
    MyGame::m_game->ShowMainMenuScreen();
  }
}
