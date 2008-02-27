#include <stdlib.h>
#include <dirent.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "boost/filesystem.hpp"
#include "boost/random.hpp"

#include "KMBackend.h"
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

void KMachineSongs::Ids(KMArrayInt &ids)
{
    for (packages_t::const_iterator i=packages_.begin(); i!=packages_.end(); i++)
    {
        ids.push_back(i->first);
    }
}


unsigned int KMachineSongs::Load(const std::string &filename)
{
    linked_ptr<KMSongPackage> newp=linked_ptr<KMSongPackage>(new KMSongPackage(0));
    newp->Load(filename);

    unsigned int did=newp->GetDefaultId();
    while (Exists(did)) did++;
    newp->SetId(did);
    packages_[did]=newp;
    return did;
}

int KMachineSongs::LoadPath(const std::string &path)
{
    if (!boost::filesystem::exists(path)) return 0;

   int loadcount=0;

    for (boost::filesystem::directory_iterator d(path);
        d!=boost::filesystem::directory_iterator(); ++d)
    {
        if (!boost::filesystem::is_directory(d->status()) &&
            boost::filesystem::extension(d->leaf())==".kms")
        {
            Load(d->string());
            loadcount++;
        }
    }
    return loadcount;
}

/////////////////////////////////
// CLASS
//      KMachineImages
/////////////////////////////////
KMImagePackage &KMachineImages::Get(unsigned int id)
{
    packages_t::iterator i=packages_.find(id);
    if (i==packages_.end())
        throw KMException("Image package not found");
    return *i->second;
}

bool KMachineImages::Exists(unsigned int id)
{
    packages_t::iterator i=packages_.find(id);
    return i!=packages_.end();
}

unsigned int KMachineImages::Add()
{
    unsigned int id=++maxid_;
    packages_.insert( std::pair<unsigned int, linked_ptr<KMImagePackage> >(id, linked_ptr<KMImagePackage>(new KMImagePackage(id))) );
    return id;
}

void KMachineImages::Ids(KMArrayInt &ids)
{
    for (packages_t::const_iterator i=packages_.begin(); i!=packages_.end(); i++)
    {
        ids.push_back(i->first);
    }
}

unsigned int KMachineImages::Load(const std::string &filename)
{
    linked_ptr<KMImagePackage> newp=linked_ptr<KMImagePackage>(new KMImagePackage(0));
    newp->Load(filename);

    unsigned int id=++maxid_;
    newp->SetId(id);
    packages_[id]=newp;
    return id;
}

int KMachineImages::LoadPath(const std::string &path)
{
    if (!boost::filesystem::exists(path)) return 0;

    int loadcount=0;

    for (boost::filesystem::directory_iterator d(path);
        d!=boost::filesystem::directory_iterator(); ++d)
    {
        if (!boost::filesystem::is_directory(d->status()) &&
            boost::filesystem::extension(d->leaf())==".kmi")
        {
            Load(d->string());
            loadcount++;
        }
    }
    return loadcount;
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
//      KMachineImageListImage
/////////////////////////////////
KMImagePackageItem *KMachineImageListImage::GetImage()
{
    return &imagelist_->Machine()->Images().Get(package_).Get(image_);
}


/////////////////////////////////
// CLASS
//      KMachineImageList
/////////////////////////////////
int KMachineImageList::Add(unsigned int package, unsigned short image)
{
    images_.push_back(KMachineImageListImage(this, package, image));
    return images_.size()-1;
}

void KMachineImageList::Remove(int index)
{
    images_.erase(images_.begin()+index);
}

KMachineImageListImage &KMachineImageList::GetRandom()
{
    if (GetCount()==0)
        throw KMException("No items");

    boost::mt19937 rng;
    rng.seed(static_cast<unsigned int>(std::time(0)));
    boost::uniform_int<> rval(0,GetCount()-1);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> >
           die(rng, rval);

    return Get(die());
}

/////////////////////////////////
// CLASS
//      KMachine
/////////////////////////////////

KMachine::~KMachine()
{
    DoStop();
}

void KMachine::Initialize()
{
    KMArrayInt ids, idsitem;
    Images().Ids(ids);

    for (KMArrayInt::iterator i=ids.begin(); i!=ids.end(); i++)
    {
        Images().Get(*i).Ids(idsitem);
        for (KMArrayInt::iterator ii=idsitem.begin(); ii!=idsitem.end(); ii++)
        {
            imagelist_.Add(*i, *ii);
        }
    }
}

void KMachine::Finalize()
{

}

void KMachine::Run()
{
    Initialize();
    DoRun();
    Finalize();
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
        //std::cout << "KMachine::Loop playing" << std::endl;
/*
        if (!playing_->Poll())
        {
            DoStop();
        }
*/
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

        PlaySong(playing_);
        //playing_->Play();
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

void KMachine::PlaySong(KMSong *song)
{

}

};
