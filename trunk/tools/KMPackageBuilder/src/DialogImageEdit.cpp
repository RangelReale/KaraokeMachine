#include <wx/statline.h>
#include <wx/valgen.h>
#include <wx/thread.h>

#include "DialogImageEdit.h"

BEGIN_EVENT_TABLE(KMPB_ImageEditDialog, wxDialog)
END_EVENT_TABLE()


KMPB_ImageEditDialog::KMPB_ImageEditDialog( wxWindow* parent,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    wxDialog(parent, id, caption, pos, size, style), image_(NULL)
{
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    Init();
    CreateControls();
}

KMPB_ImageEditDialog::~KMPB_ImageEditDialog()
{
    if (image_)
        delete image_;
}


void KMPB_ImageEditDialog::Init()
{
    title_=wxEmptyString;
    iswide_=false;
}

void KMPB_ImageEditDialog::Load(KMImagePackageItem *image)
{
    title_=wxString(image->GetTitle().c_str(), wxConvUTF8);
    iswide_=image->GetIsWide();

    image_=new std::stringstream;
    image->GetFileData(*image_);
}

void KMPB_ImageEditDialog::Save(KMImagePackageItem *image)
{
    image->SetTitle(std::string(title_.mb_str(wxConvUTF8)));
    image->SetIsWide(iswide_);
}

void KMPB_ImageEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxEXPAND|wxALL, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);

    // FIELDS
    //wxBoxSizer *fieldssizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer *fieldssizer = new wxFlexGridSizer(2);
    fieldssizer->AddGrowableCol(1, 1);
    fieldssizer->SetFlexibleDirection(wxHORIZONTAL);
    bodysizer->Add(fieldssizer, 1, wxEXPAND|wxALL, 3);

    // Title
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Title:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_TITLE, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0),
        0, wxGROW|wxALL, 3);

    // IsWide
    fieldssizer->Add(new wxCheckBox(this, ID_ISWIDE, wxT("Is &Wide"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxGROW|wxALL, 3);


    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // ok button
    buttonsizer->Add(new wxButton ( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 ),
        0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // cancel button
    buttonsizer->Add(new wxButton ( this, wxID_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 ),
        0, wxALIGN_CENTER_VERTICAL|wxALL, 3);


    // validators
    FindWindow(ID_TITLE)->SetValidator(wxTextValidator(wxFILTER_NONE, &title_));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}

bool KMPB_ImageEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxCheckBox *iswidectrl=(wxCheckBox*)FindWindow(ID_ISWIDE);

    iswidectrl->SetValue(iswide_);

    return true;
}

bool KMPB_ImageEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    wxCheckBox *iswidectrl=(wxCheckBox*)FindWindow(ID_ISWIDE);

    iswide_=iswidectrl->GetValue();

    return true;
}
