// $Id$

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "Tiles.h"
#include <SDL/SDL.h>
#include "Sprite.h"
#include <vector>

#define MAX_LAYERS 5

class Screen{
	private:
		char* tileMaps[MAX_LAYERS];		
		TilePallete* tilePallete;
        std::vector<Sprite*> sprites;
        Uint32 screenWidth;
		Uint32 screenHeight;
		Uint32 tileSize;        
	public:
		Screen(int screenWidth, int screenHeight, int tileSize);
		~Screen();
		void SetTileMap(int layer, char* tileMap);
		void SetTilePallete(TilePallete* tilePallete);
		void Render(SDL_Surface* dest);
        void Think();
        void AddSprite(Sprite* sprite);
        void RemoveSprite(Sprite* sprite);
        Tile* GetTileXY(int x, int y, int layer);
        Tile* GetTile(int x, int y, int layer);
        int GetTileSize();
};

#endif
