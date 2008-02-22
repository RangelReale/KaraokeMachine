#ifndef H_KMSONG_TSE3_H
#define H_KMSONG_TSE3_H

#include "KMUtil.h"
#include "KMSong.h"

#include "tse3/MidiFile.h"
#include "tse3/Metronome.h"
#include "tse3/util/MidiScheduler.h"
#include "tse3/Transport.h"
#ifdef __WIN32__
#include "tse3/plt/Win32.h"
#elif defined(unix)
//#include "tse3/plt/OSS.h"
#include "tse3/plt/Alsa.h"
#else
    #error "Unknown platform"
#endif
#include "tse3/Playable.h"
#include "tse3/Song.h"

#include <string>
#include <map>

namespace KaraokeMachine {

/**
 * @class KMSong_TSE3_Lyric
 *
 * @brief TSE3 lyrics
 */
class KMSong_TSE3_Lyric
{
public:
    KMSong_TSE3_Lyric(TSE3::Clock clock, int line, int idxstart, int idxend,
        const std::string &text) :
        clock_(clock), line_(line), idxstart_(idxstart), idxend_(idxend),
        text_(text) {}

    const TSE3::Clock &GetClock() { return clock_; }
    int GetLine() { return line_; }
    int GetIDXStart() { return idxstart_; }
    int GetIDXEnd() { return idxend_; }
    const std::string &GetText() { return text_; }
private:
    TSE3::Clock clock_;
    int line_;
    int idxstart_, idxend_;
    std::string text_;
};

/**
 * @class KMSong_TSE3
 *
 * @brief TSE3 song
 */
class KMSong_TSE3 : public KMSong {
public:
    KMSong_TSE3(KMInputStream &stream);
    KMSong_TSE3(const std::string &filename);

    virtual ~KMSong_TSE3();

    virtual bool Play();
    virtual bool Stop();
    virtual bool Pause();
    virtual bool Poll();

    virtual int GetTranspose() { return transport_.filter()->transpose(); }
    virtual void SetTranspose(int t) { transport_.filter()->setTranspose(t); }
    virtual void SetMelodyTrack(int t);
protected:
    bool Load(KMInputStream &stream);
private:
    typedef std::map<TSE3::Clock, KMSong_TSE3_Lyric> lyrics_t;

    void LoadLyrics();

    TSE3::Song *song_;
    TSE3::Metronome                 metronome_;
#ifdef __WIN32__
    static TSE3::Plt::Win32MidiScheduler   scheduler_;
#elif defined(unix)
    //static TSE3::Plt::OSSMidiScheduler      scheduler_;
    static TSE3::Plt::AlsaMidiScheduler        scheduler_;
#else
    #error "Unknown platform"
#endif
    TSE3::Transport                 transport_;

    lyrics_t lyrics_;
    int melodytrack_, melodytrackvolume_;

    friend class KMSong_TSE3_Callback;
};

};

#endif //H_KMSONG_TSE3_H
