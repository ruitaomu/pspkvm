#include <pspkernel.h>
#include "javacall_multimedia.h"
#include "javacall_file.h"
#include "javacall_logging.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h" 

static const javacall_utf16 midi_cfg_fn[] = {'t','i','m','i','d','i','t','y','.','c','f','g'};

#define TMP_MUSIC_FILENAME_LEN_MAX 64
#define DEFAULT_WAVE_BUFFER_LENGTH (64*1024)
#define CHANNEL_NUM 16

#define MEDIA_TYPE_MIDI  1
#define MEDIA_TYPE_WAVE 2
#define MEDIA_TYPE_MP3 3

typedef struct {
	int type;
	int needBuffer;
	int contentLength;
	Mix_Music* music;
	int occupied;
	char filename[JAVACALL_MAX_FILE_NAME_LENGTH];
	javacall_utf16 tmpfilename[64];
	int tmpfilenamelen;
	javacall_handle fp;
	char* buffer;
	int buffered_len;
	int paused;
	javacall_int64 playerId;
} midi_player_handle;

struct _wav_player_handle;
typedef struct _wav_player_handle {
	int type;
	int needBuffer;
	int contentLength;
	Mix_Chunk* chunk;
	int channel;
	char filename[JAVACALL_MAX_FILE_NAME_LENGTH];
	javacall_utf16 tmpfilename[64];
	int tmpfilenamelen;
	javacall_handle fp;	
	javacall_int64 playerId;
	char* buffer;
	int buffered_len;
	int paused;
	struct _wav_player_handle* prev;
	struct _wav_player_handle* next;
} wav_player_handle;

struct _mp3_player_handle;
typedef struct _mp3_player_handle {
	int type;
	int needBuffer;
	int contentLength;
	int occupied;
	char filename[JAVACALL_MAX_FILE_NAME_LENGTH];
	javacall_handle fd;
	char* buffer;
	int buffered_len;
	int paused;
	javacall_int64 playerId;
	int isOpen;
	int isPlaying;
	int numPlayed;
	SceUID mp3thread;
} mp3_player_handle;

typedef union {
	int type;
	midi_player_handle midi;
	wav_player_handle wav;
	mp3_player_handle mp3;
} mmplayer_handle;

#define INVALID_PLAYER_ID (0LL)

