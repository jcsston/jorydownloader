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

#include "BgMusic.h"

BgMusic::BgMusic()
{
  m_Music = NULL;
}

BgMusic::~BgMusic()
{
  Free();
}

int BgMusic::Load(const std::string &filename)
{
  // load the MOD file to play as music
  m_Music = Mix_LoadMUS(filename.c_str());
  if(!m_Music) {
    printf("Failed to load music: %s\n", Mix_GetError());
    // this might be a critical error...
    return -1;
  }

  return 0;
}

void BgMusic::Free()
{
  if (m_Music != NULL) {
    Mix_FreeMusic(m_Music);
    m_Music = NULL;
  }
}

int BgMusic::Play()
{
  int ret = 0;

  // play music forever
  if((ret=Mix_PlayMusic(m_Music, -1)) == -1) {
    printf("Failed to play music: %s\n", Mix_GetError());
    // well, there's no music, but most games don't break without music...
  }

  return ret;
}
