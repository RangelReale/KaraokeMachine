#ifndef H_KMBACKEND_SDL_H
#define H_KMBACKEND_SDL_H

#include "KMBackend.h"
#include "KMUtil.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>

namespace KaraokeMachine {

class KMBackendThread_SDL : public KMBackendThread {
public:
    KMBackendThread_SDL(KMBackendThreadProcess *process) :
        KMBackendThread(process), thread_(NULL) {}
    void SetThread(SDL_Thread *thread) { thread_=thread; }
private:
    SDL_Thread *thread_;
};

class KMBackend_SDL : public KMBackend
{
public:
    KMBackend_SDL();
    virtual ~KMBackend_SDL();

    //virtual void ShowImage(KMImage *image);

    virtual bool Loop(KMachine &machine);
    virtual void SkipImage();
    virtual KMBackendThread* CreateThread(KMBackendThreadProcess *process);
private:
    int displaywidth_, displayheight_, fontsize_;
    int FPS, lastFPS, pastFPS, past;
    SDL_Surface *screen_, *bg_;
    TTF_Font *font_;
};

};

#endif //H_KMVIDEO_SDL_H
