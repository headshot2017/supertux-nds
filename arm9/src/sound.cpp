//  $Id: sound.cpp 1741 2004-08-10 22:50:39Z rmcruz $
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2000 Bill Kendrick <bill@newbreedsoftware.com>
//  Copyright (C) 2004 Duong-Khang NGUYEN <neoneurone@users.sf.net>
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "defines.h"
#include "globals.h"
#include "sound.h"
#include "setup.h"
#include <map>
#include <maxmod9.h>

#include <soundbank.h>

/*global variable*/
bool use_sound = true;    /* handle sound on/off menu and command-line option */
bool use_music = true;    /* handle music on/off menu and command-line option */
bool audio_device = true; /* != 0: available and initialized */

char * soundfilenames[NUM_SOUNDS] = {
                                       "jump.wav",
                                       "bigjump.wav",
                                       "skid.wav",
                                       "distro.wav",
                                       "herring.wav",
                                       "brick.wav",
                                       "hurt.wav",
                                       "squish.wav",
                                       "fall.wav",
                                       "ricochet.wav",
                                       "bump-upgrade.wav",
                                       "upgrade.wav",
                                       "excellent.wav",
                                       "coffee.wav",
                                       "shoot.wav",
                                       "lifeup.wav",
                                       "stomp.wav",
                                       "kick.wav",
                                       "explode.wav",
                                       "warp.wav"
                                    };

//#include <SDL_mixer.h>

mm_word sounds[NUM_SOUNDS];
std::map<std::string, mm_word> Sound2Handle; // sound handle from string to maxmod handle

/* --- OPEN THE AUDIO DEVICE --- */

int open_audio()
{
  /*
  if (Mix_OpenAudio( frequency, format, channels, chunksize ) < 0)
    return -1;

  // allocate 16 channels for mixing
  if (Mix_AllocateChannels(8)  != 8)
    return -2;
  
  // reserve some channels and register panning effects
  if (Mix_ReserveChannels(SOUND_RESERVED_CHANNELS) != SOUND_RESERVED_CHANNELS)
    return -3;

  // prepare the spanning effects
  Mix_SetPanning( SOUND_LEFT_SPEAKER, 230, 24 );
  Mix_SetPanning( SOUND_RIGHT_SPEAKER, 24, 230 );
  */

  Sound2Handle["jump.wav"] = SFX_JUMP;
  Sound2Handle["bigjump.wav"] = SFX_BIGJUMP;
  Sound2Handle["skid.wav"] = SFX_SKID;
  Sound2Handle["distro.wav"] = SFX_DISTRO;
  Sound2Handle["herring.wav"] = SFX_HERRING;
  Sound2Handle["brick.wav"] = SFX_BRICK;
  Sound2Handle["hurt.wav"] = SFX_HURT;
  Sound2Handle["squish.wav"] = SFX_SQUISH;
  Sound2Handle["fall.wav"] = SFX_FALL;
  Sound2Handle["ricochet.wav"] = SFX_RICOCHET;
  Sound2Handle["bump-upgrade.wav"] = SFX_BUMP_UPGRADE;
  Sound2Handle["upgrade.wav"] = SFX_UPGRADE;
  Sound2Handle["excellent.wav"] = SFX_EXCELLENT;
  Sound2Handle["coffee.wav"] = SFX_COFFEE;
  Sound2Handle["shoot.wav"] = SFX_SHOOT;
  Sound2Handle["lifeup.wav"] = SFX_LIFEUP;
  Sound2Handle["stomp.wav"] = SFX_STOMP;
  Sound2Handle["kick.wav"] = SFX_KICK;
  Sound2Handle["explode.wav"] = SFX_EXPLODE;
  Sound2Handle["warp.wav"] = SFX_WARP;
  return 0;
}


/* --- CLOSE THE AUDIO DEVICE --- */

void close_audio( void )
{
  /*if (audio_device) {
    Mix_UnregisterAllEffects( SOUND_LEFT_SPEAKER );
    Mix_UnregisterAllEffects( SOUND_RIGHT_SPEAKER );
    Mix_CloseAudio();
  }*/
}


/* --- LOAD A SOUND --- */

mm_word load_sound(const std::string& file)
{
  if(!audio_device)
    return 0;
  
  /*Mix_Chunk* snd = Mix_LoadWAV(file.c_str());

  if (snd == 0)
    st_abort("Can't load", file);*/

  mmLoadEffect(Sound2Handle[file]);
  return Sound2Handle[file];
}

/* --- PLAY A SOUND ON LEFT OR RIGHT OR CENTER SPEAKER --- */

void play_sound(mm_word snd, enum Sound_Speaker whichSpeaker)
{
  /* this won't call the function if the user has disabled sound
   * either via menu or via command-line option
   */
  if(!audio_device || !use_sound)
    return;

  //Mix_PlayChannel( whichSpeaker, snd, 0);
  mm_sfxhand handle = mmEffect(snd);

  /* prepare for panning effects for next call */
  switch (whichSpeaker) {
    case SOUND_LEFT_SPEAKER:
	  mmEffectPanning(handle, 32);
      //Mix_SetPanning( SOUND_LEFT_SPEAKER, 230, 24 );
      break;
    case SOUND_RIGHT_SPEAKER:
	  mmEffectPanning(handle, 224);
      //Mix_SetPanning( SOUND_RIGHT_SPEAKER, 24, 230 );
      break;
    default:  // keep the compiler happy
      break;
  }
}

void free_chunk(mm_word chunk)
{
  //Mix_FreeChunk( chunk );
  mmUnloadEffect(chunk);
}

