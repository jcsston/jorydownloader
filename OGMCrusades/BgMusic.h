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

#ifndef _BG_MUSIC_H_
#define _BG_MUSIC_H_

#include <string>
#include <SDL/SDL_mixer.h>
#include "GameDataManger.h"

class BgMusic
{
public:
  BgMusic();
  ~BgMusic();

  int Load(const std::string &filename);
  void Free();
  int Play();

protected:
  Mix_Music *m_Music; 
};

#endif // _BG_MUSIC_H_
