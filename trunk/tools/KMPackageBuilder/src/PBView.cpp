#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/treectrl.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/progdlg.h>

#include "KMPackageBuilderApp.h"
#include "PBDoc.h"
#include "PBView.h"
#include "KMSongFactory.h"

#include "DialogSongPackageEdit.h"
#include "DialogSongEdit.h"

class IDTreeData : public wxTreeItemData
{
public:
    IDTreeData(unsigned int id) : id_(id) {}

    unsigned int GetId() { return id_; }
private:
    unsigned int id_;
};

IMPLEMENT_DYNAMIC_CLASS(PBSongView, wxView)

BEGIN_EVENT_TABLE(PBSongView, wxView)
END_EVENT_TABLE()

bool PBSongView::OnCreate(wxDocument *doc, long flags)
{
    frame = wxGetApp().CreateChildFrame(doc, this, true);
    frame->SetTitle(wxT("Song"));
    frame->Connect(ID_ADD, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PBSongView::OnAdd), NULL, this);
    frame->Connect(ID_EDIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PBSongView::OnEdit), NULL, this);
    frame->Connect(ID_REMOVE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PBSongView::OnRemove), NULL, this);
    frame->Connect(ID_PROPERTIES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PBSongView::OnProperties), NULL, this);

    wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxHORIZONTAL);
    topsizer->Add(boxsizer, 1, wxGROW|wxALL, 3);

    // SONG LIST
    wxTreeCtrl *songlistctrl = new wxTreeCtrl(frame, ID_SONGLIST, wxDefaultPosition, wxSize(300, 400));
    songlistctrl->AddRoot(wxT("Songs"));
    boxsizer->Add(songlistctrl, 1, wxGROW|wxALL, 3);

    // BUTTONS
    wxBoxSizer *buttonssizer = new wxBoxSizer(wxVERTICAL);
    boxsizer->Add(buttonssizer, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // add button
    wxButton* add = new wxButton ( frame, ID_ADD, wxT("&Add"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonssizer->Add(add, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // edit button
    wxButton* edit = new wxButton ( frame, ID_EDIT, wxT("&Edit"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonssizer->Add(edit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // remove button
    wxButton* remove = new wxButton ( frame, ID_REMOVE, wxT("&Remove"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonssizer->Add(remove, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // properties button
    wxButton* properties = new wxButton ( frame, ID_PROPERTIES, wxT("&Properties"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonssizer->Add(properties, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);


    frame->SetSizer(topsizer);
    topsizer->SetSizeHints(frame);

    Load((PBSongDocument*)doc);

    frame->Show(true);
    Activate(true);

    return true;
}

bool PBSongView::OnClose(bool deleteWindow)
{
    if (!GetDocument()->Close())
        return false;

    Activate(false);

    if (deleteWindow)
    {
        //delete frame;
        if (frame)
            frame->Destroy();
        return true;
    }
    return true;
}

void PBSongView::OnUpdate(wxView* sender, wxObject* hint)
{
    Load((PBSongDocument*)GetDocument());
}

void PBSongView::OnAdd(wxCommandEvent &event)
{
    wxFileDialog d(frame, wxT("Add songs"), wxEmptyString, wxEmptyString, wxT("Karaoke files (*.kar)|*.kar"),
        wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE );
    if (d.ShowModal()==wxID_OK)
    {
        wxArrayString fls;
        d.GetPaths(fls);
        wxFileName fn;

        wxProgressDialog progress(wxT("Loading files"), wxT("Loading files, please wait..."), fls.GetCount(), frame);
        for (size_t i=0; i<fls.GetCount(); i++)
        {
            fn=fls[i];
            progress.Update(i, fn.GetFullName());
            AddFile(fls[i]);
        }
        GetDocument()->Modify(true);
    }
    //event.Skip();
}

void PBSongView::OnEdit(wxCommandEvent &event)
{
    wxTreeCtrl *songlistctrl=(wxTreeCtrl*)frame->FindWindow(ID_SONGLIST);

    if (songlistctrl->GetSelection().IsOk())
    {
        unsigned short did=static_cast<IDTreeData*>(songlistctrl->GetItemData(songlistctrl->GetSelection()))->GetId();

        KMPB_SongEditDialog d(frame);
        d.Load(&((PBSongDocument*)GetDocument())->songpackage_.Get(did));
        if (d.ShowModal()==wxID_OK)
        {
            d.Save(&((PBSongDocument*)GetDocument())->songpackage_.Get(did));
            GetDocument()->Modify(true);
        }
    }
}

void PBSongView::OnRemove(wxCommandEvent &event)
{
    wxTreeCtrl *songlistctrl=(wxTreeCtrl*)frame->FindWindow(ID_SONGLIST);

    if (songlistctrl->GetSelection().IsOk())
    {
        unsigned short did=static_cast<IDTreeData*>(songlistctrl->GetItemData(songlistctrl->GetSelection()))->GetId();
        ((PBSongDocument*)GetDocument())->songpackage_.Remove(did);
        TreeRemove(did);

        GetDocument()->Modify(true);
    }

    //event.Skip();
}

void PBSongView::OnProperties(wxCommandEvent &event)
{
    KMPB_SongPackageEditDialog d(frame);
    d.Load(&((PBSongDocument*)GetDocument())->songpackage_);
    if (d.ShowModal()==wxID_OK)
    {
        d.Save(&((PBSongDocument*)GetDocument())->songpackage_);
        GetDocument()->Modify(true);
    }
}

void PBSongView::Load(PBSongDocument *doc)
{
    wxTreeCtrl *songlistctrl=(wxTreeCtrl*)frame->FindWindow(ID_SONGLIST);
    songlistctrl->DeleteAllItems();
    songlistctrl->AddRoot(wxT("Songs"));

    KMArrayInt ids;
    doc->songpackage_.Ids(ids);
    for (KMArrayInt::const_iterator i=ids.begin(); i!=ids.end(); i++)
    {
        TreeAdd(*i, wxString(doc->songpackage_.Get(*i).GetTitle().c_str(), wxConvUTF8));
    }
}

void PBSongView::AddFile(const wxString &filename)
{
    wxFileName fn(filename);
    unsigned short nid;
    KMSong *song;

    try
    {
        song=KMSongFactory::Load(std::string(filename.mb_str(wxConvUTF8)));
        if (!song)
            throw KMException("Error");
    } catch (KMException &e) {
        wxMessageBox(wxString::Format(wxT("Invalid karaoke file: %s"), filename.c_str()), wxT("Error"),
            wxOK|wxICON_ERROR, frame);
        return;
    }

    PBSongDocument *doc=(PBSongDocument*)GetDocument();
    nid=doc->songpackage_.Add(std::string(fn.GetName().mb_str(wxConvUTF8)),
        std::string(filename.mb_str(wxConvUTF8)));

    std::string lyrics;
    for (int i=0; i<song->Lyrics().GetCount(); i++)
    {
        if (i>0) lyrics+=" ";
        lyrics+=song->Lyrics().GetLine(i).GetLine();
        if (lyrics.length()>15) break;
    }

    doc->songpackage_.Get(nid).SetStartLyrics(lyrics);
    TreeAdd(nid, fn.GetName());

    delete song;
}

void PBSongView::TreeAdd(unsigned int id, const wxString &title)
{
    wxTreeCtrl *songlistctrl=(wxTreeCtrl*)frame->FindWindow(ID_SONGLIST);

    songlistctrl->AppendItem(songlistctrl->GetRootItem(), wxString::Format(wxT("%d - %s"), id, title.c_str()),
        -1, -1, new IDTreeData(id));

    songlistctrl->ExpandAll();
}

void PBSongView::TreeRemove(unsigned int id)
{
    wxTreeCtrl *songlistctrl=(wxTreeCtrl*)frame->FindWindow(ID_SONGLIST);
    wxTreeItemIdValue cookie;

    wxTreeItemId item=songlistctrl->GetFirstChild(songlistctrl->GetRootItem(), cookie);
    while (item.IsOk())
    {
        if (static_cast<IDTreeData*>(songlistctrl->GetItemData(item))->GetId()==id)
        {
            songlistctrl->Delete(item);
            break;
        }

        item=songlistctrl->GetNextChild(songlistctrl->GetRootItem(), cookie);
    }
}

IMPLEMENT_DYNAMIC_CLASS(PBImageView, wxView)

BEGIN_EVENT_TABLE(PBImageView, wxView)
END_EVENT_TABLE()

bool PBImageView::OnCreate(wxDocument *doc, long flags)
{
    frame = wxGetApp().CreateChildFrame(doc, this, true);

    frame->Show(true);
    Activate(true);

    return true;
}

bool PBImageView::OnClose(bool deleteWindow)
{
    if (!GetDocument()->Close())
        return false;

    Activate(false);

    if (deleteWindow)
    {
        //delete frame;
        frame->Destroy();
        return true;
    }
    return true;
}
