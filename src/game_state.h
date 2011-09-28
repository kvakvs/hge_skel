// to prevent multiple compiling of this header
#pragma once

// GameState class
// Represents current state of the application - is it in menu screen, or in the
// game world, or at the game over screen (showing high scores screen).

class GameState
{
public:
    // This is called periodically to draw the main menu, or the world, or the gameover screen
    // Please no logic in this code, only drawing
    virtual void Render() = 0;

    // This is called periodically to do the thinking (or nothing) and check keyboard etc.
    // Please no drawing in this code, only logic
    virtual void Think() = 0;
};
