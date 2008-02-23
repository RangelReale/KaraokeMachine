#include <wx/statline.h>
#include <wx/valgen.h>
#include <wx/thread.h>

#include "DialogSongEdit.h"
#include "KMSongFactory.h"

/////////////////////////////////
// CLASS
//      KMPB_SongEditDialog_LyricEvent
/////////////////////////////////

DEFINE_EVENT_TYPE( wxEVT_KMPBLYRIC_ACTION )

KMPB_SongEditDialog_LyricEvent::KMPB_SongEditDialog_LyricEvent(const std::string &lyric,
    int id, wxEventType commandType) :
    wxEvent(id, commandType), lyric_(lyric)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}

wxEvent* KMPB_SongEditDialog_LyricEvent::Clone() const
{
    return new KMPB_SongEditDialog_LyricEvent(*this);
}


/////////////////////////////////
// CLASS
//      KMPB_SongEditDialog_Player
/////////////////////////////////

void KMPB_SongEditDialog_Player::Play()
{
    if (song_)
    {
        lastlyric_=-1;
        song_->Play();
        isplaying_=true;
    }
}

void KMPB_SongEditDialog_Player::Stop()
{
    if (song_)
    {
        lastlyric_=-1;
        song_->Stop();
        isplaying_=false;
    }
}

wxThread::ExitCode KMPB_SongEditDialog_Player::Entry()
{
    while (!TestDestroy())
    {
        wxCriticalSectionLocker lock(cs_);
        if (song_ && isplaying_)
        {
            song_->Poll();
            if (parent_ && song_->GetLyricsCurrentLine()>-1 && song_->GetLyricsCurrentLine()!=lastlyric_)
            {
                lastlyric_=song_->GetLyricsCurrentLine();
                KMPB_SongEditDialog_LyricEvent event(song_->Lyrics().GetLine(song_->GetLyricsCurrentLine()).GetLine());
                wxPostEvent(parent_, event);
            }
        }
    }
    return 0;
}

void KMPB_SongEditDialog_Player::SetMelodyTrack(char v)
{
    wxCriticalSectionLocker lock(cs_);
    if (song_)
        song_->SetMelodyTrack(v);
}

void KMPB_SongEditDialog_Player::SetTranspose(char v)
{
    wxCriticalSectionLocker lock(cs_);
    if (song_)
        song_->SetTranspose(v);
}

BEGIN_EVENT_TABLE(KMPB_SongEditDialog, wxDialog)
    EVT_BUTTON(ID_PLAY, KMPB_SongEditDialog::OnPlay)
    EVT_BUTTON(ID_STOP, KMPB_SongEditDialog::OnStop)
    EVT_SPINCTRL(ID_MELODYTRACK, KMPB_SongEditDialog::OnChangeMelodyTrack)
    EVT_SPINCTRL(ID_TRANSPOSE, KMPB_SongEditDialog::OnChangeTranspose)
    EVT_KMPBLYRIC(wxID_ANY, KMPB_SongEditDialog::OnLyric)
END_EVENT_TABLE()


KMPB_SongEditDialog::KMPB_SongEditDialog( wxWindow* parent,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    wxDialog(parent, id, caption, pos, size, style), song_(NULL), player_(NULL)
{
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    Init();
    CreateControls();
}

KMPB_SongEditDialog::~KMPB_SongEditDialog()
{
    if (player_)
        player_->Delete();

    if (song_)
        delete song_;
}


void KMPB_SongEditDialog::Init()
{
    title_=wxEmptyString;
    artist_=wxEmptyString;
    artist2_=wxEmptyString;
    genre_=wxEmptyString;
    subgenre_=wxEmptyString;
    startlyrics_=wxEmptyString;
    melodytrack_=0;
    transpose_=0;
    tags_=wxEmptyString;
}

void KMPB_SongEditDialog::Load(KMSongPackageItem *song)
{
    title_=wxString(song->GetTitle().c_str(), wxConvUTF8);
    artist_=wxString(song->GetArtist().c_str(), wxConvUTF8);
    artist2_=wxString(song->GetArtist2().c_str(), wxConvUTF8);
    genre_=wxString(song->GetGenre().c_str(), wxConvUTF8);
    subgenre_=wxString(song->GetSubGenre().c_str(), wxConvUTF8);
    startlyrics_=wxString(song->GetStartLyrics().c_str(), wxConvUTF8);
    melodytrack_=song->GetMelodyTrack()+1;
    transpose_=song->GetTranspose();
    tags_=wxString(song->Tags().GetTags().c_str(), wxConvUTF8);

    song_=new std::stringstream;
    song->GetFileData(*song_);
}

void KMPB_SongEditDialog::Save(KMSongPackageItem *song)
{
    song->SetTitle(std::string(title_.mb_str(wxConvUTF8)));
    song->SetArtist(std::string(artist_.mb_str(wxConvUTF8)));
    song->SetArtist2(std::string(artist2_.mb_str(wxConvUTF8)));
    song->SetGenre(std::string(genre_.mb_str(wxConvUTF8)));
    song->SetSubGenre(std::string(subgenre_.mb_str(wxConvUTF8)));
    song->SetMelodyTrack(melodytrack_-1);
    song->SetTranspose(transpose_);
    song->Tags().SetTags(std::string(tags_.mb_str(wxConvUTF8)));
}

void KMPB_SongEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxEXPAND|wxALL, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);

    // FIELDS
    wxBoxSizer *fieldssizer = new wxBoxSizer(wxVERTICAL);
/*
    wxFlexGridSizer *fieldssizer = new wxFlexGridSizer(2);
    fieldssizer->AddGrowableCol(1, 1);
    fieldssizer->AddGrowableRow(8, 1);
    fieldssizer->SetFlexibleDirection(wxHORIZONTAL);
*/
    bodysizer->Add(fieldssizer, 1, wxEXPAND|wxALL, 3);

    // Title
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Title:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_TITLE, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0),
        0, wxGROW|wxALL, 3);

    // Artist
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Artist:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_ARTIST, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0),
        0, wxGROW|wxALL, 3);

    // Artist 2
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("A&rtist 2:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_ARTIST2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0),
        0, wxGROW|wxALL, 3);

    // Genre
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Genre:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_GENRE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0),
        0, wxGROW|wxALL, 3);

    // Subgenre
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("Su&b-genre:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_SUBGENRE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0),
        0, wxGROW|wxALL, 3);

    // Start Lyrics
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("Start &Lyrics:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_STARTLYRICS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0),
        0, wxGROW|wxALL, 3);

    // Melody Track
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Melody Track: (1-16, 0 is none)"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxSpinCtrl(this, ID_MELODYTRACK, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 16, 0),
        0, wxGROW|wxALL, 3);

    // Transpose
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Transpose:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxSpinCtrl(this, ID_TRANSPOSE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -20, 20, 0),
        0, wxGROW|wxALL, 3);

    // Tags
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, wxT("&Tags (comma-separated):"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);
    fieldssizer->Add(new wxTextCtrl(this, ID_TAGS, wxEmptyString, wxDefaultPosition, wxSize(-1, 40), wxTE_MULTILINE),
        1, wxGROW|wxALL, 3);


    // Lyric
    wxStaticText *lyricctrl=new wxStaticText(this, ID_LYRIC, wxT(""), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
    boxsizer->Add(lyricctrl, 0, wxEXPAND|wxALL, 3);


    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // play button
    buttonsizer->Add(new wxButton ( this, ID_PLAY, wxT("&Play"), wxDefaultPosition, wxDefaultSize, 0 ),
        0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // stop button
    buttonsizer->Add(new wxButton ( this, ID_STOP, wxT("&Stop"), wxDefaultPosition, wxDefaultSize, 0 ),
        0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // ok button
    buttonsizer->Add(new wxButton ( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 ),
        0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // cancel button
    buttonsizer->Add(new wxButton ( this, wxID_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 ),
        0, wxALIGN_CENTER_VERTICAL|wxALL, 3);


    // validators
    FindWindow(ID_TITLE)->SetValidator(wxTextValidator(wxFILTER_NONE, &title_));
    FindWindow(ID_ARTIST)->SetValidator(wxTextValidator(wxFILTER_NONE, &artist_));
    FindWindow(ID_ARTIST2)->SetValidator(wxTextValidator(wxFILTER_NONE, &artist2_));
    FindWindow(ID_GENRE)->SetValidator(wxTextValidator(wxFILTER_NONE, &genre_));
    FindWindow(ID_SUBGENRE)->SetValidator(wxTextValidator(wxFILTER_NONE, &subgenre_));
    FindWindow(ID_STARTLYRICS)->SetValidator(wxTextValidator(wxFILTER_NONE, &startlyrics_));
    FindWindow(ID_MELODYTRACK)->SetValidator(wxGenericValidator(&melodytrack_));
    FindWindow(ID_TRANSPOSE)->SetValidator(wxGenericValidator(&transpose_));
    FindWindow(ID_TAGS)->SetValidator(wxTextValidator(wxFILTER_NONE, &tags_));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}

bool KMPB_SongEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    return true;
}

bool KMPB_SongEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    return true;
}

void KMPB_SongEditDialog::OnPlay(wxCommandEvent &event)
{
    if (!player_)
    {
        wxSpinCtrl *melodytrackctrl=(wxSpinCtrl*)FindWindow(ID_MELODYTRACK);
        wxSpinCtrl *transposectrl=(wxSpinCtrl*)FindWindow(ID_TRANSPOSE);

        song_->seekg(0);
        KMSong *s=KMSongFactory::Load(*song_);
        s->SetMelodyTrack(melodytrackctrl->GetValue()-1);
        s->SetTranspose(transposectrl->GetValue());

        player_=new KMPB_SongEditDialog_Player(this, s);
        player_->Create();
        player_->Play();
        player_->Run();
    }
}

void KMPB_SongEditDialog::OnStop(wxCommandEvent &event)
{
    if (player_)
    {
        player_->Delete();
        player_=NULL;

        wxStaticText *lyricctrl=(wxStaticText*)FindWindow(ID_LYRIC);
        lyricctrl->SetLabel(wxT(""));

    }
}

void KMPB_SongEditDialog::OnChangeMelodyTrack(wxSpinEvent &event)
{
    if (player_)
        player_->SetMelodyTrack(event.GetPosition()-1);
}

void KMPB_SongEditDialog::OnChangeTranspose(wxSpinEvent &event)
{
    if (player_)
        player_->SetTranspose(event.GetPosition());
}

void KMPB_SongEditDialog::OnLyric(KMPB_SongEditDialog_LyricEvent &event)
{
    wxStaticText *lyricctrl=(wxStaticText*)FindWindow(ID_LYRIC);
    lyricctrl->SetLabel(wxString(event.GetLyric().c_str(), wxConvUTF8));
}
