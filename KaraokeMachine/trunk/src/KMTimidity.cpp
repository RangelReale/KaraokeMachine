#include "KMTimidity.h"
#include "KMUtil.h"

// use timidity
#ifdef KM_USE_TIMIDITY

namespace KaraokeMachine {

// global variable
KM_Timidity kmtimidity;


KM_Timidity::KM_Timidity() :
    song_(NULL), options_(), mutex_(NULL)
{
    options_.rate = 44100;
    options_.format = MID_AUDIO_S16;
    options_.channels = 2;
    options_.buffer_size = 44100;

    // initialize timidity
    if (mid_init (NULL) < 0)
        throw KMException( "Unable to init timidity" );
}

KM_Timidity::~KM_Timidity()
{
    StopSong();

    //mid_song_stop(song_);
    mid_exit();
}

MidSong *KM_Timidity::InitSong()
{
    StopSong();

    Lock();

    song_=mid_song_load (NULL, &options_);
    if (!song_)
    {
        Unlock();
        throw KMException( "Unable to init timidity" );
    }

    mid_song_start(song_);
    Unlock();
    return song_;
}

void KM_Timidity::StopSong()
{
    if (song_)
    {
        Lock();
        mid_song_free (song_);
        song_=NULL;
        Unlock();
    }
}


KM_Timidity *KM_Timidity::get()
{
    return &kmtimidity;
}

void KM_Timidity::Lock()
{
    if (!mutex_)
        mutex_=SDL_CreateMutex();
    SDL_mutexP(mutex_);
}

void KM_Timidity::Unlock()
{
    if (!mutex_)
        mutex_=SDL_CreateMutex();
    SDL_mutexV(mutex_);
}

};

#endif //KM_USE_TIMIDITY

