#ifndef _H_DIALOGSONGEDIT_H_
#define _H_DIALOGSONGEDIT_H_

#include <sstream>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "KMSong.h"

using namespace KaraokeMachine;


/**
 * @class KMPB_SongEditDialog_LyricEvent
 *
 * @brief Lyric event
 */
DECLARE_EVENT_TYPE( wxEVT_KMPBLYRIC_ACTION, -1 )

class KMPB_SongEditDialog_LyricEvent : public wxEvent
{
public:
    KMPB_SongEditDialog_LyricEvent(const std::string &lyric,
        int id = 0, wxEventType commandType = wxEVT_KMPBLYRIC_ACTION);

    // required for sending with wxPostEvent()
    virtual wxEvent* Clone() const;

    const std::string &GetLyric() { return lyric_; }
private:
    std::string lyric_;
};

typedef void (wxEvtHandler::*KMPB_SongEditDialog_LyricEventFunction)(KMPB_SongEditDialog_LyricEvent&);

#define KMPB_SongEditDialog_LyricEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(KMPB_SongEditDialog_LyricEventFunction, &func)

#define EVT_KMPBLYRIC(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_KMPBLYRIC_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( KMPB_SongEditDialog_LyricEventFunction, & fn ), (wxObject *) NULL ),


class KMPB_SongEditDialog_Player : public wxThread
{
public:
    KMPB_SongEditDialog_Player(wxEvtHandler *parent, KMSong *song) :
        wxThread(), parent_(parent), song_(song), isplaying_(false), cs_(), lastlyric_(-1) {}
    ~KMPB_SongEditDialog_Player() { if (song_) { song_->Stop(); delete song_; } }

    void Play();
    void Stop();

    void SetMelodyTrack(char v);
    void SetTranspose(char v);
protected:
    virtual wxThread::ExitCode Entry();
private:
    wxEvtHandler *parent_;
    KMSong *song_;
    bool isplaying_;
    wxCriticalSection cs_;
    int lastlyric_;
};

class KMPB_SongEditDialog : public wxDialog
{
    DECLARE_EVENT_TABLE()
public:
    KMPB_SongEditDialog( wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = wxT("Song"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );
    ~KMPB_SongEditDialog();

    void Init();
    void CreateControls();

    void Load(KMSongPackageItem *song);
    void Save(KMSongPackageItem *song);

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    void OnPlay(wxCommandEvent &event);
    void OnStop(wxCommandEvent &event);
    void OnChangeMelodyTrack(wxSpinEvent &event);
    void OnChangeTranspose(wxSpinEvent &event);
    void OnLyric(KMPB_SongEditDialog_LyricEvent &event);

    enum {
        ID_FIRST = wxID_HIGHEST,
        ID_TITLE,
        ID_ARTIST,
        ID_ARTIST2,
        ID_GENRE,
        ID_SUBGENRE,
        ID_STARTLYRICS,
        ID_MELODYTRACK,
        ID_TRANSPOSE,
        ID_TAGS,

        ID_LYRIC,

        ID_PLAY,
        ID_STOP
    };
private:
    std::stringstream *song_;
    KMPB_SongEditDialog_Player *player_;

    wxString title_, artist_, artist2_, genre_, subgenre_, startlyrics_, tags_;
    int melodytrack_, transpose_;
};

#endif //_H_DIALOGSONGEDIT_H_
