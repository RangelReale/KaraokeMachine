/* playmidi -- play MIDI files
 */

#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>
#include <SDL.h>
#include "SDL_thread.h"
#include "timidity.h"
#include "playmidi.h"

SDL_mutex *mutex;

void
print_usage()
{
  printf("Usage: playmidi [-r rate] [-s sample_width] [-c channels]\n"
         "                [-v volume] [-q] [midifile]\n");
}

int samplesize;
  size_t bytes_read;
  MidSong *song;
  long total_time;

void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
    SDL_mutexP(mutex);

	  long time = mid_song_get_time (song);
	  printf ("\rTime: %02ld:%02ld/%02ld:%02ld",
		  time / 60000, (time / 1000) % 60,
		  total_time / 60000, (total_time / 1000) % 60);
	  fflush (stdout);

      bytes_read = mid_song_read_wave (song, stream, len);

    SDL_mutexV(mutex);
}

int main(int argc, char *argv[])
{
  int rate = 44100;
  int bits = 16;
  int channels = 2;
  int volume = 100;
  int quiet = 0;
  int arg;
  MidIStream *stream;
  MidSongOptions options;
    SDL_AudioSpec aspec, aspecget;

  for (arg = 1; arg < argc; arg++)
    {
      if (!strcmp(argv[arg], "-r"))
	{
	  if (++arg >= argc) break;
	  rate = atoi (argv[arg]);
	  if (rate <= 0)
	    {
	      fprintf (stderr, "Invalid rate\n");
	      return 1;
	    }
	}
      else if (!strcmp(argv[arg], "-s"))
	{
	  if (++arg >= argc) break;
	  switch (argv[arg][0])
	  {
	    case 'b': bits = 8; break;
	    case 'w': bits = 16; break;
	    default:
              fprintf (stderr, "Invalid sample width\n");
	      return 1;
	  }
	}
      else if (!strcmp(argv[arg], "-c"))
	{
	  if (++arg >= argc) break;
	  channels = atoi (argv[arg]);
	  if (channels < 1 || channels > 2)
	    {
	      fprintf (stderr, "Invalid number of channels\n");
	      return 1;
	    }
	}
      else if (!strcmp(argv[arg], "-v"))
	{
	  if (++arg >= argc) break;
	  volume = atoi (argv[arg]);
	  if (volume < 0)
	    {
	      fprintf (stderr, "Invalid volume\n");
	      return 1;
	    }
	}
      else if (!strcmp(argv[arg], "-q"))
	{
	  quiet = 1;
	}
      else if (!strcmp(argv[arg], "-h"))
	{
	  print_usage();
	  return 0;
	}
      else if (argv[arg][0] == '-')
	{
	  fprintf (stderr, "Unknown option: %s\n", argv[arg]);
	  print_usage();
	  return 1;
	}
      else break;
    }

  if (mid_init (NULL) < 0)
    {
      fprintf (stderr, "Could not initialise libTiMidity\n");
      return 1;
    }

  if (arg >= argc)
    {
      stream = mid_istream_open_fp (stdin, 0);
    }
  else
    {
      stream = mid_istream_open_file (argv[arg]);
      if (stream == NULL)
	{
	  fprintf (stderr, "Could not open file %s\n", argv[arg]);
	  mid_exit ();
	  return 1;
	}

    }

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_AUDIO ) < 0 )
    {
	fprintf (stderr, "SDL error %s\n", SDL_GetError() );
        exit(0);
    }

  options.rate = rate;
  options.format = (bits == 16) ? MID_AUDIO_S16LSB : MID_AUDIO_S8;
  options.channels = channels;
  options.buffer_size = rate;

  //song = mid_song_load (stream, &options);
  song = mid_song_load (NULL, &options);
  mid_istream_close (stream);

  if (song == NULL)
    {
      fprintf (stderr, "Invalid MIDI file\n");
      mid_exit ();
      return 1;
    }

  total_time = mid_song_get_total_time (song);

  mid_song_set_volume (song, volume);
  mid_song_start (song);

    mutex=SDL_CreateMutex();


    memset(&aspec, 0, sizeof(aspec));
    aspec.freq=rate;
    aspec.format=(bits==16?AUDIO_S16:AUDIO_S8);
    aspec.channels=channels;
    aspec.samples=2048;
    aspec.callback=my_audio_callback;

    if ( SDL_OpenAudio( &aspec, &aspecget ) < 0 )
    {
	fprintf (stderr, "SDL error %s\n", SDL_GetError() );
        return 1;
    }

    samplesize = aspecget.size / aspecget.samples;


  if (!quiet)
    {
      char *title = mid_song_get_meta (song, MID_SONG_TEXT);
      if (title == NULL)
        {
	  if (arg < argc) title = argv[arg];
	  else title = "stdin";
	}
      printf ("Playing: %s\n", title);
    }

    SDL_PauseAudio(0);
    int ct=0, nct;
    MidEvent event;
    while (1)
    {
        if (ct==50) {
            event.time=0;
            event.channel=0;
            event.type=ME_PROGRAM;
            event.a=40;
            event.b=0;
            SDL_mutexP(mutex);
            mid_song_event(song, &event);
            SDL_mutexV(mutex);

            event.time=0;
            event.channel=1;
            event.type=ME_PROGRAM;
            event.a=1;
            event.b=0;
            SDL_mutexP(mutex);
            mid_song_event(song, &event);
            SDL_mutexV(mutex);
        }
        if (ct > 50 && ct%20==0) {
            for (nct=0; nct<2; nct++) {
                event.time=0;
                event.channel=nct;
                event.type=ME_NOTEOFF;
                event.a=26+(ct/20)-1;
                event.b=70;
                SDL_mutexP(mutex);
                mid_song_event(song, &event);
                SDL_mutexV(mutex);

                event.time=0;
                event.channel=nct;
                event.type=ME_NOTEON;
                event.a=26+(ct/20);
                event.b=70;
                SDL_mutexP(mutex);
                mid_song_event(song, &event);
                SDL_mutexV(mutex);
            }
        }


        SDL_Delay(100);
        ct++;
    }

  if (!quiet)
    printf ("\n");

  mid_song_free (song);
  mid_exit ();

  SDL_DestroyMutex(mutex);

  return 0;
}
