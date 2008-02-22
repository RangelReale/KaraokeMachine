

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "PBDoc.h"
#include "PBView.h"

IMPLEMENT_DYNAMIC_CLASS(PBSongDocument, wxDocument)

PBSongDocument::~PBSongDocument(void)
{

}

bool PBSongDocument::OnNewDocument()
{
    SetTitle(wxT("New song"));
    return true;
}

bool PBSongDocument::OnSaveDocument(const wxString& filename)
{
    songpackage_.Save(std::string(filename.mb_str(wxConvUTF8)));
    Modify(false);
    return true;
}

bool PBSongDocument::OnOpenDocument(const wxString& filename)
{
    songpackage_.Load(std::string(filename.mb_str(wxConvUTF8)));
    UpdateAllViews();
    Modify(false);
    return true;
}

/*
bool PBSongDocument::IsModified(void) const
{
    return false;
}

void PBSongDocument::Modify(bool mod)
{

}
*/

IMPLEMENT_DYNAMIC_CLASS(PBImageDocument, wxDocument)

PBImageDocument::~PBImageDocument(void)
{

}

bool PBImageDocument::OnNewDocument()
{
    return true;
}

bool PBImageDocument::OnSaveDocument(const wxString& filename)
{
    return false;
}

bool PBImageDocument::OnOpenDocument(const wxString& filename)
{
    return false;
}

bool PBImageDocument::IsModified(void) const
{
    return false;
}

void PBImageDocument::Modify(bool mod)
{

}
