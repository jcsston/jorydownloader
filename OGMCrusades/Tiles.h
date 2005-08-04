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

#ifndef __TILES_H__
#define __TILES_H__

#include <SDL/SDL.h>
#include <string>
#include <vector>

#ifdef _MSC_VER
#define snprintf  _snprintf
#endif

class Tile{
	private:
		SDL_Surface* surface;
		int tileSize;        
	public:
        bool isWalkable;
		Tile(bool walkable = true);
		virtual ~Tile();
		virtual int LoadImage(std::string fileName);
		virtual SDL_Surface* GetSurface();
        virtual bool IsWalkable();
};

class TileCache{
	protected:
		std::vector<Tile*> tiles;		
	public:
		TileCache();
		~TileCache();
		/* Gets a tile by ID */
		Tile* GetTile(int tileID);
		/* Loads a tile and returns the tile ID */
		int LoadTile(std::string fileName);
		int AddTile(Tile* t);
		/* Clears the cache */
		void Clear();
};

class TilePallete{
	protected:
		std::vector<int> tileIDs;
		TileCache* cache;
	public:
		TilePallete(TileCache* cache);
		~TilePallete();
		/* Gets a tile by pallete ID.  Retrieves it from the cache. */
		Tile* GetTile(int tileID);
		/* Loads a Tile and returns it's pallete ID */
		int LoadTile(std::string fileName);
		int AddTile(Tile* t);
};

class AnimatedTile: public Tile{
	private:
		SDL_Surface** surfaces;
		int numSurfaces;
		int tileSize;
		Uint32 tickTime;
		Uint32 nextTime;
		int currentSurface;
        bool isWalkable;
	public:
		AnimatedTile(bool isWalkable = true);
		~AnimatedTile();
		virtual int LoadImage(std::string fileName);
		virtual int LoadImageSequence(std::string fileNamePattern, int numImages);
		virtual void SetTickLength(int timeMS);
		virtual SDL_Surface* GetSurface();
        virtual bool IsWalkable();
};

#endif
