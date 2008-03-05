#ifndef H_KMACHINE_H
#define H_KMACHINE_H

#include <map>
#include "linked_ptr.h"
#include "KMSong.h"
#include "KMImage.h"
#include "KMOptions.h"

namespace KaraokeMachine {

// forward deckarations
class KMachine;
class KMachinePlaylist;
class KMachineImageList;

/**
 * @class KMachineSongs
 *
 * @brief Machine song database, organized by package
 */
class KMachineSongs {
public:
    KMachineSongs(KMachine *machine) : machine_(machine), maxid_(0), packages_() {}

    unsigned int Add(unsigned int id = 0);
    KMSongPackage &Get(unsigned int id);
    bool Exists(unsigned int id);
    void Ids(KMArrayInt &ids);

    unsigned int Load(const std::string &filename);
    int LoadPath(const std::string &path);
private:
    typedef std::map<unsigned int, linked_ptr<KMSongPackage> > packages_t;

    KMachine *machine_;
    unsigned int maxid_;
    packages_t packages_;
};

/**
 * @class KMachineImages
 *
 * @brief Machine image database, organized by package
 */
class KMachineImages {
public:
    KMachineImages(KMachine *machine) : machine_(machine), maxid_(0), packages_() {}

    unsigned int Add();
    KMImagePackage &Get(unsigned int id);
    bool Exists(unsigned int id);
    void Ids(KMArrayInt &ids);

    unsigned int Load(const std::string &filename);
    int LoadPath(const std::string &path);
private:
    typedef std::map<unsigned int, linked_ptr<KMImagePackage> > packages_t;

    KMachine *machine_;
    unsigned int maxid_;
    packages_t packages_;
};

/**
 * @class KMachinePlaylistSong
 *
 * @brief A song on the machine's playlist
 */
class KMachinePlaylistSong {
public:
    KMachinePlaylistSong(KMachinePlaylist *playlist, unsigned int package, unsigned short song) :
        playlist_(playlist), package_(package), song_(song) {}

    unsigned int GetPackageId() { return package_; }
    unsigned short GetSongId() { return song_; }

    KMSongPackageItem *GetSong();
private:
    KMachinePlaylist *playlist_;
    unsigned int package_;
    unsigned short song_;
};


/**
 * @class KMachinePlaylist
 *
 * @brief Machine's playlist
 */
class KMachinePlaylist {
public:
    KMachinePlaylist(KMachine *machine) : machine_(machine), songs_() {}

    int Add(unsigned int package, unsigned short song);
    void Remove(int index);

    int GetCount() { return songs_.size(); }

    KMachinePlaylistSong &Get(int index) { return songs_[index]; }

    KMachinePlaylistSong Pop() { KMachinePlaylistSong ret=songs_.front(); songs_.pop_front(); return ret; }

    KMachine *Machine() { return machine_; }
private:
    typedef std::deque<KMachinePlaylistSong> songs_t;

    KMachine *machine_;
    songs_t songs_;
};

/**
 * @class KMachineImageListImage
 *
 * @brief Image list image
 */
class KMachineImageListImage {
public:
    KMachineImageListImage(KMachineImageList *imagelist, unsigned int package, unsigned short image) :
        imagelist_(imagelist), package_(package), image_(image) {}

    unsigned int GetPackageId() { return package_; }
    unsigned short GetImageId() { return image_; }

    KMImagePackageItem *GetImage();
private:
    KMachineImageList *imagelist_;
    unsigned int package_;
    unsigned short image_;
};


/**
 * @class KMachineImageList
 *
 * @brief Image list
 */
class KMachineImageList {
public:
    KMachineImageList(KMachine *machine) : machine_(machine), images_() {}

    int Add(unsigned int package, unsigned short image);
    void Remove(int index);

    int GetCount() { return images_.size(); }

    KMachineImageListImage &Get(int index) { return images_[index]; }
    KMachineImageListImage &GetRandom();

    KMachine *Machine() { return machine_; }
private:
    typedef std::deque<KMachineImageListImage> images_t;

    KMachine *machine_;
    images_t images_;
};

/**
 * @class KMachine
 *
 * @brief Karaoke machine controller
 */
class KMachine {
public:
    enum commant_t { KC_ADD, KC_STOP, KC_PAUSE, KC_SKIP, KC_SKIPIMAGE, KC_SONGKEYDOWN, KC_SONGKEYUP };

    KMachine() : chars_(""), songs_(this), images_(this), playlist_(this),
        imagelist_(this), playing_(NULL), options_() {}
    virtual ~KMachine();

    virtual void Initialize();
    virtual void Finalize();
    void Run();

    virtual void AddChar(unsigned char c);
    virtual void AddString(const std::string s);
    virtual void RemoveChar();
    virtual void DoCommand(commant_t command, int param = 0);

    const std::string &GetChars() const { return chars_; }

    KMachinePlaylist &Playlist() { return playlist_; }
    KMachineImageList &ImageList() { return imagelist_; }

    KMachineSongs &Songs() { return songs_; }
    KMachineImages &Images() { return images_; }


    KMSong *Playing() { return playing_; }
    KMOptions &Options() { return options_; }
protected:
    virtual void DoRun() = 0;

    void Loop();

    void DoStop();

    virtual void PlaySong(KMSong *song);
private:
    void ParseAddPlaylist(const std::string &id);

    KMSongPackageItem *GetSong(unsigned int package, unsigned short song);
    KMSongPackageItem *GetSong(const std::string &id);

    std::string chars_;
    KMachineSongs songs_;
    KMachineImages images_;
    KMachinePlaylist playlist_;
    KMachineImageList imagelist_;
    KMSong *playing_;
    KMOptions options_;
};

};

#endif //H_KMACHINE_H
