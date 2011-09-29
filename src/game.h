// to prevent multiple compiling of this header
#pragma once

// this is for standard integer types like uint32_t (very useful)
#include <stdint.h>

#include <hge.h>
#include <hgefont.h>

// telling the compiler that GameState class exists. Somewhere.
class GameState;

//
// Main application class, does the job to start up the graphics and the game
//
class MyGame
{
public:
	// Globally visible pointer to this class (not a brilliant example of
	// software design, but it simplifies things)
	static MyGame * m_game;

	// Pointer to the HGE interface.
    HGE * m_hge;

	// flag set to 0 or 1 to enable sound effects
	int m_sound_enabled;

    // Some resource handles
    HEFFECT		m_click_sound;
    HTEXTURE	m_mouse_cursor_tex;

	// menu background quad. As well we can use it for sky during the gameplay
    hgeQuad		m_background_quad;

    // Pointers to the HGE objects we will use
    hgeFont *	m_font;
    hgeSprite *	m_mouse_cursor_sprite;

	// This will change the way game renders and reacts to user input
	// We will set this to different states when user starts or ends playing
	GameState *	m_state;

	// premade game states to change to. Since its bad idea to delete them on the
	// fly while state change code is called from the running game state, we just
	// premake states and store them here.
	GameState *	m_state_options;		// shows options menu
	GameState *	m_state_mainmenu;		// shows main menu
	GameState *	m_state_instructions;	// shows 2 pages of instructions
	GameState *	m_state_credits;		// shows scrolling credits text
	GameState *	m_state_play;			// shows game world and allows to play


public:
	MyGame();

    static bool FrameFunc();
    static bool RenderFunc();

    // this is called on start to start HGE and create things 
    // returns: true if all is fine, false if game could not start
    bool Startup();

    // this is called to end the world, when game is about to quit
    void Shutdown();

	// switches game state to a new state
	void ShowMainMenuScreen();
	void ShowOptionsScreen();
	void ShowInstructionsScreen();
	void ShowCreditsScreen();
	void ShowPlayScreen();
};
