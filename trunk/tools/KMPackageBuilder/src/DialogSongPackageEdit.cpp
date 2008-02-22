#include <wx/statline.h>
#include <wx/valgen.h>
#include <wx/thread.h>

#include "DialogSongPackageEdit.h"


BEGIN_EVENT_TABLE(KMPB_SongPackageEditDialog, wxDialog)
END_EVENT_TABLE()

KMPB_SongPackageEditDialog::KMPB_SongPackageEditDialog( wxWindow* parent,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    wxDialog(parent, id, caption, pos, size, style)
{
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    Init();
    CreateControls();
}

KMPB_SongPackageEditDialog::~KMPB_SongPackageEditDialog()
{

}


void KMPB_SongPackageEditDialog::Init()
{
    defaultid_=0;
    title_=wxEmptyString;
    author_=wxEmptyString;
    description_=wxEmptyString;
}

void KMPB_SongPackageEditDialog::Load(KMSongPackage *package)
{
    defaultid_=package->GetDefaultId();
    title_=wxString(package->GetTitle().c_str(), wxConvUTF8);
    author_=wxString(package->GetAuthor().c_str(), wxConvUTF8);
    description_=wxString(package->GetDescription().c_str(), wxConvUTF8);
}

void KMPB_SongPackageEditDialog::Save(KMSongPackage *package)
{
    package->SetDefaultId(defaultid_);
    package->SetTitle(std::string(title_.mb_str(wxConvUTF8)));
    package->SetAuthor(std::string(author_.mb_str(wxConvUTF8)));
    package->SetDescription(std::string(description_.mb_str(wxConvUTF8)));
}

void KMPB_SongPackageEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxEXPAND|wxALL, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);

    // FIELDS
    wxBoxSizer *fieldssizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(fieldssizer, 1, wxEXPAND|wxALL, 3);

    // Default ID
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("Default &Id:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxSpinCtrl(this, ID_DEFAULTID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 999999999, 0),
        0, wxGROW|wxALL, 3);


    // Title
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Title:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_TITLE, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0),
        0, wxGROW|wxALL, 3);

    // Author
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Author:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_AUTHOR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0),
        0, wxGROW|wxALL, 3);

    // Description
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Description:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxSize(200, 200), wxTE_MULTILINE|wxTE_PROCESS_ENTER),
        1, wxGROW|wxALL, 3);


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
    FindWindow(ID_DEFAULTID)->SetValidator(wxGenericValidator((int*)&defaultid_));
    FindWindow(ID_TITLE)->SetValidator(wxTextValidator(wxFILTER_NONE, &title_));
    FindWindow(ID_AUTHOR)->SetValidator(wxTextValidator(wxFILTER_NONE, &author_));
    FindWindow(ID_DESCRIPTION)->SetValidator(wxTextValidator(wxFILTER_NONE, &description_));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}

bool KMPB_SongPackageEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    return true;
}

bool KMPB_SongPackageEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    return true;
}
