#include "KMachine_Basic.h"


namespace KaraokeMachine {

KMachine_Basic::KMachine_Basic(KMBackend &backend) :
    KMachine(), backend_(&backend)
{


}


void KMachine_Basic::Run()
{
    while (backend_->Loop(*this))
    {
        Loop();
    }
}

};
