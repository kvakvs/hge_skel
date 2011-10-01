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
class SpriteManager
{
protected:
	// Textures collection, this will be filled on world loading and freed on world end
	typedef std::map <std::string, HTEXTURE> string_to_htexture_map_t;
	string_to_htexture_map_t	m_tex_map;

	HGE * m_hge;

public:
	SpriteManager();
	virtual ~SpriteManager();

	hgeSprite * GetSprite( const std::string & name );
};
