/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */
// to prevent multiple compiling of this header
#pragma once

#include <map>
#include <string>

#include <hge.h>

class hgeSprite;

// SpriteManager class
// Contains set of loaded textures, can load more textures
// hands out sprites using those textures
// frees textures it owns when manager is destroyed
class SpriteManager {
protected:
    // Textures collection, this will be filled on world loading and freed on world end
    typedef std::map<std::string, HTEXTURE> string_to_htexture_map_t;
    string_to_htexture_map_t m_tex_map;

    HGE *m_hge;

public:
    SpriteManager();

    virtual ~SpriteManager();

    hgeSprite *GetSprite(const std::string &name);
};
