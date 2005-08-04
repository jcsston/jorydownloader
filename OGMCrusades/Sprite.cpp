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

#include "Sprite.h"
#include "Screen.h"
#include <SDL/SDL_image.h>
#include <cmath>

#define PI 3.14159265f

#define ROUGH_ANGLE_COMPARE(x,y) (((int)(x*1000)) == ((int)(y*1000)))

inline float deg2rad(float x){
    if(x < 0)
        x += 360;
    
    return ((x/180.0f)*PI);
}

Sprite::Sprite(std::string name):surface(NULL),guides(),width(0),height(0),x(0),y(0),speed(0),directionAngle(0),isFlying(true),name(name){
}

Sprite::~Sprite(){
    if(surface){
		SDL_FreeSurface(surface);
		surface = NULL;
	}
    
    //Destroy motion guides
    std::vector<MotionGuide*>::iterator iter;
    for(iter = guides.begin(); iter != guides.end(); iter++){
        if(*iter)
            delete *iter;
            *iter = NULL;
    }    
    guides.clear();
    
    //Destroy collision handlers
    for(std::vector<CollisionHandler*>::iterator iter = collisionHandlers.begin(); iter != collisionHandlers.end(); iter++){
        if(*iter){
            delete *iter;
            *iter = NULL;
        }
    }
    collisionHandlers.clear();
    
}

int Sprite::LoadImage(std::string fileName){
    surface = IMG_Load(fileName.c_str());
	if(!surface) return -1;
	
    width = surface->w;
    height = surface->h;
	return 0;
}

SDL_Surface* Sprite::GetSurface(){
    return surface;    
}

void Sprite::AddMotionGuide(MotionGuide* guide){
    if(guide)
    guides.push_back(guide);
}

void Sprite::AddCollisionHandler(CollisionHandler* handler){
    if(handler)
    collisionHandlers.push_back(handler);
}

void Sprite::Update(Screen* screen){
    std::vector<MotionGuide*>::iterator iter;
    for(iter = guides.begin(); iter != guides.end(); iter++){
        (*iter)->UpdateMotion(this, screen);
    }        
}


void Sprite::CheckCollision(Sprite* sprite){
    //Colliding sprite's rectangle
    int leftX2 = (sprite->x);
    int topY2 = (sprite->y);
    int rightX2 = (sprite->x+sprite->width);
    int botY2 = (sprite->y+sprite->height);
    //Our rectangle
    int leftX = (this->x);
    int topY = (this->y);
    int rightX = (this->x+this->width);
    int botY = (this->y+this->height);
    
    if(!(  ((leftX)<(leftX2) && (rightX)<(leftX2))
        || ((leftX)>(rightX2) && (rightX)>(rightX2)) 
        || ((topY)<(topY2) && (botY)<(topY2)) 
        || ((topY)>(botY2) && (botY)>(botY2))   
    ))
    
    for(std::vector<CollisionHandler*>::iterator iter = collisionHandlers.begin(); iter != collisionHandlers.end(); iter++){
        if(*iter){
            (*iter)->HandleCollision(this, sprite);
        }
    }
}

CircularMotionGuide::CircularMotionGuide(float angularVelocity, int radius, int centerX, int centerY, int intialAngle):angularVelocity(angularVelocity),radius(radius),centerX(centerX),centerY(centerY),initialAngle(initialAngle){
    lastTime = (unsigned int) -1;    
}

void CircularMotionGuide::UpdateMotion(Sprite* sprite, Screen* screen){
    Uint32 now = SDL_GetTicks();
    
    if(lastTime == (unsigned int)-1){
        lastTime = now;
        currentAngle = initialAngle;
    }else{
        Uint32 timePassed = now - lastTime;
        float angleChange = (timePassed) * (angularVelocity * (360.0f / 1000.0f));
        //currentAngle = asin((float)sprite->y / radius);
        currentAngle += angleChange;
        while(currentAngle >= 360){
            currentAngle -= 360;
        }
        lastTime = now;
    }
    
    int xCoord = (int) (centerX + (radius * cos(deg2rad(currentAngle))));
    int yCoord = (int) (centerY + (-1 * (radius * sin(deg2rad(currentAngle)))));
    
    sprite->x = xCoord;
    sprite->y = yCoord;    
    
}

WalkingMotionGuide::WalkingMotionGuide():lastX((unsigned int)-1),lastY((unsigned int)-1){
    
}

bool isValidPosition(Screen* screen, Sprite* sprite, int x, int y){
    Tile* tile;
    int tileSize = screen->GetTileSize();
    int leftX = (x) / tileSize;
    int topY = (y) / tileSize;
    int rightX = (x+sprite->width-1) / tileSize;
    int botY = (y+sprite->height-1) / tileSize;
    
    for(int i = 0; i < MAX_LAYERS; i++){
        for(int j = leftX; j <= rightX; j++){
            for(int k = topY; k <= botY; k++){
                tile = screen->GetTile(j, k, i);
                if(tile){            
                    if(!tile->isWalkable){
                        return false;
                        break;
                    }
                }
            }
        }
    }
    
    return true;
}

void WalkingMotionGuide::UpdateMotion(Sprite* sprite, Screen* screen){    
    bool can_walk = true;
    int tileSize = screen->GetTileSize();    
    if(lastX == (unsigned int)-1 && lastY == (unsigned int)-1){
        lastX = sprite->x;
        lastY = sprite->y;
    }
    
    //int leftX = (sprite->x) / tileSize;
    //int topY = (sprite->y) / tileSize;
    int rightX = (sprite->x+sprite->width-1) / tileSize;
    int botY = (sprite->y+sprite->height-1) / tileSize;
        
    can_walk = isValidPosition(screen, sprite, sprite->x, sprite->y);
    
    if(!can_walk){
        
        if(lastX != sprite->x && lastY == sprite->y){
            if(ROUGH_ANGLE_COMPARE(sprite->directionAngle,deg2rad(0))){
                sprite->x = (rightX * tileSize) - sprite->width -1;                 
            }else if(ROUGH_ANGLE_COMPARE(sprite->directionAngle,deg2rad(180))){
                sprite->x = (lastX / tileSize) * tileSize;                
            }else{
                sprite->x = lastX;
            }
        }else if(lastY != sprite->y && lastX == sprite->x){
            if(ROUGH_ANGLE_COMPARE(sprite->directionAngle,deg2rad(90))){
                sprite->y = (lastY / tileSize) * tileSize;        
            }else if(ROUGH_ANGLE_COMPARE(sprite->directionAngle,deg2rad(270))){            
                sprite->y = (botY * tileSize) - sprite->height;           
            }else{
                sprite->y = lastY;
            }
        }else if(lastY != sprite->y && lastX != sprite->x){
            if(ROUGH_ANGLE_COMPARE(sprite->directionAngle,deg2rad(45))){                    
                if(isValidPosition(screen, sprite, (rightX * tileSize) - sprite->width -1, sprite->y))
                    sprite->x = (rightX * tileSize) - sprite->width -1;
                else if(isValidPosition(screen, sprite, sprite->x, (lastY / tileSize) * tileSize))
                    sprite->y = (lastY / tileSize) * tileSize;    
                else{
                    sprite->x = (rightX * tileSize) - sprite->width -1;
                    sprite->y = (lastY / tileSize) * tileSize;    
                }
            }else if(ROUGH_ANGLE_COMPARE(sprite->directionAngle,deg2rad(315))){
                if(isValidPosition(screen, sprite, (rightX * tileSize) - sprite->width -1, sprite->y))
                    sprite->x = (rightX * tileSize) - sprite->width -1;
                else if(isValidPosition(screen, sprite, sprite->x, (botY * tileSize) - sprite->height))
                    sprite->y = (botY * tileSize) - sprite->height;
                else{
                    sprite->x = (rightX * tileSize) - sprite->width -1;
                    sprite->y = (botY * tileSize) - sprite->height;
                }
            }else if(ROUGH_ANGLE_COMPARE(sprite->directionAngle,deg2rad(135))){
                if(isValidPosition(screen, sprite, (lastX / tileSize) * tileSize, sprite->y))
                    sprite->x = (lastX / tileSize) * tileSize;
                else if(isValidPosition(screen, sprite, sprite->x, (lastY / tileSize) * tileSize))
                    sprite->y = (lastY / tileSize) * tileSize;      
                else{
                    sprite->x = (lastX / tileSize) * tileSize;
                    sprite->y = (lastY / tileSize) * tileSize;      
                }
            }else if(ROUGH_ANGLE_COMPARE(sprite->directionAngle,deg2rad(225))){
                if(isValidPosition(screen, sprite, ((lastX / tileSize) * tileSize) , sprite->y))
                    sprite->x = ((lastX / tileSize) * tileSize);
                else if(isValidPosition(screen, sprite, sprite->x, (botY * tileSize) - sprite->height))
                    sprite->y = (botY * tileSize) - sprite->height;
                else{
                    sprite->x = ((lastX / tileSize) * tileSize);
                    sprite->y = (botY * tileSize) - sprite->height;
                }
            }else{
                sprite->x = lastX;
                sprite->y = lastY;
            }
        }            
    }
        
    lastX = sprite->x;
    lastY = sprite->y;
}


CharacterMotionGuide::CharacterMotionGuide(){
    
}
    
void CharacterMotionGuide::UpdateMotion(Sprite* sprite, Screen* screen){
    sprite->x += (unsigned int) (speed * cos(directionAngle));
    sprite->y -= (unsigned int) (speed * sin(directionAngle));
    sprite->directionAngle = directionAngle;
}

void CharacterMotionGuide::AddVelocityComponent(int speed, int direction){
    float XcompA = cos(deg2rad(direction)) * speed;
    float YcompA;
    
    if(direction != 180)
        YcompA = sin(deg2rad(direction)) * speed;
    else
        YcompA = 0;
        
    float XcompB = cos(this->directionAngle) * this->speed;
    float YcompB = sin(this->directionAngle) * this->speed;
    
    float resX = XcompB + XcompA;
    float resY = YcompB + YcompA;
    this->speed = sqrt((resX*resX) + (resY*resY));
       
   if(resX != 0 && resY != 0){
        this->directionAngle = atan(resY/resX);
        if(directionAngle < 0 && resY > 0)
            directionAngle += deg2rad(180); 
        if(directionAngle > 0 && resY < 0)
            directionAngle += deg2rad(180);        
    }else if(resY == 0 && resX > 0)
        this->directionAngle = deg2rad(0);
    else if(resY == 0 && resX < 0)
        this->directionAngle = deg2rad(180);
    else if(resX == 0 && resY > 0)
        this->directionAngle = deg2rad(90);
    else if(resX == 0 && resY < 0)
        this->directionAngle = deg2rad(270);
    else
        this->directionAngle = deg2rad(0);    
    
    if(this->directionAngle < 0)
        this->directionAngle += 2*PI;
    
}

void CharacterMotionGuide::Stop(){
    this->directionAngle = 0;
    this->speed = 0;
}

void CharacterMotionGuide::RemoveVelocityComponent(int speed, int direction){
    AddVelocityComponent(speed, direction+180);
}


