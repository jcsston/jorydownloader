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

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL/SDL.h>
#include <string>
#include <vector>

class MotionGuide;
class Screen;
    
class Sprite{  
  private:
    SDL_Surface * surface;
  protected:    
    std::vector<MotionGuide*> guides;
  public:
    Uint32 width, height, x, y;
    Uint32 speed;
    float directionAngle;
    bool isFlying; /* means this sprite is not bounded */
  
    Sprite();
    virtual ~Sprite();
    int LoadImage(std::string fileName);
    virtual SDL_Surface* GetSurface();
    virtual void AddMotionGuide(MotionGuide* guide);
    virtual void Update(Screen* screen);
    
};

class MotionGuide{
  public:
    virtual void UpdateMotion(Sprite* sprite, Screen* screen) = 0;
};

class CircularMotionGuide: public MotionGuide{
    private:
        float angularVelocity;
        int radius,centerX,centerY,initialAngle;
        Uint32 lastTime;
        float currentAngle;
    public:
        CircularMotionGuide(float angularVelocity, int radius, int centerX, int centerY, int intialAngle = 0);
        virtual void UpdateMotion(Sprite* sprite, Screen* screen);
};

class WalkingMotionGuide: public MotionGuide{
    private:
        Uint32 lastX,lastY;
    public:
        WalkingMotionGuide();
        virtual void UpdateMotion(Sprite* sprite, Screen* screen);
};

class CharacterMotionGuide: public MotionGuide{
    private:
        float speed, directionAngle;
    public:
        CharacterMotionGuide();
        virtual void UpdateMotion(Sprite* sprite, Screen* screen);
        virtual void Stop();
        virtual void AddVelocityComponent(int speed, int direction);
        virtual void RemoveVelocityComponent(int speed, int direction);
};


#endif
