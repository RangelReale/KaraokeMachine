#ifndef _H_DIALOGIMAGEPACKAGEEDIT_H_
#define _H_DIALOGIMAGEPACKAGEEDIT_H_

#include <sstream>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "KMImage.h"

using namespace KaraokeMachine;

class KMPB_ImagePackageEditDialog : public wxDialog
{
    DECLARE_EVENT_TABLE()
public:
    KMPB_ImagePackageEditDialog( wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = wxT("Image Package"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );
    ~KMPB_ImagePackageEditDialog();

    void Init();
    void CreateControls();

    void Load(KMImagePackage *package);
    void Save(KMImagePackage *package);

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    enum {
        ID_FIRST = wxID_HIGHEST,
        ID_TITLE,
        ID_AUTHOR,
        ID_DESCRIPTION,
    };
private:
    wxString title_, author_, description_;
};

#endif //_H_DIALOGIMAGEPACKAGEEDIT_H_
