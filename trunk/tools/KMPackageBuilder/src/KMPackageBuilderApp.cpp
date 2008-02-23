/***************************************************************
 * Name:      KMPackageBuilderApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2008-02-19
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/config.h>

#include "KMPackageBuilderApp.h"
#include "KMPackageBuilderMain.h"

#include "PBDoc.h"
#include "PBView.h"
#include "KMUtil.h"

IMPLEMENT_APP(KMPackageBuilderApp);

KMPackageBuilderApp::KMPackageBuilderApp(void)
{
    docManager_ = (wxDocManager *) NULL;
}

bool KMPackageBuilderApp::OnInit()
{
    SetAppName(wxT("KaraokeMachine Package Builder"));

    // create default config file
    wxConfigBase::Get(true);

    wxImage::AddHandler(new wxJPEGHandler);

    //// Create a document manager
    docManager_ = new wxDocManager;

    //// Template: Song
    (void) new wxDocTemplate((wxDocManager *) docManager_, wxT("Song Packages"), wxT("*.kms"), wxT(""), wxT("kms"),wxT("Songs Doc"), wxT("Songs View"),
          CLASSINFO(PBSongDocument), CLASSINFO(PBSongView));

    //// Template: Image
    (void) new wxDocTemplate(docManager_, wxT("Image Packages"), wxT("*.kmi"), wxT(""), wxT("kmi"), wxT("Images Doc"), wxT("Images View"),
          CLASSINFO(PBImageDocument), CLASSINFO(PBImageView));

    KMPackageBuilderFrame* frame = new KMPackageBuilderFrame((wxDocManager *) docManager_, (wxFrame *) NULL,
        wxT("KaraokeMachine Package Builder"), wxPoint(0, 0), wxSize(500, 400),
                      wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show(true);

    SetTopWindow(frame);

    return true;
}

int KMPackageBuilderApp::OnExit(void)
{
    delete docManager_;
    return 0;
}

wxMDIChildFrame *KMPackageBuilderApp::CreateChildFrame(wxDocument *doc, wxView *view, bool issong)
{
  //// Make a child frame
  wxDocMDIChildFrame *subframe =
      new wxDocMDIChildFrame(doc, view, (wxMDIParentFrame*)wxGetApp().GetTopWindow(), wxID_ANY, _T("Child Frame"),
                             wxPoint(10, 10), wxSize(300, 300),
                             wxDEFAULT_FRAME_STYLE |
                             wxNO_FULL_REPAINT_ON_RESIZE);

    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(wxID_NEW, _T("&New...\tCtrl-N"));
    fileMenu->Append(wxID_OPEN, _T("&Open...\tCtrl-O"));
    fileMenu->Append(wxID_SAVE, _T("&Save...\tCtrl-S"));
    fileMenu->Append(wxID_SAVEAS, _T("&Save as..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _("E&xit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(ID_MENUABOUT, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));


    subframe->SetMenuBar(mbar);

    return subframe;
}

bool KMPackageBuilderApp::OnExceptionInMainLoop()
{
    // this shows how we may let some exception propagate uncaught
    try
    {
        throw;
    }
/*
    catch ( KMException& e )
    {
        wxMessageBox(e.what(),
                     wxT("HTML Butcher"), wxOK | wxICON_ERROR);
    }
*/
    catch ( std::runtime_error& e )
    {
        wxMessageBox(wxString(e.what(), wxConvUTF8),
                     wxT("HTML Butcher"), wxOK | wxICON_ERROR);
    }
    catch ( ... )
    {
        wxMessageBox(wxT("Unhandled exception caught, program will terminate."),
                     wxT("HTML Butcher"), wxOK | wxICON_ERROR);
    }
    return true;
}

void KMPackageBuilderApp::OnUnhandledException()
{
    // this shows how we may let some exception propagate uncaught
    try
    {
        throw;
    }
/*
    catch ( KMException& e )
    {
        wxMessageBox(e.what(),
                     wxT("HTML Butcher"), wxOK | wxICON_ERROR);
    }
*/
    catch ( std::runtime_error& e )
    {
        wxMessageBox(wxString(e.what(), wxConvUTF8),
                     wxT("HTML Butcher"), wxOK | wxICON_ERROR);
    }
    catch ( ... )
    {
        wxMessageBox(wxT("Unhandled exception caught, program will terminate."),
                     wxT("HTML Butcher"), wxOK | wxICON_ERROR);
    }

}
