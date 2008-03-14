#ifndef H_KMBACKEND_H
#define H_KMBACKEND_H

#include "KMUtil.h"
#include "KMachine.h"

namespace KaraokeMachine {

class KMBackendThreadProcess {
public:
    KMBackendThreadProcess() { iscancel_=false; }
    void Cancel() { iscancel_=true; }
protected:
    virtual void Run() = 0;
    bool IsCancel() { return iscancel_; }

    friend class KMBackendThread;
private:
    bool iscancel_;
};

class KMBackendThread {
public:
    KMBackendThread(KMBackendThreadProcess *process) : process_(process) {}
    ~KMBackendThread() { if (process_) delete process_; }

    void Run() { if (process_) process_->Run(); }
private:
    KMBackendThreadProcess *process_;
};

class KMBackend {
public:
    KMBackend() {}
    virtual ~KMBackend() {}

    //virtual void ShowImage(KMImage *image) = 0;

    virtual bool Loop(KMachine &machine) = 0;
    virtual void SkipImage() {};

    virtual KMBackendThread* CreateThread(KMBackendThreadProcess *process) = 0;
private:

};

};

#endif //H_KMBACKEND_H
