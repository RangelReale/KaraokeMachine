#ifndef H_KMSONG_H
#define H_KMSONG_H

#include "KMDefs.h"
#include "KMUtil.h"

#include <string>
#include <deque>
#include <map>
#include <ostream>

namespace KaraokeMachine {

// forward declarations
class KMSongPackage;

/**
 * @class KMLyricsLine
 *
 * @brief Lyrics line
 */
class KMLyricsLine {
public:
    KMLyricsLine(const std::string &line) : line_(line) {}

    const std::string GetLine() const { return line_; }
private:
    std::string line_;
};

/**
 * @class KMLyrics
 *
 * @brief Lyrics list
 */
class KMLyrics {
public:
    KMLyrics() : lines_() {}

    const KMLyricsLine &GetLine(int i) const { return lines_[i]; }
    int GetCount() const { return lines_.size(); }
private:
    std::deque<KMLyricsLine> lines_;

    friend class KMSong;
};

/**
 * @class KMSong
 *
 * @brief Song
 */
class KMSong {
public:
    KMSong(KMInputStream &stream) : title_(""), lyrics_(),
        lyricscurrentline_(-1), lyricscurrentpospct_(-1), debug_("") { }
    KMSong(const std::string &filename) : title_(""), lyrics_(),
        lyricscurrentline_(-1), lyricscurrentpospct_(-1), debug_("") { }
    virtual ~KMSong() {}

    virtual bool Play() = 0;
    virtual bool Stop() = 0;
    virtual bool Pause() = 0;

    virtual bool Poll() = 0;

    virtual int GetTranspose() { return 0; }
    virtual void SetTranspose(int t) {}
    virtual void SetMelodyTrack(int t) {}

    const std::string &GetTitle() { return title_; }

    KMLyrics &Lyrics() { return lyrics_; }
    int GetLyricsCurrentLine() { return lyricscurrentline_; }
    int GetLyricsCurrentPosPct() { return lyricscurrentpospct_; }
    const std::string &GetDebug() { return debug_; }
protected:
    void SetTitle(const std::string &v) { title_=v; }

    void AddLyricsLine(const std::string &line);
    void SetLyricsCurrentLine(int l) { lyricscurrentline_=l; }
    void SetLyricsCurrentPosPctLine(int l) { lyricscurrentpospct_=l; }

    void SetDebug(const std::string &debug) { debug_=debug; }
private:
    std::string title_;
    KMLyrics lyrics_;
    int lyricscurrentline_, lyricscurrentpospct_;
    std::string debug_;
};

/**
 * @class KMSongPackageItem
 *
 * @brief Song package song
 */
class KMSongPackageItem {
public:
    KMSongPackageItem(KMSongPackage *package, unsigned short id, std::string title = "",
        std::string filename = "") : package_(package), id_(id),
        title_(title), artist_(""), artist2_(""), genre_(""), subgenre_(""),
        startlyrics_(""), melodytrack_(-1), transpose_(0), filename_(filename),
        filepos_(0), filesize_(0), tags_() {}

    KMSongPackage *Package() const { return package_; }
    unsigned short GetId() const { return id_; }
    const std::string &GetTitle() const { return title_; }
    const std::string &GetArtist() const { return artist_; }
    const std::string &GetArtist2() const { return artist2_; }
    const std::string &GetGenre() const { return genre_; }
    const std::string &GetSubGenre() const { return subgenre_; }
    const std::string &GetFilename() const { return filename_; }
    const std::string &GetStartLyrics() const { return startlyrics_; }
    char GetMelodyTrack() const { return melodytrack_; }
    char GetTranspose() const { return transpose_; }

    void SetTitle(const std::string &v) { title_=v; }
    void SetArtist(const std::string &v) { artist_=v; }
    void SetArtist2(const std::string &v) { artist2_=v; }
    void SetGenre(const std::string &v) { genre_=v; }
    void SetSubGenre(const std::string &v) { subgenre_=v; }
    void SetFilename(const std::string &v) { filename_=v; }
    void SetStartLyrics(const std::string &v) { startlyrics_=v; }
    void SetMelodyTrack(char v) { melodytrack_=v; }
    void SetTranspose(char v) { transpose_=v; }

    KMTags &Tags() { return tags_; }

    void GetFileData(std::ostream &stream) const;
private:
    KMSongPackage *package_;
    unsigned short id_;
    std::string title_, artist_, artist2_, genre_, subgenre_, startlyrics_;
    char melodytrack_, transpose_;
    std::string filename_;
    unsigned int filepos_, filesize_;
    KMTags tags_;

    friend class KMSongPackage;
};

/**
 * @class KMSongPackage
 *
 * @brief Song package
 */
class KMSongPackage {
public:
    KMSongPackage(unsigned int id) : id_(id), defaultid_(0), maxid_(0),
        title_(""), author_(""), description_(""),
        songs_(), file_(NULL) {}
    ~KMSongPackage();

    unsigned short Add(const std::string &songname,
        const std::string &filename, unsigned short id = 0);
    bool Exists(unsigned short id);
    KMSongPackageItem &Get(unsigned short id);
    void Remove(unsigned short id);
    void ChangeId(unsigned short oldid, unsigned short newid);
    void Ids(KMArrayInt &ids);

    unsigned int GetId() { return id_; }
    unsigned int GetDefaultId() { return defaultid_; }
    const std::string &GetTitle() { return title_; }
    const std::string &GetAuthor() { return author_; }
    const std::string &GetDescription() { return description_; }

    void SetId(unsigned int v) { id_=v; }
    void SetDefaultId(unsigned int v) { defaultid_=v; }
    void SetTitle(const std::string &v) { title_=v; }
    void SetAuthor(const std::string &v) { author_=v; }
    void SetDescription(const std::string &v) { description_=v; }

    void Load(const std::string &filename);
    void Save(const std::string &filename);

    KMInputStream *GetFile() { return file_; }
private:
    typedef std::map<unsigned short, KMSongPackageItem> songs_t;

    unsigned short Add(unsigned short id);

    void Load(KMInputStream &stream);
    void Save(KMOutputStream &stream);

    unsigned int id_, defaultid_;
    unsigned short maxid_;
    std::string title_, author_, description_;
    songs_t songs_;
    KMStream *file_;
};

};

#endif //H_KMSONG_H
