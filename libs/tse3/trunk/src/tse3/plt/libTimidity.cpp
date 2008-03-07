/*
 * @(#)libTimidity.cpp 1.00 05 March 2008
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

#include "tse3/plt/libTimidity.h"
#include "tse3/Error.h"
#include "tse3/util/MulDiv.h"

#include "tse3/util/MidiScheduler.h"

#ifdef __WIN32__
#include <windows.h>
#elif defined(__unix__)
#include <sys/time.h>
#endif

using namespace TSE3;
using namespace TSE3::Plt;


//static char *STR_MOD_FMSYNTH  = "FM synthesizer";
//static char *STR_MOD_MAPPER   = "MIDI mapper";
//static char *STR_MOD_MIDIPORT = "MIDI hardware port";
//static char *STR_MOD_SQSYNTH  = "Square wave synthesizer";
//static char *STR_MOD_SYNTH    = "Synthesizer";
//static char *STR_MOD_UNKNOWN  = "Unknown MIDI device";



/******************************************************************************
 * TimidityMidiScheduler class
 *****************************************************************************/

TimidityMidiScheduler::TimidityMidiScheduler(MidSong* msong)
: wstartClock(0)
{
    song=msong;

/*
    if (mid_init (NULL) < 0)
        throw TSE3::MidiSchedulerError(MidiSchedulerCreateErr);

    song=mid_song_load (NULL, const_cast<MidSongOptions*>(&options));
    if (!song)
        throw TSE3::MidiSchedulerError(MidiSchedulerCreateErr);
*/
    addPort(0, true, 0);
}


TimidityMidiScheduler::~TimidityMidiScheduler()
{
    // if playing, stop first!
    //if (running()) stop(-1);
    //mid_song_free (song);
    //mid_exit();
}

void TimidityMidiScheduler::SetSong(MidSong *msong)
{
    song=msong;
}

const char* TimidityMidiScheduler::impl_implementationName() const
{
    return "TimidityMidiScheduler version 0.00 [dev].";
}


const char* TimidityMidiScheduler::impl_portName(int port) const
{
  if (port > numPorts())
    return NULL;
  return "Timidity port";
}

const char* TimidityMidiScheduler::impl_portType(int port) const
{
  if (port > numPorts()) return NULL;
  return "SW Synth";
}


bool TimidityMidiScheduler::impl_portReadable(int port) const {
  if (port == 0)
    return true;
  else
    return false;
}

bool TimidityMidiScheduler::impl_portWriteable(int port) const {
  return !portReadable(port);
}


void TimidityMidiScheduler::impl_tx(MidiCommand mc)
{
    if (!song || mc.port != 0)
        return;

    MidEvent event;
    event.time=0;
    event.channel=mc.channel;
    switch (mc.status)
    {
    case MidiCommand_NoteOff: event.type=ME_NOTEOFF; break;
    case MidiCommand_NoteOn: event.type=ME_NOTEON; break;
    case MidiCommand_KeyPressure: event.type=ME_KEYPRESSURE; break;
    //case MidiCommand_ControlChange: event.type=ME_CONTROL; break;
    case MidiCommand_ProgramChange: event.type=ME_PROGRAM; break;
    //case MidiCommand_ChannelPressure: event.type=ME_CHANNE; break;
    case MidiCommand_PitchBend: event.type=ME_PITCHWHEEL; break;
    default: return;
    }
    event.a=mc.data1;
    event.b=mc.data2;
    mid_song_event(song, &event);
}

void TimidityMidiScheduler::impl_start(Clock s)
{
    if (!running()) {
        start_ticks();

       wstartClock = get_ticks();
       clockStarted(s);
    }
}


void TimidityMidiScheduler::impl_stop(Clock t)
{
    if (!running()) return;
    end_ticks();
    clockStopped(t);
}


void TimidityMidiScheduler::impl_moveTo(Clock moveTime, Clock newTime)
{
    clockMoved(moveTime, newTime);
}


Clock TimidityMidiScheduler::impl_clock()
{
    int time = get_ticks() - wstartClock;
    return msToClock(time);
}


int TimidityMidiScheduler::impl_msecs()
{
    return get_ticks() - wstartClock;
}


void TimidityMidiScheduler::impl_setTempo(int newTempo, Clock changeTime)
{
  tempoChanged(newTempo, changeTime);
}


bool TimidityMidiScheduler::impl_eventWaiting()
{
    return false;
}


MidiEvent TimidityMidiScheduler::impl_rx()
{
    return MidiEvent();
}

void TimidityMidiScheduler::impl_tx(MidiEvent e)
{
    impl_tx(e.data);

/*
  unsigned int msecs = clockToMs(e.time);
  if (msecs > timeGetTime()) {
    if (  e.data.port < midiInGetNumDevs()
       || e.data.port > numPorts())
          return;
    CallbackData* data = new CallbackData;
    data->port = hMidi[e.data.port].out;
    data->e    = e.data;
    data->sch  = this;
    timeSetEvent(msecs-timeGetTime(), 10, &callback, (DWORD) data, TIME_ONESHOT);
  } else
    tx(e.data);
*/
}


void TimidityMidiScheduler::impl_txSysEx(int port,
                                      const unsigned char* data, size_t size)
{
     std::cerr << "No implemented Sys Ex in Timidity" << endl;
}


void TimidityMidiScheduler::start_ticks()
{
#ifdef __WIN32__
    TIMECAPS timecaps;
    timeGetDevCaps(&timecaps, sizeof(timecaps));
    timeBeginPeriod(10);
#endif
}

int TimidityMidiScheduler::get_ticks()
{
#ifdef __WIN32__
    return timeGetTime();
#elif defined(__unix__)

    // from SDL_GetTicks
#if HAVE_CLOCK_GETTIME
    int ticks;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    ticks =
        (now.tv_sec) * 1000 + (now.tv_nsec) / 1000000;
    return (ticks);
#else
    int ticks;
    struct timeval now;
    gettimeofday(&now, NULL);
    ticks =
        (now.tv_sec) * 1000 + (now.tv_usec) / 1000;
    return (ticks);
#endif

#else
    return 0;
#endif
}

void TimidityMidiScheduler::end_ticks()
{
#ifdef __WIN32__
    timeEndPeriod(10);
#endif
}
