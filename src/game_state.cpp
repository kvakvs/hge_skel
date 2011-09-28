// GameState class
// Represents current state of the application - is it in menu screen, or in the
// game world, or at the game over screen (showing high scores screen).
#include "game_state.h"
#include "game.h"

#include <math.h>


// This should render main menu
void GameState_MainMenu::Render( MyGame * game )
{
    game->m_hge->Gfx_BeginScene();
    game->m_hge->Gfx_RenderQuad( &game->m_background_quad );
    game->m_gui->Render();

	// Draw framerate and time since previous frame
    game->m_font->SetColor(0xFFFFFFFF);

	game->m_font->printf( 5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d",
		game->m_hge->Timer_GetDelta(),
		game->m_hge->Timer_GetFPS() );

	// we are using GUI in this state, so it will draw the mouse cursor itself
	// no need to bother

    game->m_hge->Gfx_EndScene();
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
    static int lastid = MyGame::MAINMENU_ELEMENT_NONE_SELECTED;

    // If ESCAPE was pressed, tell the GUI to finish
    if (game->m_hge->Input_GetKeyState(HGEK_ESCAPE)) {
		lastid = MyGame::MAINMENU_ELEMENT_EXIT;
		game->m_gui->Leave();
	}
    
    // We update the GUI and take an action if
    // one of the menu items was selected
    id = game->m_gui->Update(dt);
    if(id == -1)
    {
        switch(lastid)
        {
            case MyGame::MAINMENU_ELEMENT_PLAY:
				game->m_gui->SetFocus(1);
                game->m_gui->Enter();
                break;
            
			case MyGame::MAINMENU_ELEMENT_OPTIONS:
				game->ShowOptionsScreen();
				game->m_gui->SetFocus(1);
                game->m_gui->Enter();
                break;

			case MyGame::MAINMENU_ELEMENT_INSTRUCTIONS:
				game->ShowInstructionsScreen();
				game->m_gui->SetFocus(1);
                game->m_gui->Enter();
                break;

			case MyGame::MAINMENU_ELEMENT_CREDITS:
   				game->ShowCreditsScreen();
				game->m_gui->SetFocus(1);
                game->m_gui->Enter();
                break;

            case MyGame::MAINMENU_ELEMENT_EXIT:
				return true;
        }
    } else if(id) {
        lastid = id;
		game->m_gui->Leave();
    }

	AnimateBackground( dt, game );

    return false;
}



void GameState_Instructions::Render( MyGame * game )
{
    game->m_hge->Gfx_BeginScene();
    game->m_hge->Gfx_RenderQuad( &game->m_background_quad );

	// draw instructions
    game->m_font->SetColor(0xFFFFFFFF);

	game->m_font->printf( 400, 150, HGETEXT_CENTER, "This is My Game of Doom" );
	game->m_font->printf( 400, 180, HGETEXT_CENTER, "You are to guide the character" );
	game->m_font->printf( 400, 210, HGETEXT_CENTER, "to the last sanctuary in this galaxy" );
	game->m_font->printf( 400, 240, HGETEXT_CENTER, "or you die. That's about it. Yea." );

	game->m_font->printf( 400, 300, HGETEXT_CENTER, "Click or press SPACE to return" );

	// as we are not using GUI in this state, we have to draw cursor ourself
	float mx, my;
	game->m_hge->Input_GetMousePos( & mx, & my );
	game->m_mouse_cursor_sprite->Render( mx, my );

    game->m_hge->Gfx_EndScene();
}


bool GameState_Instructions::Think( MyGame * game )
{
	float dt = game->m_hge->Timer_GetDelta();

	if( game->m_hge->Input_GetKeyState(HGEK_SPACE) 
		|| game->m_hge->Input_GetKeyState(HGEK_LBUTTON) )
	{
		game->ShowMainMenuScreen();
		return false;
	}

	AnimateBackground( dt, game );

	return false;
}
