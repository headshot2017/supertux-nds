//  $Id: gameobjs.cpp 834 2004-04-29 10:45:24Z matzebraun $
// 
//  SuperTux
//  Copyright (C) 2000 Bill Kendrick <bill@newbreedsoftware.com>
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
#include <algorithm>
#include "world.h"
#include "tile.h"
#include "gameloop.h"
#include "gameobjs.h"

void
BouncyDistro::init(float x, float y)
{
  base.x = x;
  base.y = y;
  base.ym = -2;
}

void
BouncyDistro::action(double frame_ratio)
{
  base.y = base.y + base.ym * frame_ratio;

  base.ym += 0.1 * frame_ratio;

  if (base.ym >= 0)
    {
      std::vector<BouncyDistro*>::iterator i
        = std::find(World::current()->bouncy_distros.begin(), 
                    World::current()->bouncy_distros.end(), 
                    this);
      if (i != World::current()->bouncy_distros.end())
        World::current()->bouncy_distros.erase(i);
    }
}

void
BouncyDistro::draw()
{
  img_distro[0]->draw((base.x - scroll_x) / 32. * 12.,
                      base.y / 32. * 12.);
}


void
BrokenBrick::init(Tile* tile_, float x, float y, float xm, float ym)
{
  tile    = tile_;
  base.x  = x;
  base.y  = y;
  base.xm = xm;
  base.ym = ym;

  timer.init(true);
  timer.start(200);
}

void
BrokenBrick::action(double frame_ratio)
{
  base.x = base.x + base.xm * frame_ratio;
  base.y = base.y + base.ym * frame_ratio;

  if (!timer.check())
    {
      std::vector<BrokenBrick*>::iterator i
        = std::find(World::current()->broken_bricks.begin(), 
                    World::current()->broken_bricks.end(), 
                    this);
      if (i != World::current()->broken_bricks.end())
        World::current()->broken_bricks.erase(i);
    }
}

void
BrokenBrick::draw()
{
  SDL_Rect src, dest;
  src.x = (rand() % 16)/32.*12.;
  src.y = (rand() % 16)/32.*12.;
  src.w = 6;
  src.h = 6;

  dest.x = (int)((base.x - scroll_x)/32.*12.);
  dest.y = (int)(base.y/32.*12.);
  dest.w = 6;
  dest.h = 6;
  
  if (tile->images.size() > 0)
    tile->images[0]->draw_part(src.x,src.y,dest.x,dest.y,dest.w,dest.h);
}

void
BouncyBrick::init(float x, float y)
{
  base.x   = x;
  base.y   = y;
  offset   = 0;
  offset_m = -BOUNCY_BRICK_SPEED;
  shape    = World::current()->get_level()->gettileid(x, y);
}

void
BouncyBrick::action(double frame_ratio)
{
  offset = (offset + offset_m * frame_ratio);

  /* Go back down? */
  if (offset < -BOUNCY_BRICK_MAX_OFFSET)
    offset_m = BOUNCY_BRICK_SPEED;


  /* Stop bouncing? */
  if (offset >= 0)
    {
      std::vector<BouncyBrick*>::iterator i
        = std::find(World::current()->bouncy_bricks.begin(), 
                    World::current()->bouncy_bricks.end(), 
                    this);
      if (i != World::current()->bouncy_bricks.end())
        World::current()->bouncy_bricks.erase(i);
    }
}

void
BouncyBrick::draw()
{
  SDL_Rect dest;
  float to12 = 1/32.*12.;
  
  if (base.x*to12 >= (scroll_x - 32)*to12 &&
      base.x*to12 <= scroll_x*to12 + screen->w)
    {
      dest.x = (int)((base.x - scroll_x)*to12);
      dest.y = (int)(base.y*to12);
      dest.w = 12;
      dest.h = 12;

      Level* plevel = World::current()->get_level();

      // FIXME: overdrawing hack to clean the tile from the screen to
      // paint it later at on offseted position
      if(plevel->bkgd_image[0] == '\0')
        {
          fillrect((base.x - scroll_x)*to12, base.y*to12,
                   32,32, 
                   plevel->bkgd_top.red, plevel->bkgd_top.green, plevel->bkgd_top.blue, 0);
// FIXME: doesn't respect the gradient, futhermore is this necessary at all??
        }
      else
        {
          int s = ((int)scroll_x / 2)%640*to12;
          plevel->img_bkgd->draw_part(dest.x + s, dest.y, 
                                      dest.x, dest.y,dest.w,dest.h);
        }

      Tile::draw((base.x - scroll_x)*to12,
                 (base.y + offset)*to12,
                 shape);
    }
}

void
FloatingScore::init(float x, float y, int s)
{
  base.x = x;
  base.y = y - 16;
  timer.init(true);
  timer.start(1000);
  value = s;
}

void
FloatingScore::action(double frame_ratio)
{
  base.y = base.y - 2 * frame_ratio;

  if(!timer.check())
    {
      std::vector<FloatingScore*>::iterator i
        = std::find(World::current()->floating_scores.begin(), 
                    World::current()->floating_scores.end(), 
                    this);
      if (i != World::current()->floating_scores.end())
        World::current()->floating_scores.erase(i);
    }
}

void
FloatingScore::draw()
{
  char str[10];
  sprintf(str, "%d", value);
  gold_text->draw(str, (int)(base.x + 16 - strlen(str) * 8)/32.*12., (int)(base.y/32.*12.), 1);
}

/* EOF */

