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
	static MyGame * m_game;
    // Pointer to the HGE interface.
    // Helper classes require this to work.
    HGE * hge;

    // Some resource handles
    HEFFECT		snd;
    HTEXTURE	tex;
    hgeQuad		quad;

    // Pointers to the HGE objects we will use
    hgeGUI *	gui;
    hgeFont *	fnt;
    hgeSprite *	spr;

	GameState *		m_state;

public:
	MyGame();

    static bool FrameFunc();
    static bool RenderFunc();

    // this is called on start to start HGE and create things 
    // returns: true if all is fine, false if game could not start
    bool Startup();

    // this is called to end the world, when game is about to quit
    void Shutdown();
};
