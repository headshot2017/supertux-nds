//  $Id$
// 
//  SuperTux
//  Copyright (C) 2004 Tobias Glaesser <tobi.web@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#include <sys/types.h>
#include <ctype.h>

#include <nds.h>
#include <maxmod9.h>
#include <fat.h>

#include "defines.h"
#include "globals.h"
#include "setup.h"
#include "intro.h"
#include "title.h"
#include "gameloop.h"
//#include "leveleditor.h"
#include "screen.h"
#include "worldmap.h"
#include "resources.h"
#include "texture.h"
#include "tile.h"

int main(int argc, char * argv[])
{
  consoleDemoInit();
  if (!fatInitDefault())
  {
	  iprintf("failed to init fat\ncheck your microSD card\n");
	  while(1) swiWaitForVBlank();
  }

  st_directory_setup();
  parseargs(argc, argv);

  st_video_setup();
  iprintf("Audio setup\n");
  st_audio_setup();
  iprintf("Joystick setup\n");
  st_joystick_setup();
  iprintf("General setup\n");
  st_general_setup();
  iprintf("Loading menus\n");
  st_menu();
  iprintf("Loading shared assets\n");
  loadshared();

  SDL_ShowCursor(false);
  setBackdropColorSub(RGB15(4, 9, 15));

  /*if (launch_leveleditor_mode && level_startup_file)
    {
    leveleditor(level_startup_file);
    }
  else*/ if (level_startup_file)
    {
      GameSession session(level_startup_file, 1, ST_GL_LOAD_LEVEL_FILE);
      session.run();
    }
  else
    {
	  consoleClear();
      fadeout();
      title();
    }
  
  clearscreen(0, 0, 0);
  updatescreen();

  unloadshared();
  st_general_free();
  TileManager::destroy_instance();
#ifdef DEBUG
  Surface::debug_check();
#endif
  st_shutdown();

	consoleClear();
	iprintf("SuperTux for the DS\nPorted by Headshotnoby\n\nThank you for playing!\n");
	SDL_Delay(3000);

  return 0;
}
