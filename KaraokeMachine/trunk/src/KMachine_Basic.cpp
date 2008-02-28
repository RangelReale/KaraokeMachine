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
    while (song_->Poll())
        kmutil_usleep(100);
}


KMachine_Basic::KMachine_Basic(KMBackend &backend) :
    KMachine(), backend_(&backend)
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
    KMachinePlaySongProcess *ps=new KMachinePlaySongProcess(song);
    backend_->CreateThread(ps);
}

};
