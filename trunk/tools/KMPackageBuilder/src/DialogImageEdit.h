#ifndef _H_DIALOGIMAGEEDIT_H_
#define _H_DIALOGIMAGEEDIT_H_

#include <sstream>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "KMImage.h"

using namespace KaraokeMachine;

class KMPB_ImageEditDialog : public wxDialog
{
    DECLARE_EVENT_TABLE()
public:
    KMPB_ImageEditDialog( wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = wxT("Image"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );
    ~KMPB_ImageEditDialog();

    void Init();
    void CreateControls();

    void Load(KMImagePackageItem *image);
    void Save(KMImagePackageItem *image);

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    enum {
        ID_FIRST = wxID_HIGHEST,
        ID_TITLE,
        ID_ISWIDE
    };
private:
    std::stringstream *image_;

    wxString title_;
    bool iswide_;
};

#endif //_H_DIALOGIMAGEEDIT_H_
