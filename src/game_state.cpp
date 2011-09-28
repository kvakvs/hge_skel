// GameState class
// Represents current state of the application - is it in menu screen, or in the
// game world, or at the game over screen (showing high scores screen).
#include "game_state.h"
#include "game.h"

#include <math.h>


// This should render main menu
void GameState_MainMenu::Render( MyGame * game )
{
    game->hge->Gfx_BeginScene();
    game->hge->Gfx_RenderQuad( &game->quad );
    game->gui->Render();

	// Draw framerate and time since previous frame
    game->fnt->SetColor(0xFFFFFFFF);
    game->fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d",
		game->hge->Timer_GetDelta(),
		game->hge->Timer_GetFPS());

    game->hge->Gfx_EndScene();
}


bool GameState_MainMenu::Think( MyGame * game )
{
    float dt = game->hge->Timer_GetDelta();
    static float t = 0.0f;
    float tx, ty;
    int id;
    static int lastid = 0;

    // If ESCAPE was pressed, tell the GUI to finish
    if (game->hge->Input_GetKeyState(HGEK_ESCAPE)) {
		lastid = 5;
		game->gui->Leave();
	}
    
    // We update the GUI and take an action if
    // one of the menu items was selected
    id = game->gui->Update(dt);
    if(id == -1)
    {
        switch(lastid)
        {
            case 1:
            case 2:
            case 3:
            case 4:
                game->gui->SetFocus(1);
                game->gui->Enter();
                break;

            case 5: return true;
        }
    } else if(id) {
        lastid = id;
		game->gui->Leave();
    }

    // Here we update our background animation
    t += dt;
    tx = 50*cosf(t/60);
    ty = 50*sinf(t/60);

    game->quad.v[0].tx=tx;
	game->quad.v[0].ty=ty;

    game->quad.v[1].tx=tx+800/64;
	game->quad.v[1].ty=ty;

    game->quad.v[2].tx=tx+800/64;
	game->quad.v[2].ty=ty+600/64;

    game->quad.v[3].tx=tx;
	game->quad.v[3].ty=ty+600/64;

    return false;
}