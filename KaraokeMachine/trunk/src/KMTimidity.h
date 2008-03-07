#ifndef H_KMTIMIDITY_H
#define H_KMTIMIDITY_H

// use timidity
#ifdef KM_USE_TIMIDITY

#include <SDL_thread.h>
#include <SDL_mutex.h>
#include <timidity.h>

namespace KaraokeMachine {

class KM_Timidity
{
public:
    KM_Timidity();
    ~KM_Timidity();


    static KM_Timidity *get();

    void Lock();
    void Unlock();

    MidSong *InitSong();
    void StopSong();

    MidSong *GetSong() { return song_; }
    MidSongOptions *GetSongOptions() { return &options_; }
private:
    MidSong *song_;
    MidSongOptions options_;
    SDL_mutex *mutex_;
};

};

#endif

#endif //H_KMTIMIDITY_H
