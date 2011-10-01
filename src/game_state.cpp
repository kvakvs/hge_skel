// GameState class
// Represents current state of the application - is it in menu screen, or in the
// game world, or at the game over screen (showing high scores screen).
#include "game_state.h"
#include "game.h"
#include "menuitem.h"
#include "world.h"
#include "player.h"

#include <hgeguictrls.h>

#include <math.h>

char * _opt_sound_text[2] = {
	"Sound: Disabled",
	"Sound: Enabled"
};


// array of strings of unknown length (NULL marks the end).
// You may delete few lines or add new lines before NULL if screen height allows.
const char * _page1_text[] = {
	"This is My Game of Doom",
	"You are to guide the character to the last",
	"sanctuary in this galaxy. or you die. That's about it. Yea.",
	"You will have to avoid obstacles, and kill stuff.",
	NULL
};


// arrayof strings of unknown length (NULL marks the end)
// You may delete few lines or add new lines before NULL if screen height allows.
const char * _page2_text[] = {
	"You can move with A/D/arrows, jump with W or space",
	"and throw deadly rocks with mouse button",
	"Game slowly scrolls with time, if you get caught",
	"between the end of screen and the wall, you will die.",
	NULL
};


// array of strings of unknown length (NULL marks the end)
// You may delete few lines or add new lines before NULL if screen height allows.
const char * _credits_text[] = {
	"This brilliant masterpiece",
	"was brought to you by",
	"-",
	"big boss Haaf, the master and",
	"the maker of HGE Engine",
	"-",
	"and your humble slave, kvakvs,",
	"who made this ugly game",
	"***",
	"tremble, mortals, before it",
	"and feel free to edit to your liking",
	NULL
};


// This should render main menu
void GameState_MainMenu::Render( MyGame * game )
{
    game->m_hge->Gfx_RenderQuad( &game->m_background_quad );
    m_gui->Render();

	// we are using GUI in this state, so it will draw the mouse cursor itself
	// no need to bother
}


void AnimateBackground( float dt, MyGame * game )
{
    static float t = 0.0f;
    float tx, ty;

    // Here we update our background animation
    t += dt;
    tx = 50*cosf(t/60);
    ty = 50*sinf(t/60);

    game->m_background_quad.v[0].tx=tx;
	game->m_background_quad.v[0].ty=ty;

    game->m_background_quad.v[1].tx=tx+800/64;
	game->m_background_quad.v[1].ty=ty;

    game->m_background_quad.v[2].tx=tx+800/64;
	game->m_background_quad.v[2].ty=ty+600/64;

    game->m_background_quad.v[3].tx=tx;
	game->m_background_quad.v[3].ty=ty+600/64;
}


bool GameState_MainMenu::Think( MyGame * game )
{
    float dt = game->m_hge->Timer_GetDelta();
    int id;
    static int lastid = MAINMENU_ELEMENT_NONE_SELECTED;

    // If ESCAPE was pressed, tell the GUI to finish
    if (game->m_hge->Input_GetKeyState(HGEK_ESCAPE)) {
		lastid = MAINMENU_ELEMENT_EXIT;
		m_gui->Leave();
	}
    
    // We update the GUI and take an action if
    // one of the menu items was selected
    id = m_gui->Update(dt);
    if(id == -1)
    {
        switch(lastid)
        {
            case MAINMENU_ELEMENT_PLAY:
				game->ShowPlayScreen();
				m_gui->Enter();
                break;
            
			case MAINMENU_ELEMENT_OPTIONS:
				game->ShowOptionsScreen();
				m_gui->Enter();
                break;

			case MAINMENU_ELEMENT_INSTRUCTIONS:
				game->ShowInstructionsScreen();
				m_gui->Enter();
                break;

			case MAINMENU_ELEMENT_CREDITS:
   				game->ShowCreditsScreen();
				m_gui->Enter();
                break;

            case MAINMENU_ELEMENT_EXIT:
				return true;
        }
    } else if(id) {
        lastid = id;
		m_gui->Leave();
    }

	AnimateBackground( dt, game );

    return false;
}



GameState_MainMenu::GameState_MainMenu( MyGame * game )
{
    // Create and initialize the GUI
    m_gui = new hgeGUI();

    m_gui->AddCtrl(new hgeGUIMenuItem(MAINMENU_ELEMENT_PLAY, game->m_font, game->m_click_sound,
						400, MAINMENU_TOP_ITEM_Y, 0.0f, "Play"));
    m_gui->AddCtrl(new hgeGUIMenuItem(MAINMENU_ELEMENT_OPTIONS, game->m_font, game->m_click_sound,
						400, MAINMENU_TOP_ITEM_Y+MAINMENU_ITEM_HEIGHT, 0.1f, "Options"));
    m_gui->AddCtrl(new hgeGUIMenuItem(MAINMENU_ELEMENT_INSTRUCTIONS, game->m_font, game->m_click_sound,
						400, MAINMENU_TOP_ITEM_Y+2*MAINMENU_ITEM_HEIGHT, 0.2f, "Instructions"));
    m_gui->AddCtrl(new hgeGUIMenuItem(MAINMENU_ELEMENT_CREDITS, game->m_font, game->m_click_sound,
						400, MAINMENU_TOP_ITEM_Y+3*MAINMENU_ITEM_HEIGHT, 0.3f, "Credits"));
    m_gui->AddCtrl(new hgeGUIMenuItem(MAINMENU_ELEMENT_EXIT, game->m_font, game->m_click_sound,
						400, MAINMENU_TOP_ITEM_Y+4*MAINMENU_ITEM_HEIGHT, 0.4f, "Exit"));

    m_gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
    m_gui->SetCursor( game->m_mouse_cursor_sprite );
    m_gui->SetFocus(MAINMENU_ELEMENT_PLAY);
    m_gui->Enter();
}


GameState_MainMenu::~GameState_MainMenu()
{
	delete m_gui;
}


void GameState_Instructions::Render( MyGame * game )
{
    game->m_hge->Gfx_RenderQuad( &game->m_background_quad );
    m_gui->Render();

	// we are using GUI in this state, so it will draw the mouse cursor itself
	// no need to bother
}


bool GameState_Instructions::Think( MyGame * game )
{
	float dt = game->m_hge->Timer_GetDelta();
    int id;
    static int lastid = INSTRUC_ELEMENT_NONE_SELECTED;

    // If ESCAPE was pressed, tell the GUI to finish
    if (game->m_hge->Input_GetKeyState(HGEK_ESCAPE)) {
		lastid = INSTRUC_ELEMENT_EXIT;
		m_gui->Leave();
	}
    
    // We update the GUI and take an action if
    // one of the menu items was selected
    id = m_gui->Update(dt);
    if(id == -1)
    {
        switch(lastid)
        {
            case INSTRUC_ELEMENT_PAGE1:
				PopulateInstructionsPage( game, _page1_text );
				m_gui->SetFocus(INSTRUC_ELEMENT_PAGE1);
                m_gui->Enter();
                break;
            
			case INSTRUC_ELEMENT_PAGE2:
				PopulateInstructionsPage( game, _page2_text );
				m_gui->SetFocus(INSTRUC_ELEMENT_PAGE2);
                m_gui->Enter();
                break;

            case INSTRUC_ELEMENT_EXIT:
				game->ShowMainMenuScreen();
// 				m_gui->SetFocus(INSTRUC_ELEMENT_EXIT);
//				m_gui->Enter();
                break;
        }
    } else if(id) {
        lastid = id;
		m_gui->Leave();
    }

	AnimateBackground( dt, game );

	return false;
}



GameState_Instructions::GameState_Instructions( MyGame * game )
{
    // Create and initialize the GUI
    m_gui = new hgeGUI();

    m_gui->AddCtrl(new hgeGUIMenuItem(INSTRUC_ELEMENT_PAGE1, game->m_font, game->m_click_sound,100,550,0.0f,"Page 1"));
    m_gui->AddCtrl(new hgeGUIMenuItem(INSTRUC_ELEMENT_PAGE2, game->m_font, game->m_click_sound,400,550,0.1f,"Page 2"));
    m_gui->AddCtrl(new hgeGUIMenuItem(INSTRUC_ELEMENT_EXIT, game->m_font, game->m_click_sound,700,550,0.2f,"Done"));

	PopulateInstructionsPage( game, _page1_text );
	
    m_gui->SetNavMode(HGEGUI_LEFTRIGHT | HGEGUI_CYCLED);
    m_gui->SetCursor( game->m_mouse_cursor_sprite );
    m_gui->SetFocus(INSTRUC_ELEMENT_PAGE1);
    m_gui->Enter();
}


void GameState_Instructions::PopulateInstructionsPage( MyGame * game, const char * text[] )
{
	// for each row in instructions text
	for( int textrow = 0; text[textrow]; textrow++ )
	{
		// attempt to retrieve and delete old text control
		if (m_gui->GetCtrl( 100+textrow )) {
			m_gui->DelCtrl( 100+textrow );
		}

		// add new text control with line from instructions
		hgeGUIText * c1 = new hgeGUIText(100+textrow, 0, 150.0f+40.0f*textrow, 800, 40, game->m_font);
		c1->SetMode( HGETEXT_CENTER );
		c1->SetText( text[textrow] );
		m_gui->AddCtrl(c1);
	}
}

GameState_Instructions::~GameState_Instructions()
{
	delete m_gui;
}


GameState_Credits::GameState_Credits( MyGame * game )
{
    // Create and initialize the GUI
    m_gui = new hgeGUI();
    m_gui->AddCtrl(new hgeGUIMenuItem(CREDITS_ELEMENT_EXIT, game->m_font, game->m_click_sound,700,550,0.0f,"Done"));

	m_draw_credits_y = 600; // bottom of screen, will decrease to slide up
	//PopulateCredits( game );
	
    //m_gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
    m_gui->SetCursor( game->m_mouse_cursor_sprite );
    //m_gui->SetFocus(1);
    m_gui->Enter();
}

GameState_Credits::~GameState_Credits()
{
	delete m_gui;
}


/*
// This shows how to fill array of hgeGUIText's with values
void GameState_Credits::PopulateCredits( MyGame * game )
{
	m_credits_row_count = 0;

	// for each row in credits text
	for( int textrow = 0; _credits_text[textrow]; textrow++ )
	{
		// attempt to retrieve and delete old text control
		if (m_gui->GetCtrl( 100+textrow )) {
			m_gui->DelCtrl( 100+textrow );
		}

		// add new text control with line from instructions
		// starting from under bottom of screen (600) and going deeper
		hgeGUIText * c1 = new hgeGUIText(100+textrow,
							0, 600.0f + 40.0f*textrow,
							800, 40, game->m_font);

		c1->SetMode( HGETEXT_CENTER );
		c1->SetText( _credits_text[textrow] );
		m_gui->AddCtrl(c1);
		m_credits_row_count++;
	}
}
*/


void GameState_Credits::Render( MyGame * game )
{
    game->m_hge->Gfx_RenderQuad( &game->m_background_quad );
    m_gui->Render();

	game->m_font->SetColor(0xFFFFFFFF);
	for( int textrow = 0; _credits_text[textrow]; textrow++ )
	{
		game->m_font->printf( 400, m_draw_credits_y + CREDITS_ROW_HEIGHT * textrow,
				HGETEXT_CENTER, _credits_text[textrow] );
	}

	// we are using GUI in this state, so it will draw the mouse cursor itself
	// no need to bother
}


bool GameState_Credits::Think( MyGame * game )
{
	float dt = game->m_hge->Timer_GetDelta();
    int id;
    static int lastid = CREDITS_ELEMENT_NONE_SELECTED;

    // If ESCAPE was pressed, tell the GUI to finish
    if (game->m_hge->Input_GetKeyState(HGEK_ESCAPE)) {
		lastid = CREDITS_ELEMENT_EXIT;
		m_gui->Leave();
	}
    
    // We update the GUI and take an action if
    // one of the menu items was selected
    id = m_gui->Update(dt);
    if(id == -1)
    {
        switch(lastid)
        {
            case CREDITS_ELEMENT_EXIT:
				game->ShowMainMenuScreen();
				//m_gui->SetFocus(CREDITS_ELEMENT_EXIT);
                m_gui->Enter();
                break;
        }
    } else if(id) {
        lastid = id;
		m_gui->Leave();
    }

	AnimateBackground( dt, game );

	// slide credits up
/*
	for( int i = 100; i < 100+m_credits_row_count; i++ )
	{
		hgeGUIText * t = (hgeGUIText *)m_gui->GetCtrl( i );
		if( ! t ) continue;

		// this one doesn't move anything, at all. Might be my mistake of course.
		m_gui->MoveCtrl( i, t->rect.x1, t->rect.y1 - dt * 20.0f );
	}
*/
	// this is rough speed estimation, should be properly reworked with millisecond timer
	m_draw_credits_y -= dt * CREDITS_SPEED;

	return false;
}


GameState_Options::GameState_Options( MyGame * game )
{
    // Create and initialize the GUI
    m_gui = new hgeGUI();

    m_gui->AddCtrl(new hgeGUIMenuItem(OPTIONS_ELEMENT_SOUND, game->m_font, game->m_click_sound,
						400, OPTIONS_TOP_ITEM_Y, 0.0f, _opt_sound_text[game->m_sound_enabled] ));
    m_gui->AddCtrl(new hgeGUIMenuItem(OPTIONS_ELEMENT_EXIT, game->m_font, game->m_click_sound,
						400, OPTIONS_TOP_ITEM_Y+OPTIONS_ITEM_HEIGHT, 0.1f, "Exit"));

    m_gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
    m_gui->SetCursor( game->m_mouse_cursor_sprite );
    m_gui->SetFocus( OPTIONS_ELEMENT_SOUND );
    m_gui->Enter();
}


GameState_Options::~GameState_Options()
{
	delete m_gui;
}


void GameState_Options::Render( MyGame * game )
{
    game->m_hge->Gfx_RenderQuad( &game->m_background_quad );
    m_gui->Render();
	// we are using GUI in this state, so it will draw the mouse cursor itself
	// no need to bother
}


bool GameState_Options::Think( MyGame * game )
{
	float dt = game->m_hge->Timer_GetDelta();
    int id;
    static int lastid = OPTIONS_ELEMENT_NONE_SELECTED;

    // If ESCAPE was pressed, tell the GUI to finish
    if (game->m_hge->Input_GetKeyState(HGEK_ESCAPE)) {
		lastid = OPTIONS_ELEMENT_EXIT;
		m_gui->Leave();
	}
    
    // We update the GUI and take an action if
    // one of the menu items was selected
	id = m_gui->Update(dt);
	if(id == -1)
	{
		switch(lastid)
		{
		case OPTIONS_ELEMENT_SOUND:
			game->m_sound_enabled = ! game->m_sound_enabled;
			
			// delete and re-add the menu element
			m_gui->DelCtrl(OPTIONS_ELEMENT_SOUND);
			m_gui->AddCtrl(new hgeGUIMenuItem(OPTIONS_ELEMENT_SOUND, game->m_font, game->m_click_sound,
						400, OPTIONS_TOP_ITEM_Y, 0.0f, _opt_sound_text[game->m_sound_enabled] ));

			m_gui->SetFocus( OPTIONS_ELEMENT_SOUND );
			m_gui->Enter();
			break;

		case OPTIONS_ELEMENT_EXIT:
			game->ShowMainMenuScreen();
			m_gui->Enter();
			break;
		}
	} else if(id) {
		lastid = id;
		m_gui->Leave();
	}

	AnimateBackground( dt, game );

	return false;
}


void GameState_Play::Render( MyGame * game )
{
	if( m_world ) m_world->Render();
	if( m_player ) m_player->Render( m_world );

	// as we are not using GUI in this state, we have to draw cursor ourself
	float mx, my;
	game->m_hge->Input_GetMousePos( & mx, & my );
	game->m_mouse_cursor_sprite->Render( mx, my );
}


bool GameState_Play::Think( MyGame * game )
{
	// give the World a chance to play its internal logic, like move monsters and animate things
	m_world->Think();
	// give the Player a chance to check keys, test powerup collisions etc
	m_player->Think();

	// TODO: Design a way to return events from the World::Think, like inform about player taking damage/dying

	if( m_world->Victory() ) {
	}

	return false;
}


void GameState_Play::OnEnterState( MyGame * game )
{
	delete m_player;
	delete m_world;
	m_player = new Player();
	m_world = new World( m_player, "level_01.txt" );
	
	m_player->EnterWorld( m_world );
}


GameState_Play::GameState_Play()
	: m_world(NULL), m_player(NULL)
{
}


GameState_Play::~GameState_Play()
{
	delete m_world;
	delete m_player;
}
