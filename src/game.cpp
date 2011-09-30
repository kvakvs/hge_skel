/*
** Based on: hge_tut06 - Creating menus
** Haaf's Game Engine 1.8 -- Copyright (C) 2003-2007, Relish Games http://hge.relishgames.com
** Reworked by Dmytro Lytovchenko kvakvs@yandex.ru https://github.com/kvakvs/hge_skel
*/
// Copy the files "menu.wav", "font1.fnt", "font1.png",
// "bg.png" and "cursor.png" from the folder "precompiled"
// to the folder with executable file. Also copy hge.dll
// and bass.dll to the same folder.


#include "game.h"
#include "game_state.h"

#include <stdio.h>

MyGame * MyGame::m_game = NULL;


MyGame::MyGame()
	: m_hge(NULL), m_font(NULL), m_mouse_cursor_sprite(NULL), m_state(NULL)
	, m_sound_enabled(true)
{
	// remember pointer to 'this' in globally visible variable so that everyone
	// else can reach it (not very good example of software design, but this simplifies a lot)
	m_game = this;

	// do nothing else here, unless it can run before HGE is started
}


bool MyGame::FrameFunc()
{
	// passing control to the current game state, so that game reacts to events
	// differently in different states
	return m_game->m_state->Think( m_game );
	// do not add more code here, all the action happens in GameState's child classes
}


bool MyGame::RenderFunc()
{
	// letting the current game state to take care of rendering (different render for
	// main menu, for game, and for game over screen)
	m_game->m_hge->Gfx_BeginScene();

	m_game->m_state->Render( m_game );

	// Draw framerate and time since previous frame
	char fps_text[64]; // some safer length so FPS text will fit
	_snprintf( fps_text, sizeof(fps_text)-1,
				"dt:%.3f\nFPS:%d",
				m_game->m_hge->Timer_GetDelta(), m_game->m_hge->Timer_GetFPS()
				);
	m_game->m_font->SetColor(ARGB(255,0,0,0)); // black
	m_game->m_font->printf( 7, 7, HGETEXT_LEFT, fps_text );
	m_game->m_font->SetColor(ARGB(255,255,255,255)); // white
	m_game->m_font->printf( 5, 5, HGETEXT_LEFT, fps_text );
    
	m_game->m_hge->Gfx_EndScene();

	// do not add more code here, all the action happens in GameState's child classes
	return false;
}


bool MyGame::Startup()
{
    m_hge = hgeCreate(HGE_VERSION);

	m_hge->System_SetState(HGE_SHOWSPLASH, false);
	m_hge->System_SetState(HGE_FPS, 60);

    m_hge->System_SetState(HGE_LOGFILE, "hgeskel.log");
    m_hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    m_hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
    m_hge->System_SetState(HGE_TITLE, "My Mighty Game of Doom!");
    m_hge->System_SetState(HGE_WINDOWED, true);
    m_hge->System_SetState(HGE_SCREENWIDTH, 800);
    m_hge->System_SetState(HGE_SCREENHEIGHT, 600);
    m_hge->System_SetState(HGE_SCREENBPP, 32);

    if( !m_hge->System_Initiate() ) return false;

    // Load sound and textures
    m_background_quad.tex = m_hge->Texture_Load("bg.png");
    m_mouse_cursor_tex = m_hge->Texture_Load("cursor.png");
    m_click_sound = m_hge->Effect_Load("menu.wav");

    if(!m_background_quad.tex || !m_mouse_cursor_tex || !m_click_sound)
    {
        // If one of the data files is not found, display
        // an error message and shutdown.
        MessageBox(NULL, "Can't load BG.PNG, CURSOR.PNG or MENU.WAV", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
        m_hge->System_Shutdown();
        m_hge->Release();
        return false;
    }

    // Set up the quad we will use for background animation
    m_background_quad.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

    for(int i=0;i<4;i++)
    {
        // Set up z-coordinate of vertices
        m_background_quad.v[i].z=0.5f;
        // Set up color. The format of DWORD col is 0xAARRGGBB
        m_background_quad.v[i].col=ARGB(0xFF, 0xFF, 0xFF, 0xFF);
    }

	m_background_quad.v[0].x=0;
	m_background_quad.v[0].y=0; 

	m_background_quad.v[1].x=800;
	m_background_quad.v[1].y=0;

	m_background_quad.v[2].x=800;
	m_background_quad.v[2].y=600; 

	m_background_quad.v[3].x=0;
	m_background_quad.v[3].y=600; 


	// Load the font, create the cursor sprite
	m_font = new hgeFont("font1.fnt");
	m_mouse_cursor_sprite = new hgeSprite(m_mouse_cursor_tex,0,0,32,32);

	m_state_options = new GameState_Options( this );
	m_state_mainmenu = new GameState_MainMenu( this );
	m_state_instructions = new GameState_Instructions( this );
	m_state_credits = new GameState_Credits( this );
	m_state_play = new GameState_Play();

	ShowMainMenuScreen();	

	return true; // all is fine
}


void MyGame::Shutdown()
{
	delete m_state_options;
	delete m_state_mainmenu;
	delete m_state_instructions;
	delete m_state_credits;
	delete m_state_play;

	// Delete created objects and free loaded resources
	delete m_font;
	delete m_mouse_cursor_sprite;

	m_hge->Effect_Free(m_click_sound);
	m_hge->Texture_Free(m_mouse_cursor_tex);
	m_hge->Texture_Free(m_background_quad.tex);

	// Clean up and shutdown
	m_hge->System_Shutdown();
	m_hge->Release();
}


void MyGame::ShowMainMenuScreen()
{
	m_state = m_state_mainmenu;
}


void MyGame::ShowOptionsScreen()
{
	m_state = m_state_options;
}


void MyGame::ShowInstructionsScreen()
{
	m_state = m_state_instructions;
}


void MyGame::ShowCreditsScreen()
{
	m_state = m_state_credits;
}


void MyGame::ShowPlayScreen()
{
	m_state = m_state_play;
	m_state_play->OnEnterState( this );
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    MyGame game;
    if( game.Startup() )
    {
        game.m_hge->System_Start(); // Let's rock now!
        game.Shutdown();
    }
}

