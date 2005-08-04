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

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "common.h"
#include "Screen.h"

char tileMapTerrain[] = 
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

char tileMapObjects[] = 
{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 0, 0, 3, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };


Screen* myScreen;
TileCache tileCache;
TilePallete tilePallete(&tileCache);

void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B){
	Uint32 color = SDL_MapRGB(screen->format, R, G, B);
	switch(screen->format->BytesPerPixel){
		case 1:
			{
				Uint8 *bufp;
				bufp = (Uint8*)screen->pixels + y*screen->pitch + x;
				*bufp = color;
			}
			break;
		case 2:
			{
				Uint16 *bufp;
				bufp = (Uint16*)screen->pixels + y*screen->pitch/2 + x;
				*bufp = color;
			}
			break;
		case 3: // Slow 24-bpp mode, usually not used
			{
				Uint8 *bufp;
				bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
				if(SDL_BYTEORDER == SDL_LIL_ENDIAN){
					bufp[0] = color;
					bufp[1] = color >> 8;
					bufp[2] = color >> 16;
				}else{
					bufp[2] = color;
					bufp[1] = color >> 8;
					bufp[0] = color >> 16;
				}
			}
			break;
		case 4: // Probably 32-bpp
			{
				Uint32 *bufp;
				bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
				*bufp = color;
			}
			break;
	}	
}


void DrawScene(SDL_Surface *screen){
	int x,y,c;
	/*Slock(screen);
	for(y = 0; y < 480; y++){
		for(x = 0; x < 640; x++){
			//c = (Uint8)(((double) x / 640 ) * 256); 
			DrawPixel(screen, x,y,x/3,y/2,x/3);
		}
	}
	
	SDL_Rect rcDest = { 100, 100, 0, 0 };
	SDL_Surface* image = tilePallete.GetTile(0)->GetSurface();
	SDL_BlitSurface(image, NULL, screen, &rcDest);
	
	Sulock(screen);*/
	myScreen->Render(screen);
	SDL_Flip(screen);
}

int main(int argc, char* argv[]){
	SDL_Surface *screen;
	int done;
	SDL_Event event;
	
		
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	atexit(SDL_Quit);
	
	screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	
	if(screen == NULL){
		printf("Unable obtain screen surface: %s\n", SDL_GetError());
		return 1;
	}
	
	tilePallete.LoadTile("grass-plain.png");
	int bushID = tilePallete.LoadTile("spyder482.bush2.png");
    Tile* bushTile = tilePallete.GetTile(bushID);
    bushTile->isWalkable = false;
	
	//Animated Tile
	AnimatedTile* animTile = new AnimatedTile();
	printf("Loading...");
	if(animTile->LoadImageSequence("bush%02d.png", 2)){
		printf("ERROR LOADING IMAGE SEQUENCE");
		return -1;
	}
	animTile->SetTickLength(500);
	
	printf("done.\n");
	
	tilePallete.AddTile(animTile);
	
    Sprite* mySprite = new Sprite();
    mySprite->LoadImage("spyder482.bush2.png");
    mySprite->x = 320;
    mySprite->y = 140;
    //mySprite->AddMotionGuide(new CircularMotionGuide(0.001, 100, 320, 240, 90));
    
    CharacterMotionGuide* cmg = new CharacterMotionGuide();
    mySprite->AddMotionGuide(cmg);
    mySprite->AddMotionGuide(new WalkingMotionGuide());
    
    myScreen = new Screen(640,480,32);
	myScreen->SetTilePallete(&tilePallete);
	myScreen->SetTileMap(0, tileMapTerrain);
	myScreen->SetTileMap(1, tileMapObjects);
    myScreen->AddSprite(mySprite);
	
	done = 0;
	
	while(!done){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)
				done = 1;
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				done = 1;
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP)
				cmg->AddVelocityComponent(10, 90);
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN)
				cmg->AddVelocityComponent(10, 270);
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT)
				cmg->AddVelocityComponent(10, 180);
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT)
				cmg->AddVelocityComponent(10, 0);
            if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP)
				cmg->RemoveVelocityComponent(10, 90);
                //cmg->Stop();
            if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN)
				cmg->RemoveVelocityComponent(10, 270);
                //cmg->Stop();
            if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LEFT)
				cmg->RemoveVelocityComponent(10, 180);
                //cmg->Stop();
            if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RIGHT)
				cmg->RemoveVelocityComponent(10, 0);
                //cmg->Stop();
		}			
		DrawScene(screen);
	}
	
	tileCache.Clear();
	//SDL_Quit();
	
}
