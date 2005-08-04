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

#include "Tiles.h"
#include <assert.h>
#include <SDL/SDL_image.h>

Tile::Tile(bool walkable):surface(NULL),tileSize(0),isWalkable(walkable){
}

Tile::~Tile(){
	if(surface){
		SDL_FreeSurface(surface);
		surface = NULL;
	}
	//printf("Deleted tile!\n");
}

int Tile::LoadImage(std::string fileName){
	surface = IMG_Load(fileName.c_str());
	
	if(!surface) return -1;
	
	return 0;
}

SDL_Surface* Tile::GetSurface(){
	return surface;
}

bool Tile::IsWalkable(){
    return isWalkable;
}

TileCache::TileCache(){
	
}

TileCache::~TileCache(){
	Clear();
}

Tile* TileCache::GetTile(int tileID){
	if((unsigned int)tileID > tiles.size())
        return NULL;
    
	return tiles.at(tileID);	
}

int TileCache::LoadTile(std::string fileName){
	Tile* newTile = new Tile();
	if(newTile->LoadImage(fileName)){
		printf("Failed to load tile %s\n", fileName.c_str());
		return -1;
	}
	tiles.push_back(newTile);
	return tiles.size() - 1;
}

int TileCache::AddTile(Tile* tile){
	tiles.push_back(tile);
	return tiles.size() -1;
}

void TileCache::Clear(){
	std::vector<Tile*>::iterator iter;
	
	for(iter = tiles.begin(); iter != tiles.end(); iter++)
		if(*iter) delete *iter;
	
	tiles.clear();
}


TilePallete::TilePallete(TileCache* cache){
	this->cache = cache;
}

TilePallete::~TilePallete(){
	
}

Tile* TilePallete::GetTile(int tileID){
	if((unsigned int)tileID > tileIDs.size() || tileID <= 0)
        return NULL;
    
	int cacheTileID = tileIDs.at(tileID-1);
	return cache->GetTile(cacheTileID);
}

int TilePallete::LoadTile(std::string fileName){
	int cacheTileID = cache->LoadTile(fileName);
	if(cacheTileID != -1){
		tileIDs.push_back(cacheTileID);
		return tileIDs.size();
	}
    return -1;
}

int TilePallete::AddTile(Tile* t){
	int cacheTileID = cache->AddTile(t);
	if(cacheTileID != -1){
		tileIDs.push_back(cacheTileID);
		return tileIDs.size();
	}
    return -1;
}

AnimatedTile::AnimatedTile(bool walkable):surfaces(NULL),numSurfaces(0),tileSize(0),tickTime(0),nextTime(0),currentSurface(0),isWalkable(walkable){
}

AnimatedTile::~AnimatedTile(){
	for(int i = 0; i < numSurfaces; i++){
		if(surfaces[i]){
			SDL_FreeSurface(surfaces[i]);
			surfaces[i] = NULL;
		}
	}
	delete surfaces;
	surfaces = NULL;
	//printf("Deleted tile!\n");
}

int AnimatedTile::LoadImage(std::string fileName){
	return -1;
}

int AnimatedTile::LoadImageSequence(std::string fileNamePattern, int numImages){
	if(surfaces) return -1;
		
	surfaces = new SDL_Surface*[numImages];
	char fileName[256];
	
	for(int i = 0; i < numImages; i++){
		snprintf(fileName, 256, fileNamePattern.c_str(), i);
		surfaces[i] = IMG_Load(fileName);
		printf("Loading %s\n", fileName);
		if(!surfaces[i]) return -1;
	}
	
	numSurfaces = numImages;
		
	return 0;
}

void AnimatedTile::SetTickLength(int timeMS){
	tickTime = timeMS;
}

SDL_Surface* AnimatedTile::GetSurface(){
	Uint32 now = SDL_GetTicks();
	
	if(now >= nextTime){
		nextTime = now + tickTime;
		currentSurface++;
		if(currentSurface >= numSurfaces) currentSurface = 0;
	}	
	
	if(!surfaces[currentSurface])
		return surfaces[0];

	return surfaces[currentSurface];
	
}

bool AnimatedTile::IsWalkable(){
    return isWalkable;
}
