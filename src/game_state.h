// to prevent multiple compiling of this header
#pragma once

// GameState class
// Represents current state of the application - is it in menu screen, or in the
// game world, or at the game over screen (showing high scores screen).

// telling the compiler that MyGame class exists. Somewhere.
class MyGame;

class GameState
{
public:
    // This is called periodically to draw the main menu, or the world, or the gameover screen
    // Please no logic in this code, only drawing
	// Render returns void. Can't stop the game from here, unlike HGE's renderfunc.
    virtual void Render( MyGame * game ) = 0;

    // This is called periodically to do the thinking (or nothing) and check keyboard etc.
    // Please no drawing in this code, only logic
	// Think returns bool like HGE framefunc does, returning true will stop the game
    virtual bool Think( MyGame * game ) = 0;
};


// This game state represents main menu and reacts on keypresses like a main menu would
class GameState_MainMenu: public virtual GameState
{
public:
	void Render( MyGame * game );
	bool Think( MyGame * game );
};


// This game state represents instructions screen and only draws text until click or key pressed
class GameState_Instructions: public virtual GameState
{
public:
	void Render( MyGame * game );
	bool Think( MyGame * game );
};


// This game state represents instructions screen and only draws text until click or key pressed
class GameState_Credits: public virtual GameState
{
public:
	void Render( MyGame * game ) {}
	bool Think( MyGame * game ) { return false; }
};


// This game state represents instructions screen and only draws text until click or key pressed
class GameState_Options: public virtual GameState
{
public:
	void Render( MyGame * game ) {}
	bool Think( MyGame * game ) { return false; }
};
