#ifdef __cplusplus
extern "C" {
#endif

#ifndef __mp3_shared_h__
#define __mp3_shared_h__

#if defined(DS_MP3) && (defined(DS_MAXMOD) || defined(DS_WIFI)) // mp3 cannot fit with maxmod and/or wifi on the arm7 together so we allocate the mp3 decoder on arm9
	#define DS_MP3ARM9
#endif

//#include <stdio.h>
#ifdef DS_MP3ARM9
#include "mp3dec.h"
#endif

typedef vu32 DSTIME;

typedef enum {
        MP3_IDLE=0,
        MP3_STARTING=1,
        MP3_PLAYING=2,
        MP3_PAUSING=3,
        MP3_RESUMING=4,
        MP3_PAUSED=5,
        MP3_STOPPING=6,
        MP3_ERROR=0xffffffff
} mp3_player_state;

typedef enum
{
	WAV_IDLE,
	WAV_STARTING,
	WAV_PLAYING,
	WAV_STOPPING
} wav_player_state;

typedef struct {
#ifdef DS_MP3ARM9
		HMP3Decoder hMP3Decoder;
#endif
        vu32    flag;
        vs32    rate;
        vs32    filesize;
        vs32    loop;
        u8      *buffer;
        u16     *audioLeft;
        u16     *audioRight;

	DSTIME soundtime;

} mp3_player;

typedef struct
{
	vu16	channels;
	vu32	samplerate;
	vu64 length;
	vu16 volume;
	u32	*data;
	vu8 channel, lastChannel;
	vu8 loop, state, slot;

} wav_stuffs;

enum {
        MP3_MSG_START=0,
        MP3_MSG_STOP=1,
        MP3_MSG_PAUSE=2,
        MP3_MSG_RESUME=3,
        MP3_MSG_VOLUME,
        MP3_MSG_ERROR=0xffffffff,

	WAV_MSG_ADD=7,
	WAV_MSG_REMOVE,
	WAV_MSG_GETSLOT,
};

typedef struct {
        u32     type;
        union {
                volatile mp3_player	*player;
		volatile wav_stuffs	*wavplayer;
		int			slot;
                u32			volume;
        };
} mp3_msg;


#define OUTBUF_SIZE     (MAX_NCHAN * MAX_NGRAN * MAX_NSAMP)
#define MP3_FILE_BUFFER_SIZE (8 * 1024)

#define MP3_AUDIO_BUFFER_SAMPS (8 * 1024)
#define MP3_AUDIO_BUFFER_SIZE (MP3_AUDIO_BUFFER_SAMPS*2)

#ifdef ARM9

int mp3_init();
int mp3_play_file(FILE *file,int loop,float loopsec);
int mp3_play(const char *filename,int loop=0,float loopsec=0);
void mp3_fill_buffer();
int mp3_pause();
int mp3_resume();
int mp3_stop();
int mp3_set_volume(int volume);
bool mp3_is_playing();

volatile wav_stuffs* wav_load_handle(const char *filename, int* error_output=NULL);
void wav_free_handle(volatile wav_stuffs* wav, int* error_output=NULL);
int wav_play_handle(volatile wav_stuffs* wav, int channel, bool loop=false);
int wav_unload(volatile wav_stuffs* wav);
void wav_set_volume(volatile wav_stuffs* handle, u16 volume);
float wav_get_length(volatile wav_stuffs* handle);
u8 wav_get_free_slot();

#endif

#ifdef ARM7

void mp3_init();
void mp3_process();
void AS_StereoDesinterleave(s16 *input, s16 *outputL, s16 *outputR, u32 samples);

#endif

#endif

#ifdef __cplusplus
}
#endif