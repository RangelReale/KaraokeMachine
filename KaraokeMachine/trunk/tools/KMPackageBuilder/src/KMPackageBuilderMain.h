/***************************************************************
 * Name:      KMPackageBuilderMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2008-02-19
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef KMPACKAGEBUILDERMAIN_H
#define KMPACKAGEBUILDERMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "KMPackageBuilderApp.h"

class KMPackageBuilderFrame: public wxDocMDIParentFrame
{
    DECLARE_CLASS(KMPackageBuilderFrame)
public:
    KMPackageBuilderFrame(wxFrame *frame, const wxString& title);

    KMPackageBuilderFrame(wxDocManager *manager, wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size,
        long type);
    ~KMPackageBuilderFrame();
private:
    enum
    {
        ID_MENUQUIT = 1000,
        ID_MENUABOUT
    };
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
};


#endif // KMPACKAGEBUILDERMAIN_H
