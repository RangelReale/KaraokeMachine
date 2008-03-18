#include <sstream>
#include "KMBackend_SDL.h"
#ifdef GP2X
#include <SDL_rotozoom.h>
#endif

namespace KaraokeMachine {

// use timidity
#ifdef KM_USE_TIMIDITY
void KMBackend_SDL_AudioCallback(void *userdata, Uint8 *stream, int len)
{
    KMBackend_SDL *be=(KMBackend_SDL*)userdata;

    //SDL_mutexP(be->GetMutex());
    KM_Timidity::get()->Lock();

    if (KM_Timidity::get()->GetSong())
        mid_song_read_wave (KM_Timidity::get()->GetSong(), stream, len);

    KM_Timidity::get()->Unlock();
    //SDL_mutexV(be->GetMutex());
}
#endif

KMBackend_SDL::KMBackend_SDL() :
    KMBackend(), screen_(NULL), bg_(NULL)
{
    FPS = lastFPS = pastFPS = past = 0;

    // initialize SDL video
    int sdlflags=SDL_INIT_VIDEO;
#ifdef GP2X
    displaywidth_=320;
    displayheight_=240;
    fontsize_=12;
    sdlflags|=SDL_INIT_JOYSTICK;
#else
    displaywidth_=800;
    displayheight_=600;
    fontsize_=22;
#endif

// use timidity
#ifdef KM_USE_TIMIDITY
    sdlflags|=SDL_INIT_AUDIO;
#endif

    if ( SDL_Init( sdlflags ) < 0 )
    {
        throw KMException( kmutil_format("Unable to init SDL: %s", SDL_GetError()) );
    }


    mutex_=SDL_CreateMutex();

// use timidity
#ifdef KM_USE_TIMIDITY
    SDL_AudioSpec aspec, aspecget;

    memset(&aspec, 0, sizeof(aspec));
    aspec.freq=KM_Timidity::get()->GetSongOptions()->rate;
    aspec.format=(KM_Timidity::get()->GetSongOptions()->format==MID_AUDIO_S16?AUDIO_S16:AUDIO_S8);
    aspec.channels=KM_Timidity::get()->GetSongOptions()->channels;
#ifndef GP2X
    aspec.samples=2048;
#else
    aspec.samples=128;
#endif
    aspec.callback=KMBackend_SDL_AudioCallback;
    aspec.userdata=this;

    if ( SDL_OpenAudio( &aspec, &aspecget ) < 0 )
    {
        throw KMException( kmutil_format("Unable to init SDL: %s", SDL_GetError()) );
    }

#endif

#ifdef GP2X
    SDL_JoystickOpen(0);
#endif

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
#ifdef GP2X
    screen_ = SDL_SetVideoMode(displaywidth_, displayheight_, 16, SDL_SWSURFACE|SDL_DOUBLEBUF);
#else
    screen_ = SDL_SetVideoMode(displaywidth_, displayheight_, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
#endif //GP2X


    SDL_ShowCursor(0);


    if ( !screen_ )
    {
        throw KMException( kmutil_format("Unable to set 640x480 video: %s", SDL_GetError()));
    }

    if ( TTF_Init() < 0 )
    {
        throw KMException( kmutil_format("Unable to init SDL_ttf: %s", TTF_GetError() ));
    }


#ifdef __WIN32__
    font_=TTF_OpenFont("c:\\windows\\fonts\\LUCON.TTF", fontsize_);
#elif defined(GP2X)
    font_=TTF_OpenFont("/mnt/sd/game/app/k/fonts/dejavusans.ttf", fontsize_);
#elif defined(unix)
    font_=TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", fontsize_);
#else
    #error "Unknown platform"
#endif
    if ( !font_ )
    {
        throw KMException( "Unable to load font" );
    }


// use timidity
#ifdef KM_USE_TIMIDITY
    SDL_PauseAudio(0);
#endif
}

KMBackend_SDL::~KMBackend_SDL()
{
// use timidity
#ifdef KM_USE_TIMIDITY
    SDL_PauseAudio(1);
#endif

    TTF_CloseFont(font_);

    if (bg_)
    {
        SDL_FreeSurface(bg_);
        SDL_DestroyMutex(mutex_);
    }
}

void KMBackend_SDL::SkipImage()
{
    if (bg_)
    {
        SDL_FreeSurface(bg_);
        bg_=NULL;
    }
}

/*
void KMBackend_SDL::ShowImage(KMImage *image)
{

}
*/
bool KMBackend_SDL::Loop(KMachine &machine)
{
    SDL_Surface *text_surface;
    SDL_Color black = {0xFF,0xFF,0xFF};
    SDL_Color red = {0xFF,0,0};
#ifdef GP2X
    SDL_Rect dst  = {10, 10, 30, 30}, dstpl, srcpl;
#else
    SDL_Rect dst  = {100, 100, 300, 300}, dstpl, srcpl;
#endif

    // program main loop
/*
    if(transport.status() != TSE3::Transport::Resting)
        transport.poll();
*/

    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
            // exit if the window is closed
        case SDL_QUIT:
            return false;
            break;

            // check for keypresses
        case SDL_KEYDOWN:
            {
                // exit if ESCAPE is pressed
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return false;


                switch (event.key.keysym.sym)
                {
                case SDLK_ASTERISK: machine.AddChar('*'); break;
                case SDLK_BACKSPACE: machine.RemoveChar(); break;
                case SDLK_PLUS:
                case SDLK_KP_PLUS:
                    machine.DoCommand(KMachine::KC_SONGKEYUP);
                    break;
                case SDLK_MINUS:
                case SDLK_KP_MINUS:
                    machine.DoCommand(KMachine::KC_SONGKEYDOWN);
                    break;
                case SDLK_0: machine.AddChar('0'); break;
                case SDLK_1: machine.AddChar('1'); break;
                case SDLK_2: machine.AddChar('2'); break;
                case SDLK_3: machine.AddChar('3'); break;
                case SDLK_4: machine.AddChar('4'); break;
                case SDLK_5: machine.AddChar('5'); break;
                case SDLK_6: machine.AddChar('6'); break;
                case SDLK_7: machine.AddChar('7'); break;
                case SDLK_8:
                {
                    if ((event.key.keysym.mod&KMOD_LSHIFT)==KMOD_LSHIFT ||
                        (event.key.keysym.mod&KMOD_RSHIFT)==KMOD_RSHIFT)
                        machine.AddChar('*');
                    else
                        machine.AddChar('8');
                    break;
                }
                case SDLK_9: machine.AddChar('9'); break;
                case SDLK_RETURN: machine.DoCommand(KMachine::KC_ADD); break;
                case SDLK_SPACE: machine.DoCommand(KMachine::KC_SKIP); break;
                case SDLK_TAB: machine.DoCommand(KMachine::KC_SKIPIMAGE); break;
                default:
                    break;
                }
                break;
            }
#ifdef GP2X
        case SDL_JOYBUTTONDOWN:
            {
                switch(event.jbutton.button)
                {
                    case GP2X_VK_FX: /* X pressed */
                      {
                        machine.AddString("5532*4");
                        break;
                      }
                    case GP2X_VK_FA: /* A pressed */
                      {
                        machine.AddString("5532*1");
                        break;
                      }
                    case GP2X_VK_FB: /* X pressed */
                      {
                        machine.AddString("5532*2");
                        break;
                      }
                    case GP2X_VK_FY: /* L pressed */
                        machine.DoCommand(KMachine::KC_ADD);
                        break;
                    case GP2X_VK_START:
                        return false;
                    case GP2X_VK_FR:
                        machine.DoCommand(KMachine::KC_SKIP);
                        break;
                    case GP2X_VK_SELECT:
                        machine.DoCommand(KMachine::KC_SKIPIMAGE);
                        break;
                }
            }
        case SDL_JOYBUTTONUP:
            {

            }

#endif
        } // end switch
    } // end of message processing

    // LOAD IMAGE
    if (!bg_ && machine.ImageList().GetCount()>0)
    {
        SDL_FreeSurface(bg_);
        bg_=NULL;

        SDL_Surface *loadbg;

        std::stringstream imagedata;
        std::string sdata;
        machine.ImageList().GetRandom().GetImage()->GetFileData(imagedata);
        sdata=imagedata.str();
        SDL_RWops *sdlimage=SDL_RWFromConstMem(sdata.data(), sdata.length());
        loadbg=IMG_LoadJPG_RW(sdlimage);
        SDL_FreeRW(sdlimage);

        if (loadbg)
        {
#ifdef GP2X
            SDL_Surface *oldbg=loadbg;
            loadbg=zoomSurface(oldbg, 0.3, 0.3, 0);
            SDL_FreeSurface(oldbg);
#else
            //bg_=SDL_CreateRGBSurface(SDL_SWSURFACE, displaywidth_, displayheight_, 16, 0, 0, 0, 0);
            //SDL_Rect irect = {0, 0, displaywidth_, displayheight_};
            //SDL_BlitSurface(loadbg, NULL, bg_, NULL);
#endif
            bg_=SDL_DisplayFormat(loadbg);
            SDL_FreeSurface(loadbg);
        }
    }


    // DRAWING STARTS HERE

    // clear screen
    /* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(screen_) ) {
        if ( SDL_LockSurface(screen_) < 0 ) {
            throw KMException( kmutil_format("Can't lock screen: %s", SDL_GetError()) );
        }
    }
    SDL_FillRect(screen_, 0, SDL_MapRGB(screen_->format, 0, 0, 0));
    if ( SDL_MUSTLOCK(screen_) ) {
        SDL_UnlockSurface(screen_);
    }

    if (bg_)
    {
        SDL_Rect irect = {0, 0, displaywidth_, displayheight_};
        SDL_BlitSurface(bg_, &irect, screen_, &irect);
    }


    text_surface = TTF_RenderText_Blended(font_, "Karaoke Machine 0.2", black);
    if (text_surface != NULL)
    {
        //dst.x=20;
        //dst.y=
        SDL_BlitSurface(text_surface, NULL, screen_, NULL);
        SDL_FreeSurface(text_surface);
    }

    text_surface = TTF_RenderText_Blended(font_, machine.GetChars().c_str(), black);
    if (text_surface != NULL)
    {
        //dst.x=20;
        //dst.y=
        SDL_BlitSurface(text_surface, NULL, screen_, &dst);
        SDL_FreeSurface(text_surface);
    }

    std::string pldesc;

    // FPS
    pldesc=kmutil_format("FPS: %d", lastFPS);

    text_surface = TTF_RenderText_Blended(font_, pldesc.c_str(), black);
    if (text_surface != NULL)
    {
        dst.y=dst.y+40;
        SDL_BlitSurface(text_surface, NULL, screen_, &dst);
        SDL_FreeSurface(text_surface);
    }

#ifdef GP2X
    dstpl.x=20; dstpl.y=20;
    dstpl.w=130; dstpl.h=130;
#else
    dstpl.x=200; dstpl.y=200;
    dstpl.w=600; dstpl.h=600;
#endif
    for (int i=0; i<machine.Playlist().GetCount(); i++)
    {
        pldesc=kmutil_format("%d:%d - %s", machine.Playlist().Get(i).GetPackageId(),
            machine.Playlist().Get(i).GetSongId(), machine.Playlist().Get(i).GetSong()->GetTitle().c_str());

        text_surface = TTF_RenderText_Blended(font_, pldesc.c_str(), black);
        if (text_surface != NULL)
        {
            SDL_BlitSurface(text_surface, NULL, screen_, &dstpl);
            SDL_FreeSurface(text_surface);
            dstpl.y+=15;
        }

    }

    if (machine.Playing())
    {
#ifdef GP2X
        dstpl.x=0; dstpl.y=20;
        dstpl.w=130; dstpl.h=130;
#else
        dstpl.x=0; dstpl.y=200;
        dstpl.w=600; dstpl.h=600;
#endif

        //if (machine.Playing()->GetLyricsCurrentLine()>=0)
        //{
            for (int i=(machine.Playing()->GetLyricsCurrentLine()>=0?machine.Playing()->GetLyricsCurrentLine():0);
                i<machine.Playing()->Lyrics().GetCount(); i++)
            {
                text_surface = TTF_RenderText_Blended(font_, machine.Playing()->Lyrics().GetLine(i).GetLine().c_str(), black);
                if (text_surface != NULL)
                {
                    SDL_BlitSurface(text_surface, NULL, screen_, &dstpl);
                    SDL_FreeSurface(text_surface);
                }
                if (i==machine.Playing()->GetLyricsCurrentLine())
                {
                    int w, h;
                    TTF_SizeText(font_, machine.Playing()->Lyrics().GetLine(i).GetLine().c_str(), &w, &h);

                    text_surface = TTF_RenderText_Blended(font_, machine.Playing()->Lyrics().GetLine(i).GetLine().c_str(), red);
                    if (text_surface != NULL)
                    {
                        srcpl.x=0; srcpl.y=0;
                        srcpl.w=(int)((float)w*(machine.Playing()->GetLyricsCurrentPosPct()/100.0));
                        srcpl.h=h;
                        SDL_BlitSurface(text_surface, &srcpl, screen_, &dstpl);
                        SDL_FreeSurface(text_surface);
                    }
                }


                dstpl.y+=15;
                if (dstpl.y>600) break;
            }
        //}

        if (!machine.Playing()->GetDebug().empty())
        {
            text_surface = TTF_RenderText_Blended(font_, machine.Playing()->GetDebug().c_str(), black);
            if (text_surface != NULL)
            {
#ifdef GP2X
                dstpl.x=110;
                dstpl.y=0;
#else
                dstpl.x=300;
                dstpl.y=0;
#endif
                SDL_BlitSurface(text_surface, NULL, screen_, &dstpl);
                SDL_FreeSurface(text_surface);
            }
        }

        text_surface = TTF_RenderText_Blended(font_, kmutil_format("%d", machine.Playing()->GetLyricsCurrentPosPct()).c_str(), black);
        if (text_surface != NULL)
        {
#ifdef GP2X
            dstpl.x=110;
            dstpl.y=5;
#else
            dstpl.x=300;
            dstpl.y=25;
#endif
            SDL_BlitSurface(text_surface, NULL, screen_, &dstpl);
            SDL_FreeSurface(text_surface);
        }

    }



    // DRAWING ENDS HERE

    int currentTime = SDL_GetTicks();

    if ( currentTime - past >= 16 )
    {
      past = SDL_GetTicks();   // this should be done before redrawing screen

      SDL_Flip( screen_ );
      FPS++;
      //kmutil_usleep(1 * 1000);
    }

    if ( currentTime - pastFPS >= 1000 )
    {
      lastFPS = FPS;
      FPS = 0;
      pastFPS = currentTime;
    }

    // finally, update the screen :)
    //SDL_Flip(screen_);

    return true;
}

int thread_func(void *data)
{
    KMBackendThread_SDL *t=(KMBackendThread_SDL *)data;
    t->Run();
    delete t;
    return 0;
}

KMBackendThread* KMBackend_SDL::CreateThread(KMBackendThreadProcess *process)
{
    KMBackendThread_SDL *newt=new KMBackendThread_SDL(process);
    SDL_Thread *thread = SDL_CreateThread(thread_func, newt);
    newt->SetThread(thread);
    return newt;
}


};
