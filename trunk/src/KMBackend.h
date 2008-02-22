#ifndef H_KMBACKEND_H
#define H_KMBACKEND_H

#include "KMUtil.h"
#include "KMachine.h"

namespace KaraokeMachine {

class KMBackend {
public:
    KMBackend() {}
    virtual ~KMBackend() {}

    //virtual void ShowImage(KMImage *image) = 0;

    virtual bool Loop(KMachine &machine) = 0;
private:

};

};

#endif //H_KMBACKEND_H
