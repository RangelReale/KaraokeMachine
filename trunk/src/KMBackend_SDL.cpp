#include "KMBackend_SDL.h"


namespace KaraokeMachine {

KMBackend_SDL::KMBackend_SDL() :
    KMBackend()
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        throw KMException( kmutil_format("Unable to init SDL: %s", SDL_GetError()) );
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen_ = SDL_SetVideoMode(800, 600, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen_ )
    {
        throw KMException( kmutil_format("Unable to set 640x480 video: %s", SDL_GetError()));
    }

    if ( TTF_Init() < 0 )
    {
        throw KMException( kmutil_format("Unable to init SDL_ttf: %s", TTF_GetError() ));
    }


    font_=TTF_OpenFont("c:\\windows\\fonts\\LUCON.TTF", 22);
    if ( !font_ )
    {
        throw KMException( "Unable to load font" );
    }
}

KMBackend_SDL::~KMBackend_SDL()
{
    TTF_CloseFont(font_);
}

void KMBackend_SDL::ShowImage(KMImage *image)
{

}

bool KMBackend_SDL::Loop(KMachine &machine)
{
    SDL_Surface *text_surface;
    SDL_Color black = {0xFF,0xFF,0xFF};
    SDL_Color red = {0xFF,0,0};
    SDL_Rect dst  = {100, 100, 300, 300}, dstpl, srcpl;

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
                default:
                    break;
                }
                break;
            }
        } // end switch
    } // end of message processing

    // DRAWING STARTS HERE

    // clear screen
    SDL_FillRect(screen_, 0, SDL_MapRGB(screen_->format, 0, 0, 0));

    text_surface = TTF_RenderText_Blended(font_, "Karaoke Machine 0.1", black);
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
    dstpl.x=200; dstpl.y=200;
    dstpl.w=600; dstpl.h=600;
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
        dstpl.x=0; dstpl.y=200;
        dstpl.w=600; dstpl.h=600;

        if (machine.Playing()->GetLyricsCurrentLine()>=0)
        {
            for (int i=machine.Playing()->GetLyricsCurrentLine(); i<machine.Playing()->Lyrics().GetCount(); i++)
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
        }

        if (!machine.Playing()->GetDebug().empty())
        {
            text_surface = TTF_RenderText_Blended(font_, machine.Playing()->GetDebug().c_str(), black);
            if (text_surface != NULL)
            {
                dstpl.x=300;
                dstpl.y=0;
                SDL_BlitSurface(text_surface, NULL, screen_, &dstpl);
                SDL_FreeSurface(text_surface);
            }
        }

        text_surface = TTF_RenderText_Blended(font_, kmutil_format("%d", machine.Playing()->GetLyricsCurrentPosPct()).c_str(), black);
        if (text_surface != NULL)
        {
            dstpl.x=300;
            dstpl.y=25;
            SDL_BlitSurface(text_surface, NULL, screen_, &dstpl);
            SDL_FreeSurface(text_surface);
        }

    }



    // DRAWING ENDS HERE

    // finally, update the screen :)
    SDL_Flip(screen_);

    return true;
}


};
