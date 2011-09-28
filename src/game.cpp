/*
** Based on: hge_tut06 - Creating menus
** Haaf's Game Engine 1.8 -- Copyright (C) 2003-2007, Relish Games http://hge.relishgames.com
** Reworked by Dmytro Lytovchenko kvakvs@yandex.ru https://github.com/kvakvs/hge_skel
*/
// Copy the files "menu.wav", "font1.fnt", "font1.png",
// "bg.png" and "cursor.png" from the folder "precompiled"
// to the folder with executable file. Also copy hge.dll
// and bass.dll to the same folder.


#include "menuitem.h"
#include "game.h"
#include "game_state.h"

MyGame * MyGame::m_game = NULL;


MyGame::MyGame() : hge(NULL), gui(NULL), fnt(NULL), spr(NULL)
{
	// remember pointer to 'this' in globally visible variable so that everyone
	// else can reach it (not very good example of software design, but this simplifies a lot)
	m_game = this;

	m_state = new GameState_MainMenu();
}


bool MyGame::FrameFunc()
{
	// passing control to the current game state, so that game reacts to events
	// differently in different states
	return m_game->m_state->Think( m_game );	
}


bool MyGame::RenderFunc()
{
	m_game->m_state->Render( m_game );
    return false;
}


bool MyGame::Startup()
{
    hge = hgeCreate(HGE_VERSION);

    hge->System_SetState(HGE_LOGFILE, "hge_tut06.log");
    hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
    hge->System_SetState(HGE_TITLE, "HGE Tutorial 06 - Creating menus");
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_SCREENWIDTH, 800);
    hge->System_SetState(HGE_SCREENHEIGHT, 600);
    hge->System_SetState(HGE_SCREENBPP, 32);

    if( !hge->System_Initiate() ) return false;

    // Load sound and textures
    quad.tex=hge->Texture_Load("bg.png");
    tex=hge->Texture_Load("cursor.png");
    snd=hge->Effect_Load("menu.wav");
    if(!quad.tex || !tex || !snd)
    {
        // If one of the data files is not found, display
        // an error message and shutdown.
        MessageBox(NULL, "Can't load BG.PNG, CURSOR.PNG or MENU.WAV", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
        hge->System_Shutdown();
        hge->Release();
        return false;
    }

    // Set up the quad we will use for background animation
    quad.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

    for(int i=0;i<4;i++)
    {
        // Set up z-coordinate of vertices
        quad.v[i].z=0.5f;
        // Set up color. The format of DWORD col is 0xAARRGGBB
        quad.v[i].col=ARGB(0xFF, 0xFF, 0xFF, 0xFF);
    }

    quad.v[0].x=0; quad.v[0].y=0; 
    quad.v[1].x=800; quad.v[1].y=0; 
    quad.v[2].x=800; quad.v[2].y=600; 
    quad.v[3].x=0; quad.v[3].y=600; 


    // Load the font, create the cursor sprite
    fnt=new hgeFont("font1.fnt");
    spr=new hgeSprite(tex,0,0,32,32);

    // Create and initialize the GUI
    gui=new hgeGUI();

    gui->AddCtrl(new hgeGUIMenuItem(1,fnt,snd,400,200,0.0f,"Play"));
    gui->AddCtrl(new hgeGUIMenuItem(2,fnt,snd,400,240,0.1f,"Options"));
    gui->AddCtrl(new hgeGUIMenuItem(3,fnt,snd,400,280,0.2f,"Instructions"));
    gui->AddCtrl(new hgeGUIMenuItem(4,fnt,snd,400,320,0.3f,"Credits"));
    gui->AddCtrl(new hgeGUIMenuItem(5,fnt,snd,400,360,0.4f,"Exit"));

    gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
    gui->SetCursor(spr);
    gui->SetFocus(1);
    gui->Enter();

    return true; // all is fine
}

void MyGame::Shutdown()
{
    // Delete created objects and free loaded resources
    delete gui;
    delete fnt;
    delete spr;
    hge->Effect_Free(snd);
    hge->Texture_Free(tex);
    hge->Texture_Free(quad.tex);

    // Clean up and shutdown
    hge->System_Shutdown();
    hge->Release();
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    MyGame game;
    if( game.Startup() )
    {
        game.hge->System_Start(); // Let's rock now!
        game.Shutdown();
    }
}
