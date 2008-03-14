#include "KMachine_Basic.h"


namespace KaraokeMachine {

class KMachinePlaySongProcess : public KMBackendThreadProcess
{
public:
    KMachinePlaySongProcess(KMSong *song) : KMBackendThreadProcess(), song_(song) {};
protected:
    virtual void Run();
private:
    KMSong *song_;
};

void KMachinePlaySongProcess::Run()
{
    song_->Play();
    while (!IsCancel() && song_->Poll())
        kmutil_usleep(100);
}


KMachine_Basic::KMachine_Basic(KMBackend &backend) :
    KMachine(), backend_(&backend), songplayprocess_(NULL)
{


}

void KMachine_Basic::DoCommand(commant_t command, int param )
{
    switch(command)
    {
    case KC_SKIPIMAGE:
        backend_->SkipImage();
        break;
    default:
        KMachine::DoCommand(command, param);
        break;
    }
}

void KMachine_Basic::DoRun()
{
    //KMachine::DoRun();
    while (backend_->Loop(*this))
    {
        Loop();
    }
}

void KMachine_Basic::PlaySong(KMSong *song)
{
    songplayprocess_=new KMachinePlaySongProcess(song);
    backend_->CreateThread(songplayprocess_);
}

void KMachine_Basic::StopSong()
{
    if (songplayprocess_)
    {
        songplayprocess_->Cancel();
        songplayprocess_=NULL;
    }
}

};
