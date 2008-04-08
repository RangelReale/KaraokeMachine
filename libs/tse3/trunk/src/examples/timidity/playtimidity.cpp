/*
 * @(#)playtimidity.cpp 3.00 28 Nov 2001
 *
 * Copyright (c) 2008 Rangel Reale (rreale@bol.com.br)
 *
 * This file is part of TSE3 - the Trax Sequencer Engine version 3.00.
 *
 * This library is modifiable/redistributable under the terms of the GNU
 * General Public License.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; see the file COPYING. If not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

    /***************************************************************
     * TSE3 example MIDI file program
     * ==============================
     *
     * This example contains four "steps"
     *
     *   1. We use a MidiFile object to access a standard MIDI file.
     *   2. We create a Transport object to play it with
     *   3. We create a Song object from the data in the MIDI file
     *      and play it.
     *   4. Creating a Song takes some time (well, not too much really).
     *      If you don't need to manipulate the data in the MIDI file,
     *      just play it, this shows a slightly quicker way.
     *
     **************************************************************/

#include <iostream>

// Used in step 1
#include "tse3/MidiFile.h"

// Used in step 2
#include "tse3/Metronome.h"
#include "tse3/util/MidiScheduler.h"
#include "tse3/Transport.h"
#include "tse3/plt/libTimidity.h"
#include "tse3/Playable.h"

// Used in step 3
#include "tse3/Song.h"

#include "timidity.h"

#include <SDL.h>
#include "SDL_thread.h"

#include <string>
using namespace std;

void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
/*
	SDL_mutexP(mutex);

	  long time = mid_song_get_time (song);
	  printf ("\rTime: %02ld:%02ld/%02ld:%02ld",
		  time / 60000, (time / 1000) % 60,
		  total_time / 60000, (total_time / 1000) % 60);
	  fflush (stdout);
*/
      mid_song_read_wave ((MidSong*)userdata, stream, len);

    //SDL_mutexV(mutex);
}

class mycallback : public TSE3::TransportCallback
{
public:
    std::string Show(TSE3::MidiCommand c) {
        switch (c.status)
        {
        case TSE3::MidiCommand_NoteOff: return "NoteOff"; break;
        case TSE3::MidiCommand_NoteOn: return "NoteOn"; break;
        case TSE3::MidiCommand_KeyPressure: return "KeyPressure"; break;
        case TSE3::MidiCommand_ControlChange: return "ControlChange"; break;
        case TSE3::MidiCommand_ProgramChange: return "ProgramChange"; break;
        case TSE3::MidiCommand_ChannelPressure: return "ChannelPressure"; break;
        case TSE3::MidiCommand_PitchBend: return "PitchBend"; break;
        case TSE3::MidiCommand_System: return "System"; break;

        // Meta MIDI messages
        case TSE3::MidiCommand_Invalid: return "Invalid"; break;
        case TSE3::MidiCommand_TSE_Meta:
        {
            switch (c.data1)
            {
            case TSE3::MidiCommand_TSE_Meta_Tempo: return "TSE Meta: Tempo"; break;
            case TSE3::MidiCommand_TSE_Meta_TimeSig: return "TSE Meta: TimeSig"; break;
            case TSE3::MidiCommand_TSE_Meta_KeySig: return "TSE Meta: KeySig"; break;
            case TSE3::MidiCommand_TSE_Meta_MoveTo: return "TSE Meta: MoveTo"; break;
            case TSE3::MidiCommand_TSE_Meta_Text : {
                return string("TSE Meta: Text = ")+c.str;
                break;
            }
            default:
                return "TSE Meta"; break;
            }
        }
        case TSE3::MidiCommand_NoteEdit_Meta: return "Noteedit meta"; break;
        }
        return "Unknown";
    }

    virtual void 	Transport_MidiIn (TSE3::MidiCommand c) {
        std::cout << "MidiIn: " << Show(c) << endl;
    }
    virtual void 	Transport_MidiOut (TSE3::MidiCommand c) {
        //if (c.status==TSE3::MidiCommand_TSE_Meta && c.data1==TSE3::MidiCommand_TSE_Meta_Text)
        //{
        //    std::cout << c.str << endl;
        //}
        std::cout << "MidiOut: " << Show(c) << "=" << c.channel << " :: " << c.data1 << " - " << c.data2 << endl;
    }
};


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Useage: midifile <filename>\n";
        exit(1);
    }

    /**************************************************************************
     * Initialize libTimidity
     *************************************************************************/
	MidSong *tisong;
	MidSongOptions options;

	if (mid_init (NULL) < 0)
	{
		fprintf (stderr, "Could not initialise libTiMidity\n");
		return 1;
	}
	options.rate = 44100;
	options.format = MID_AUDIO_S16LSB;
	options.channels = 2;
	options.buffer_size = 44100;
	tisong = mid_song_load (NULL, &options);

	mid_song_set_volume (tisong, 100);
	mid_song_start (tisong);

    /**************************************************************************
     * Initialize SDL
     *************************************************************************/
    SDL_AudioSpec aspec, aspecget;

    // initialize SDL audio
    if ( SDL_Init( SDL_INIT_AUDIO ) < 0 )
    {
	fprintf (stderr, "SDL error %s\n", SDL_GetError() );
        exit(0);
    }

    memset(&aspec, 0, sizeof(aspec));
    aspec.freq=options.rate;
    aspec.format=(options.format==MID_AUDIO_S16LSB?AUDIO_S16:AUDIO_S8);
    aspec.channels=options.channels;
    aspec.samples=2048;
    aspec.callback=my_audio_callback;
    aspec.userdata=tisong;

    if ( SDL_OpenAudio( &aspec, &aspecget ) < 0 )
    {
	fprintf (stderr, "SDL error %s\n", SDL_GetError() );
        return 1;
    }
    
    /**************************************************************************
     * 1. Load the MIDI file
     *************************************************************************/

    TSE3::MidiFileImport mfi(argv[1], 2);

    /**************************************************************************
     * 2. Create a Transport object to do some playback with
     *************************************************************************/

    TSE3::Metronome                 metronome;
    TSE3::Plt::TimidityMidiScheduler   scheduler(tisong);
    //TSE3::Util::StreamMidiScheduler scheduler;
    TSE3::Transport                 transport(&metronome, &scheduler);

    //transport.filter()->setPort(scheduler.portNumber(1));
    transport.filter()->setPort(0);

    //TSE3::Notifier<TSE3::PlayableListener> plisten;
    //TSE3::Transport::listener_type pl2;
    //transport.attachTo(&plisten);

    //transport.attachCallback(new mycallback);

	//scheduler.SetSong(tisong);

    /**************************************************************************
     * 3. Create a Song object from the file, and play the Song
     *************************************************************************/
    SDL_PauseAudio(0);

    TSE3::Song *song = mfi.load();
    transport.play(song, 0);
    while (transport.status() != TSE3::Transport::Resting)
    {
        transport.poll();
        // perhaps sleep here to prevent slaughtering the CPU
        //Sleep(100);
    }

    /**************************************************************************
     * 4. The quicker way to just play a MIDI file
     *************************************************************************/
/*
    transport.play(&mfi, 0);
    while (transport.status() != TSE3::Transport::Resting)
    {
        transport.poll();
        // perhaps sleep here to prevent slaughtering the CPU
    }
*/

    /**************************************************************************
     * All done
     *************************************************************************/
	SDL_PauseAudio(1);
	SDL_CloseAudio();
    
	mid_song_free (tisong);
	mid_exit ();
    
    delete song;
    return 0;
}
