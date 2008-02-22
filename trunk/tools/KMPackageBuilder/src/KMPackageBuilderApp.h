/***************************************************************
 * Name:      KMPackageBuilderApp.h
 * Purpose:   Defines Application Class
 * Author:     ()
 * Created:   2008-02-19
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef KMPACKAGEBUILDERAPP_H
#define KMPACKAGEBUILDERAPP_H

#include <wx/app.h>
#include <wx/mdi.h>
#include <wx/docview.h>
#include <wx/docmdi.h>

class KMPackageBuilderApp : public wxApp
{
public:
    KMPackageBuilderApp(void);

    virtual bool OnInit();
    virtual int OnExit(void);

    virtual bool OnExceptionInMainLoop();
    virtual void OnUnhandledException();

    wxMDIChildFrame *CreateChildFrame(wxDocument *doc, wxView *view, bool issong);
protected:
    enum {
        ID_MENUABOUT = wxID_HIGHEST
    };

    wxDocManager* docManager_;
};

DECLARE_APP(KMPackageBuilderApp)

#endif // KMPACKAGEBUILDERAPP_H
