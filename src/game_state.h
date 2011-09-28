// to prevent multiple compiling of this header
#pragma once

// GameState class
// Represents current state of the application - is it in menu screen, or in the
// game world, or at the game over screen (showing high scores screen).

#include <hgegui.h>

// telling the compiler that MyGame class exists. Somewhere.
class MyGame;

class GameState
{
public:
	virtual ~GameState() {}

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
protected:
	hgeGUI *	m_gui;

public:
	GameState_MainMenu( MyGame * game );
	virtual ~GameState_MainMenu();

	void Render( MyGame * game );
	bool Think( MyGame * game );

	static const int MAINMENU_TOP_ITEM_Y = 200;
	static const int MAINMENU_ITEM_HEIGHT = 40;

	// numeric codes for GUI elements
	enum {
		MAINMENU_ELEMENT_NONE_SELECTED = 0,
		MAINMENU_ELEMENT_PLAY = 1,
		MAINMENU_ELEMENT_OPTIONS,
		MAINMENU_ELEMENT_INSTRUCTIONS,
		MAINMENU_ELEMENT_CREDITS,
		MAINMENU_ELEMENT_EXIT
	};
};


// This game state represents instructions screen and displays 2 pages of options
class GameState_Instructions: public virtual GameState
{
protected:
	hgeGUI		* m_gui;

public:
	GameState_Instructions( MyGame * game );
	virtual ~GameState_Instructions();

	enum {
		INSTRUC_ELEMENT_NONE_SELECTED = 0,
		INSTRUC_ELEMENT_PAGE1 = 1,
		INSTRUC_ELEMENT_PAGE2,
		INSTRUC_ELEMENT_EXIT
	};

	// fills m_gui with text labels containing lines of instructions page1 or page2
	void PopulateInstructionsPage( MyGame * game, const char * text[] );

	void Render( MyGame * game );
	bool Think( MyGame * game );
};


// This game state represents credits screen displaying slowly scrolling credits from
// outside the bottom of screen
class GameState_Credits: public virtual GameState
{
protected:
	hgeGUI		* m_gui;
	//int m_credits_row_count;

	// vertical coord to render credits, slowly decreases to show all text into screen
	float		m_draw_credits_y;

public:
	GameState_Credits( MyGame * game );
	virtual ~GameState_Credits();

	const static int CREDITS_ROW_HEIGHT = 40;	// row height (distance between rows) in pixels
	const static int CREDITS_SPEED = 25;		// pixels per second (roughly, bad estimation)

	enum {
		CREDITS_ELEMENT_NONE_SELECTED = 0,
		CREDITS_ELEMENT_EXIT = 1
	};

	// fills m_gui with text labels containing lines of credits
	// (Called once)
	//void PopulateCredits( MyGame * game );

	void Render( MyGame * game );
	bool Think( MyGame * game );
};


// This game state represents options screen with a single option: Sound, and Exit button
class GameState_Options: public virtual GameState
{
protected:
	hgeGUI		* m_gui;
public:
	GameState_Options( MyGame * game );
	virtual ~GameState_Options();

	static const int OPTIONS_TOP_ITEM_Y = 200;
	static const int OPTIONS_ITEM_HEIGHT = 40;

	enum {
		OPTIONS_ELEMENT_NONE_SELECTED = 0,
		OPTIONS_ELEMENT_SOUND = 1,
		OPTIONS_ELEMENT_EXIT
	};

	void Render( MyGame * game );
	bool Think( MyGame * game );
};
