// Player class
// controls keyboard interaction and game rules
#include "player.h"

hgeVector Player::GetPos()
{
	return hgeVector(m_position.x1, m_position.y1);
}


void Player::Die()
{
	// TODO: invent the way for player to inform the gamestate or the world about
	// level game restart or scroll back to allow player to continue
	m_lives--;
}