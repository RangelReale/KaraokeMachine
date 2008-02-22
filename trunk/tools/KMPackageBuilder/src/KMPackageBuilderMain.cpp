/***************************************************************
 * Name:      KMPackageBuilderMain.cpp
 * Purpose:   Code for Application Frame
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

#include "KMPackageBuilderMain.h"

IMPLEMENT_CLASS(KMPackageBuilderFrame, wxDocMDIParentFrame)

BEGIN_EVENT_TABLE(KMPackageBuilderFrame, wxDocMDIParentFrame)
    EVT_MENU(ID_MENUQUIT, KMPackageBuilderFrame::OnQuit)
    EVT_MENU(ID_MENUABOUT, KMPackageBuilderFrame::OnAbout)
END_EVENT_TABLE()

KMPackageBuilderFrame::KMPackageBuilderFrame(wxDocManager *manager, wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size,
    long type) : wxDocMDIParentFrame(manager, frame, wxID_ANY, title, pos, size, type, wxT("KMPackageBuilderFrame"))
{
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(wxID_NEW, _T("&New...\tCtrl-N"));
    fileMenu->Append(wxID_OPEN, _T("&Open...\tCtrl-X"));
    fileMenu->Append(wxID_SAVE, _T("&Save...\tCtrl-S"));
    fileMenu->Append(wxID_SAVEAS, _T("&Save as..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(ID_MENUABOUT, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);

    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);
}


KMPackageBuilderFrame::~KMPackageBuilderFrame()
{
}

void KMPackageBuilderFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void KMPackageBuilderFrame::OnAbout(wxCommandEvent &event)
{
}
