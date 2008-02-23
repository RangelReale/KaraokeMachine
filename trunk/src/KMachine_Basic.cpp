#include "KMachine_Basic.h"


namespace KaraokeMachine {

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

};
