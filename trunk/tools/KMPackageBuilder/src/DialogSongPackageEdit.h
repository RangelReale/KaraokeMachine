#ifndef _H_DIALOGSONGPACKAGEEDIT_H_
#define _H_DIALOGSONGPACKAGEEDIT_H_

#include <sstream>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "KMSong.h"

using namespace KaraokeMachine;

class KMPB_SongPackageEditDialog : public wxDialog
{
    DECLARE_EVENT_TABLE()
public:
    KMPB_SongPackageEditDialog( wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = wxT("Song Package"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );
    ~KMPB_SongPackageEditDialog();

    void Init();
    void CreateControls();

    void Load(KMSongPackage *package);
    void Save(KMSongPackage *package);

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    enum {
        ID_FIRST = wxID_HIGHEST,
        ID_DEFAULTID,
        ID_TITLE,
        ID_AUTHOR,
        ID_DESCRIPTION,
    };
private:
    unsigned int defaultid_;
    wxString title_, author_, description_;
};

#endif //_H_DIALOGSONGPACKAGEEDIT_H_
