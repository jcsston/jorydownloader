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

#include "Screen.h"
#include "common.h"

#define TILE_MAP_COORD(x,y,w) ((y*(w))+x)

Screen::Screen(int screenWidth, int screenHeight, int tileSize){
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->tileSize = tileSize;
	
	for(int i = 0; i < MAX_LAYERS; i++)
		this->tileMaps[i] = NULL;
	
	this->tilePallete = NULL;
}

Screen::~Screen(){
	std::vector<Sprite*>::iterator iter;
    for(iter = sprites.begin(); iter != sprites.end(); iter++){
        if(*iter)
            delete *iter;
            *iter = NULL;
    }
}

void Screen::SetTileMap(int layer, char* tileMap){
	if(layer < MAX_LAYERS && layer >= 0)
		this->tileMaps[layer] = tileMap;
}

void Screen::SetTilePallete(TilePallete* tilePallete){
	this->tilePallete = tilePallete;
}

void Screen::Render(SDL_Surface* dest){
	int rowTiles = screenWidth / tileSize;
	int colTiles = screenHeight / tileSize;
	Tile* tile = NULL;
	char* tileMap;
	SDL_Rect rcDest = { 0,0,0,0 };
	int tileID = 0;
    
    /* Blank Screen */
    //SDL_FillRect(dest, NULL, SDL_MapRGB(dest->format, 0, 0, 0));
    
    /* Render Map */
	for(int i = 0; i < MAX_LAYERS; i++){
		if(tileMaps[i] != NULL){
			tileMap = tileMaps[i];
			for(int y = 0; y < colTiles; y++){
				for(int x = 0; x < rowTiles; x++){
					tileID = tileMap[TILE_MAP_COORD(x,y,rowTiles)];
					if(tileID > 0){
						tile = tilePallete->GetTile(tileID);
                        if(!tile) return;
						rcDest.x = x * tileSize;
						rcDest.y = y * tileSize;
						Slock(dest);
						SDL_BlitSurface(tile->GetSurface(), NULL, dest, &rcDest);
						Sulock(dest);
					}
				}
			}
		}
	}
    
    /* Render Sprites */
    std::vector<Sprite*>::iterator iter;
    for(iter = sprites.begin(); iter != sprites.end(); iter++){
        if(*iter)
            (*iter)->Update(this);
            if((*iter)->x < 0 || (*iter)->x > screenWidth || (*iter)->y < 0 || (*iter)->y > screenHeight)
                printf("ERROR: Attempted to render sprite out of range.\n");
            else{
                rcDest.x = (*iter)->x;
                rcDest.y = (*iter)->y;
                Slock(dest);
			    SDL_BlitSurface((*iter)->GetSurface(), NULL, dest, &rcDest);
                Sulock(dest);
            }
    }
    
    SDL_Delay(33);
}

void Screen::AddSprite(Sprite* sprite){
    if(sprite)
    sprites.push_back(sprite);    
}

void Screen::RemoveSprite(Sprite* sprite){
    //FIXME
}

Tile* Screen::GetTileXY(int x, int y, int layer){
    int tileX = x/tileSize;
    int tileY = y/tileSize;
    char* tileMap = tileMaps[layer];
    if(!tileMap) return NULL;
    int tileID = tileMap[TILE_MAP_COORD(tileX,tileY,(screenWidth/tileSize))];    
    return tilePallete->GetTile(tileID);
}

Tile* Screen::GetTile(int x, int y, int layer){
    char* tileMap = tileMaps[layer];
    if(!tileMap) return NULL;
    int tileID = tileMap[TILE_MAP_COORD(x,y,(screenWidth/tileSize))];    
    return tilePallete->GetTile(tileID);
}

int Screen::GetTileSize(){
    return tileSize;    
}
