#include "KMSong_TSE3.h"

#include <tse3/TextTrack.h>
#include <tse3/Track.h>
#include <tse3/MidiParams.h>
#include <fstream>

namespace KaraokeMachine {

/////////////////////////////////
// CLASS
//      KMSong_TSE3_Callback
/////////////////////////////////

class KMSong_TSE3_Callback : public TSE3::TransportCallback
{
public:
    KMSong_TSE3_Callback(KMSong_TSE3 *song) : TSE3::TransportCallback(), song_(song) {}

    virtual void 	Transport_MidiIn (TSE3::MidiCommand c) {
    }
    virtual void 	Transport_MidiOut (TSE3::MidiCommand c) {
        if (c.status==TSE3::MidiCommand_NoteOn)
            song_->trackplay_[c.channel]++;
        else if (c.status==TSE3::MidiCommand_NoteOff)
            song_->trackplay_[c.channel]--;
    }
private:
    KMSong_TSE3 *song_;
};

/////////////////////////////////
// CLASS
//      KMSong_TSE3
/////////////////////////////////
#ifdef __WIN32__
TSE3::Plt::Win32MidiScheduler KMSong_TSE3::scheduler_;
#elif defined(unix)
//TSE3::Plt::OSSMidiScheduler KMSong_TSE3::scheduler_;
TSE3::Plt::AlsaMidiScheduler KMSong_TSE3::scheduler_;
#else
    #error "Unknown platform"
#endif

KMSong_TSE3::KMSong_TSE3(KMInputStream &stream) :
    KMSong(stream), metronome_(), /*scheduler_(), */transport_(&metronome_, &scheduler_),
    lyrics_(), trackplay_(), melodytrack_(-1), melodytrackvolume_(0)
{
    if (!Load(stream))
        throw KMException("Could not load song");
}

KMSong_TSE3::KMSong_TSE3(const std::string &filename) :
    KMSong(filename), metronome_(), /*scheduler_(), */transport_(&metronome_, &scheduler_),
    lyrics_(), trackplay_(), melodytrack_(-1), melodytrackvolume_(0)
{
    std::ifstream in(filename.c_str(), std::ifstream::in|std::ifstream::binary);
    if (!Load(in))
        throw KMException("Could not load song");
}


KMSong_TSE3::~KMSong_TSE3()
{
    if (song_) {
        Stop();

        delete song_;
    }
}

bool KMSong_TSE3::Load(KMInputStream &stream)
{
    TSE3::MidiFileImport mfi(stream);

    //transport_.filter()->setPort(128);
    transport_.attachCallback(new KMSong_TSE3_Callback(this));

    song_ = mfi.load();
    trackplay_.clear();
    for (unsigned int i=0; i<song_->size(); i++)
        trackplay_.push_back(false);
    LoadLyrics();

    return true;
}

void KMSong_TSE3::LoadLyrics()
{
    TSE3::PlayableIterator *txti;
    txti=song_->textTrack()->iterator(0);

    std::string ln(""), curln;

    bool islyric;
    while (txti->more())
    {
        curln=(*txti)->data.str;
        islyric=true;

        if (curln.substr(0, 1)=="@")
        {
            // title
            curln.erase(0, 1);
            SetTitle(curln);
            islyric=false;
        }
        else if (curln.substr(0, 1)=="\\" || curln.substr(0, 1)=="/")
        {
            if (!ln.empty()) AddLyricsLine(ln);
            ln="";
            curln.erase(0, 1);
        }
        if (islyric)
        {
            lyrics_.insert( std::pair<TSE3::Clock, KMSong_TSE3_Lyric>((*txti)->time,
                KMSong_TSE3_Lyric((*txti)->time, Lyrics().GetCount(), ln.size(), ln.size()+curln.size(), curln) ));
            ln=ln+curln;
        }
        ++(*txti);
    }
    if (!ln.empty()) AddLyricsLine(ln);
    delete txti;

}


bool KMSong_TSE3::Play()
{
    transport_.play(song_, 0);
    return true;
}

bool KMSong_TSE3::Stop()
{
    transport_.stop();
    return false;
}

bool KMSong_TSE3::Pause()
{
    return false;
}

bool KMSong_TSE3::Poll()
{
    if (transport_.status() == TSE3::Transport::Resting) return false;

    transport_.poll();


    int ppos=song_->textTrack()->index(transport_.scheduler()->clock(), false);

    lyrics_t::iterator i=lyrics_.find((*song_->textTrack())[ppos].time);
    if (i!=lyrics_.end())
    {
        SetDebug(kmutil_format("[%d] %d - %d = %s",
            transport_.scheduler()->clock().pulses,
            i->second.GetClock().pulses,
            (*song_->textTrack())[ppos].time.pulses, i->second.GetText().c_str()));

        std::string cln=Lyrics().GetLine(i->second.GetLine()).GetLine();
/*
        SetDebug(kmutil_format("%d - %d = %d",
            i->second.GetIDXStart(),
            i->second.GetIDXEnd(),
            cln.length()));
*/


        SetLyricsCurrentLine(i->second.GetLine());
        if (i->second.GetIDXEnd()>0)
            SetLyricsCurrentPosPctLine((int)((100.0*i->second.GetIDXEnd())/(float)cln.length()));
        else
            SetLyricsCurrentPosPctLine(0);
    }


/*
    TSE3::PlayableIterator *txti;
    txti=song_->textTrack()->iterator(0);
    txti->moveTo(transport_.scheduler()->clock());
    if (txti->more())
    {
        lyrics_t::iterator i=lyrics_.find((*txti)->time);
        if (i!=lyrics_.end())
        {
            if (transport_.scheduler()->clock()!=(*txti)->time)
                i--;

            SetDebug(kmutil_format("[%d] %d - %d = %s",
                transport_.scheduler()->clock().pulses,
                i->second.GetClock().pulses,
                (*txti)->time.pulses, i->second.GetText().c_str()));

            SetLyricsCurrentLine(i->second.GetLine());
            //SetLyricsCurrentPosPctLine
        }
        else
        {
            SetDebug("not found");
        }

    }
    else
    {
        SetDebug("not found 2");
    }
    delete txti;
*/

    return true;
}

void KMSong_TSE3::SetMelodyTrack(int t)
{
    if (melodytrack_>-1)
    {
        //(*song_)[melodytrack_]->params()->setVolume(TSE3::MidiParams::off);
        (*song_)[melodytrack_]->filter()->setStatus(true);
        melodytrack_=-1;
    }
    if (t>-1 && (unsigned)t>song_->size()-1)
        t=-1;
    if (t>-1)
    {
        //melodytrackvolume_=(*song_)[t]->params()->volume();
        //(*song_)[t]->params()->setVolume(0);
        (*song_)[t]->filter()->setStatus(false);

    }
    melodytrack_=t;
}

int KMSong_TSE3::GetTrackCount()
{
    return song_->size();
}

bool KMSong_TSE3::GetTrackPlaying(int trackindex)
{
    return trackplay_[trackindex]>0;
}


};
