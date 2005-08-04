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

#include "common.h"

void Slock(SDL_Surface *screen){
	if(SDL_MUSTLOCK(screen)){
		if(SDL_LockSurface(screen) < 0){
			return;
		}
	}
}

void Sulock(SDL_Surface *screen){
	if (SDL_MUSTLOCK(screen)){
		SDL_UnlockSurface(screen);
	}
}

