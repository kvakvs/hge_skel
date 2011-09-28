// to prevent multiple compiling of this header
#pragma once

#include <hge.h>
#include <hgefont.h>
#include <hgegui.h>

//
// Main application class, does the job to start up the graphics and the game
//
class MyGame
{
public:
    // Pointer to the HGE interface.
    // Helper classes require this to work.
    static HGE * hge;

    // Some resource handles
    static HEFFECT      snd;
    static HTEXTURE     tex;
    static hgeQuad      quad;

    // Pointers to the HGE objects we will use
    static hgeGUI       * gui;
    static hgeFont      * fnt;
    static hgeSprite    * spr;

    static bool FrameFunc();
    static bool RenderFunc();

    // this is called on start to start HGE and create things 
    // returns: true if all is fine, false if game could not start
    bool Startup();

    // this is called to end the world, when game is about to quit
    void Shutdown();
};
