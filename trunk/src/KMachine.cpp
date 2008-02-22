#include <sstream>
#include <vector>
#include "KMachine.h"
#include "KMUtil.h"
#include "KMSongFactory.h"

namespace KaraokeMachine {

/////////////////////////////////
// CLASS
//      KMachineSongs
/////////////////////////////////
KMSongPackage &KMachineSongs::Get(unsigned int id)
{
    packages_t::iterator i=packages_.find(id);
    if (i==packages_.end())
        throw KMException("Song package not found");
    return *i->second;
}

bool KMachineSongs::Exists(unsigned int id)
{
    packages_t::iterator i=packages_.find(id);
    return i!=packages_.end();
}

unsigned int KMachineSongs::Add(unsigned int id)
{
    if (id==0) id=++maxid_;
    if (id>maxid_) maxid_=id;
    packages_.insert( std::pair<unsigned int, linked_ptr<KMSongPackage> >(id, linked_ptr<KMSongPackage>(new KMSongPackage(id))) );
    return id;
}

void KMachineSongs::Load(const std::string &filename)
{
    linked_ptr<KMSongPackage> newp=linked_ptr<KMSongPackage>(new KMSongPackage(0));
    newp->Load(filename);

    unsigned int did=newp->GetDefaultId();
    while (Exists(did)) did++;
    newp->SetId(did);
    packages_[did]=newp;
}

/////////////////////////////////
// CLASS
//      KMachinePlaylistSong
/////////////////////////////////
KMSongPackageItem *KMachinePlaylistSong::GetSong()
{
    return &playlist_->Machine()->Songs().Get(package_).Get(song_);
}

/////////////////////////////////
// CLASS
//      KMachinePlaylist
/////////////////////////////////
int KMachinePlaylist::Add(unsigned int package, unsigned short song)
{
    songs_.push_back(KMachinePlaylistSong(this, package, song));
    return songs_.size()-1;
}

void KMachinePlaylist::Remove(int index)
{
    songs_.erase(songs_.begin()+index);
}


/////////////////////////////////
// CLASS
//      KMachine
/////////////////////////////////

KMachine::~KMachine()
{
    DoStop();
}

void KMachine::AddChar(unsigned char c)
{
    chars_+=c;
}

void KMachine::RemoveChar()
{
    if (chars_.length()>0)
        chars_.erase(chars_.length()-1, 1);
}

void KMachine::DoCommand(commant_t command, int param)
{
    switch (command)
    {
    case KC_ADD:
        ParseAddPlaylist(chars_);
        chars_="";
        break;
    case KC_SKIP:
        DoStop();
        break;
    case KC_SONGKEYUP:
        if (playing_)
            playing_->SetTranspose(playing_->GetTranspose()+1);
        break;
    case KC_SONGKEYDOWN:
        if (playing_)
            playing_->SetTranspose(playing_->GetTranspose()-1);
        break;
    default:
        break;
    }
}

void KMachine::ParseAddPlaylist(const std::string &id)
{
    KMSongPackageItem *song=GetSong(id);
    if (song)
        playlist_.Add(song->Package()->GetId(), song->GetId());
}

KMSongPackageItem *KMachine::GetSong(unsigned int package, unsigned short song)
{
    if (songs_.Exists(package) && songs_.Get(package).Exists(song))
        return &songs_.Get(package).Get(song);
    return NULL;
}

KMSongPackageItem *KMachine::GetSong(const std::string &id)
{
    std::vector<std::string> parse=kmutil_tokenize(id, "*");
    if (parse.size()==2)
    {
        unsigned int package;
        unsigned short song;
        package=atoi(parse[0].c_str());
        song=atoi(parse[1].c_str());

        return GetSong(package, song);
    }
    return NULL;
}

void KMachine::Loop()
{
    if (playing_)
    {
        if (!playing_->Poll())
        {
            DoStop();
        }
    }

    if (!playing_ && playlist_.GetCount()>0)
    {
        KMachinePlaylistSong ps=playlist_.Pop();
        std::stringstream filedata;
        ps.GetSong()->GetFileData(filedata);
        filedata.seekg(0);

        playing_=KMSongFactory::Load(filedata);
        playing_->SetMelodyTrack(ps.GetSong()->GetMelodyTrack());
        playing_->SetTranspose(ps.GetSong()->GetTranspose());
        playing_->Play();
    }
}

void KMachine::DoStop()
{
    if (playing_)
    {
        playing_->Stop();
        delete playing_;
        playing_=NULL;
    }
}


};
