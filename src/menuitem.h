/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */ 
// to prevent multiple compiling of this header
#pragma once

/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Tutorial 06 - Creating menus
*/

// In menuitem.cpp/h we define the
// behaviour of our custom GUI control

#include <hge.h>
#include <hgefont.h>
#include <hgegui.h>
#include <hgecolor.h>


class hgeGUIMenuItem : public hgeGUIObject
{
public:
    hgeGUIMenuItem(int id, hgeFont *fnt, HEFFECT snd, float x, float y, float delay, char *title);

    virtual void    Render();
    virtual void    Update(float dt);

    virtual void    Enter();
    virtual void    Leave();
    virtual bool    IsDone();
    virtual void    Focus(bool bFocused);
    virtual void    MouseOver(bool bOver);

    virtual bool    MouseLButton(bool bDown);
    virtual bool    KeyClick(int key, int chr);

private:
    hgeFont     *fnt;
    HEFFECT     snd;
    float       delay;
    char        *title;

    hgeColor    scolor, dcolor, scolor2, dcolor2, sshadow, dshadow;
    hgeColor    color, shadow;
    float       soffset, doffset, offset;
    float       timer, timer2;
};
