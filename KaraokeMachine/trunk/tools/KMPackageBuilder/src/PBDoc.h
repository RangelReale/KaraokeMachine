#ifndef _H_PBDOC_H_
#define _H_PBDOC_H_

#include <wx/docview.h>
#include <wx/cmdproc.h>

#include "KMSong.h"
#include "KMImage.h"

using namespace KaraokeMachine;

class PBSongDocument: public wxDocument
{
    DECLARE_DYNAMIC_CLASS(PBSongDocument)
public:
    PBSongDocument(void) : wxDocument(), songpackage_(0) {}
    ~PBSongDocument(void);

    virtual bool OnNewDocument();
    virtual bool OnSaveDocument(const wxString& filename);
    virtual bool OnOpenDocument(const wxString& filename);
/*
    virtual bool IsModified(void) const;
    virtual void Modify(bool mod);
*/
private:
    KMSongPackage songpackage_;

    friend class PBSongView;
};

class PBImageDocument: public wxDocument
{
    DECLARE_DYNAMIC_CLASS(PBImageDocument)
private:
public:
    PBImageDocument(void) : wxDocument(), imagepackage_(0) {}
    ~PBImageDocument(void);

    virtual bool OnNewDocument();
    virtual bool OnSaveDocument(const wxString& filename);
    virtual bool OnOpenDocument(const wxString& filename);
private:
    KMImagePackage imagepackage_;

    friend class PBImageView;
};

#endif //_H_PBDOC_H_
