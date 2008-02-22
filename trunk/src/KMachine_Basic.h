#ifndef H_KMACHINE_BASIC_H
#define H_KMACHINE_BASIC_H

#include "KMachine.h"
#include "KMUtil.h"
#include "KMBackend.h"
#include "KMSong.h"

namespace KaraokeMachine {

class KMachine_Basic : public KMachine {
public:
    KMachine_Basic(KMBackend &backend);
    virtual ~KMachine_Basic() {}

    virtual void Run();
private:
    KMBackend *backend_;
};

};

#endif //H_KMACHINE_BASIC_H
