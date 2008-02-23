#ifndef H_KMBACKEND_SDL_H
#define H_KMBACKEND_SDL_H

#include "KMBackend.h"
#include "KMUtil.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

namespace KaraokeMachine {

class KMBackend_SDL : public KMBackend
{
public:
    KMBackend_SDL();
    virtual ~KMBackend_SDL();

    //virtual void ShowImage(KMImage *image);

    virtual bool Loop(KMachine &machine);
    virtual void SkipImage();
private:
    SDL_Surface *screen_, *bg_;
    TTF_Font *font_;
};

};

#endif //H_KMVIDEO_SDL_H
