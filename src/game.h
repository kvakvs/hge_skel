// to prevent multiple compiling of this header
#pragma once

#include <hge.h>
#include <hgefont.h>
#include <hgegui.h>

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

    // Some resource handles
    HEFFECT		m_click_sound;
    HTEXTURE	m_mouse_cursor_tex;

	// menu background quad. As well we can use it for sky during the gameplay
    hgeQuad		m_background_quad;

    // Pointers to the HGE objects we will use
    hgeGUI *	m_gui;
    hgeFont *	m_font;
    hgeSprite *	m_mouse_cursor_sprite;

	// This will change the way game renders and reacts to user input
	// We will set this to different states when user starts or ends playing
	GameState *	m_state;

	// premade game states to change to. Since its bad idea to delete them on the
	// fly while state change code is called from the running game state, we just
	// premake states and store them here.
	GameState *	m_state_options;
	GameState *	m_state_mainmenu;
	GameState *	m_state_instructions;
	GameState *	m_state_credits;


	// numeric codes for GUI elements
	enum {
		MAINMENU_ELEMENT_NONE_SELECTED = 0,
		MAINMENU_ELEMENT_PLAY = 1,
		MAINMENU_ELEMENT_OPTIONS,
		MAINMENU_ELEMENT_INSTRUCTIONS,
		MAINMENU_ELEMENT_CREDITS,
		MAINMENU_ELEMENT_EXIT
	};

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
};
