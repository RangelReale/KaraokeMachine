/*
 * @(#)midifile.cpp 3.00 28 Nov 2001
 *
 * Copyright (c) 2001 Pete Goodliffe (pete@cthree.org)
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
#ifdef WIN32
#include "tse3/plt/Win32.h"
#include <windows.h>
#endif
#include "tse3/Playable.h"

// Used in step 3
#include "tse3/Song.h"

#include <string>
using namespace std;

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
        if (c.status==TSE3::MidiCommand_TSE_Meta && c.data1==TSE3::MidiCommand_TSE_Meta_Text)
        {
            std::cout << c.str << endl;
        }
        //std::cout << "MidiOut: " << Show(c) << endl;
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
     * 1. Load the MIDI file
     *************************************************************************/

    TSE3::MidiFileImport mfi(argv[1], 2);

    /**************************************************************************
     * 2. Create a Transport object to do some playback with
     *************************************************************************/

    // (You really want to create a MidiScheduler for your platform)
    TSE3::Metronome                 metronome;
#ifdef WIN32
    TSE3::Plt::Win32MidiScheduler   scheduler;
#else
    TSE3::Util::StreamMidiScheduler scheduler;
#endif
    TSE3::Transport                 transport(&metronome, &scheduler);

    //transport.filter()->setPort(scheduler.portNumber(1));
    transport.filter()->setPort(1);

    //TSE3::Notifier<TSE3::PlayableListener> plisten;
    //TSE3::Transport::listener_type pl2;
    //transport.attachTo(&plisten);

    transport.attachCallback(new mycallback);

    /**************************************************************************
     * 3. Create a Song object from the file, and play the Song
     *************************************************************************/

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

    delete song;
    return 0;
}
