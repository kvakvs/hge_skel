/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */ 
// base Creature class
// represents a walking or stationary creature which is able to kill player
// on touch.
//
// TODO: more various creature behaviours like ability to shoot, jump, fly?
#include "sprite_manager.h"
#include <hgesprite.h>


SpriteManager::SpriteManager()
{
    // this will not create another HGE, instead we will get the global
    // unique HGE object which is already started
    m_hge = hgeCreate( HGE_VERSION );
}

hgeSprite * SpriteManager::GetSprite( const std::string & name )
{
    // attempt to find the requested texture in cache
    string_to_htexture_map_t::iterator iter = m_tex_map.find( name );
    HTEXTURE t = 0;

    if( iter != m_tex_map.end() ) {
        // use texture from cache
        t = iter->second;
    } else {
        // attempt to load the texture
        t = m_hge->Texture_Load( name.c_str() );
    }
    
    // if loading failed
    if( ! t ) return NULL;

    hgeSprite * spr = new hgeSprite(
                            t, 0.0f, 0.0f,
                            (float)m_hge->Texture_GetWidth(t), (float)m_hge->Texture_GetHeight(t)
                            );
    return spr;
}

SpriteManager::~SpriteManager()
{
    // to free all textures we might have loaded during this world
    for( string_to_htexture_map_t::iterator iter = m_tex_map.begin();
        iter != m_tex_map.end();
        ++iter )
    {
        m_hge->Texture_Free( iter->second );
    }

    m_hge->Release();
}