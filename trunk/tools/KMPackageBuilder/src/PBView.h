#ifndef _H_PBVIEW_H_
#define _H_PBVIEW_H_

#include <wx/docview.h>
#include "PBDoc.h"

using namespace KaraokeMachine;

class PBSongView: public wxView
{
public:
    PBSongView() { frame = (wxMDIChildFrame *) NULL; }
    ~PBSongView() {}

    virtual bool OnCreate(wxDocument *doc, long flags);
    virtual void OnDraw(wxDC *dc) {}
    virtual bool OnClose(bool deleteWindow = true);

    virtual void OnUpdate(wxView* sender, wxObject* hint);
protected:
    void OnAdd(wxCommandEvent &event);
    void OnEdit(wxCommandEvent &event);
    void OnRemove(wxCommandEvent &event);
    void OnProperties(wxCommandEvent &event);

    enum
    {
        ID_SONGLIST = wxID_HIGHEST,
        ID_ADD,
        ID_EDIT,
        ID_REMOVE,
        ID_PROPERTIES,

    };
private:
    void Load(PBSongDocument *doc);

    void AddFile(const wxString &filename);

    void TreeAdd(unsigned int id, const wxString &title);
    void TreeRemove(unsigned int id);

    wxMDIChildFrame *frame;

    DECLARE_DYNAMIC_CLASS(PBSongView)
    DECLARE_EVENT_TABLE()
};

class PBImageView: public wxView
{
public:
    PBImageView(): wxView() { frame = (wxMDIChildFrame *) NULL; }
    ~PBImageView() {}

    virtual bool OnCreate(wxDocument *doc, long flags);
    virtual void OnDraw(wxDC *dc) {}
    virtual bool OnClose(bool deleteWindow = true);
private:
    wxMDIChildFrame *frame;

    DECLARE_DYNAMIC_CLASS(PBImageView)
    DECLARE_EVENT_TABLE()
};


#endif //_H_PBVIEW_H_
