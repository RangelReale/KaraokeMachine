#include "KMSong.h"
#include <cstdio>
#include <fstream>
#include <sstream>
#include <ctime>
#include <memory>
#include <algorithm>
#include <iterator>

namespace KaraokeMachine {


/////////////////////////////////
// CLASS
//      KMSong
/////////////////////////////////

void KMSong::AddLyricsLine(const std::string &line)
{
    lyrics_.lines_.push_back(line);
}

/////////////////////////////////
// CLASS
//      KMSongPackageItem
/////////////////////////////////

void KMSongPackageItem::GetFileData(std::ostream &stream) const
{
    if (filepos_>0)
    {
        if (!package_->GetFile())
            throw KMException("File not found");
        char fdata[filesize_];
        package_->GetFile()->seekg(filepos_);
        package_->GetFile()->read((char*)fdata, filesize_);
        stream.write((char*)fdata, filesize_);
    }
    else
    {
        std::ifstream f(filename_.c_str(), std::ios::in|std::ios::binary);
        kmutil_copystream(f, stream);
    }
}

/////////////////////////////////
// CLASS
//      KMSongPackage
/////////////////////////////////

KMSongPackage::~KMSongPackage()
{
    if (file_)
        delete file_;
}

unsigned short KMSongPackage::Add(const std::string &songname,
    const std::string &filename, unsigned short id)
{
    if (id==0) id=++maxid_;
    if (id>maxid_) maxid_=id;
    songs_.insert( std::pair<unsigned short, KMSongPackageItem>(id, KMSongPackageItem(this, id, songname, filename)) );
    return id;
}

unsigned short KMSongPackage::Add(unsigned short id)
{
    if (id==0) id=++maxid_;
    if (id>maxid_) maxid_=id;
    songs_.insert( std::pair<unsigned short, KMSongPackageItem>(id, KMSongPackageItem(this, id)) );
    return id;
}

bool KMSongPackage::Exists(unsigned short id)
{
    return songs_.find(id)!=songs_.end();
}

void KMSongPackage::Remove(unsigned short id)
{
    songs_.erase(id);
}

KMSongPackageItem &KMSongPackage::Get(unsigned short id)
{
    songs_t::iterator i=songs_.find(id);
    if (i==songs_.end())
        throw KMException("Song not found");
    return i->second;
}

void KMSongPackage::ChangeId(unsigned short oldid, unsigned short newid)
{
    songs_t::iterator i=songs_.find(oldid);
    if (i==songs_.end()) return;

    songs_.insert(songs_t::value_type(newid, i->second));
    songs_.find(newid)->second.id_=newid;
    songs_.erase(oldid);
}

void KMSongPackage::Ids(KMArrayInt &ids)
{
    for (songs_t::iterator i=songs_.begin(); i!=songs_.end(); i++)
    {
        ids.push_back(i->first);
    }
}

void KMSongPackage::Load(KMInputStream &stream)
{
    kmsongpackage_header_t head;
    kmsongpackage_song_t song;

    char tmp[KMSONGPACKAGE_MAXPATH+1];

    // read head
    stream.read((char*)&head, sizeof(head));
    if (strncmp(head.sig, KMSONGPACKAGE_SIG, KMSONGPACKAGE_SIG_SIZE)!=0)
        throw KMException("Invalid kms file (1)");
    defaultid_=kmutil_endian_int(head.packageid);

    memset(tmp, 0, sizeof(tmp));
    strncpy(tmp, head.title, KMSONGPACKAGE_MAXTEXT);
    title_=tmp;

    memset(tmp, 0, sizeof(tmp));
    strncpy(tmp, head.author, KMSONGPACKAGE_MAXTEXT);
    author_=tmp;

    description_="";
    if (kmutil_endian_short(head.descriptionsize)>0)
    {
        char ddata[kmutil_endian_short(head.descriptionsize)+1];
        ddata[kmutil_endian_short(head.descriptionsize)]='\0';
        stream.read(ddata, kmutil_endian_short(head.descriptionsize));
        description_=ddata;
    }

    // read songs
    songs_.clear();

    for (int i=0; i<kmutil_endian_short(head.songcount); i++)
    {
        stream.read((char*)&song, sizeof(song));
        if (stream.gcount()!=sizeof(song))
            throw KMException("Invalid kms file (2)");
        if (strncmp(song.magic, KMSONGPACKAGE_MAGIC_SONG, KMSONGPACKAGE_MAGIC_SONG_SIZE)!=0)
            throw KMException("Invalid kms file (3)");

        KMSongPackageItem *newsong=&Get(Add(kmutil_endian_short(song.id)));

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, song.filename, KMSONGPACKAGE_MAXPATH);
        newsong->filename_=tmp;

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, song.title, KMSONGPACKAGE_MAXTEXT);
        newsong->title_=tmp;

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, song.artist, KMSONGPACKAGE_MAXTEXT);
        newsong->artist_=tmp;

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, song.artist2, KMSONGPACKAGE_MAXTEXT);
        newsong->artist2_=tmp;

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, song.genre, KMSONGPACKAGE_MAXGENRE);
        newsong->genre_=tmp;

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, song.subgenre, KMSONGPACKAGE_MAXGENRE);
        newsong->subgenre_=tmp;

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, song.startlyrics, KMSONGPACKAGE_MAXTEXT);
        newsong->startlyrics_=tmp;

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, song.tags, KMSONGPACKAGE_MAXTAGS);
        newsong->Tags().SetTags(tmp);

        newsong->melodytrack_=song.melodytrack;
        newsong->transpose_=song.transpose;
        newsong->filepos_=stream.tellg();
        newsong->filesize_=kmutil_endian_int(song.filesize);

        stream.seekg(kmutil_endian_int(song.filesize), std::ios::cur);
    }
}

void KMSongPackage::Load(const std::string &filename)
{
    if (file_)
    {
        delete file_;
        file_=NULL;
    }

    file_=new std::fstream(filename.c_str(), std::ios::in|std::ios::binary);
    Load(*file_);
}

void KMSongPackage::Save(KMOutputStream &stream)
{
    kmsongpackage_header_t head;
    kmsongpackage_song_t song;

    // write head
    memset(&head, 0, sizeof(head));
    strncpy(head.sig, KMSONGPACKAGE_SIG, KMSONGPACKAGE_SIG_SIZE);
    head.version=KMSONGPACKAGE_VERSION;
    head.packageid=defaultid_;
    strncpy(head.title, title_.c_str(), KMSONGPACKAGE_MAXTEXT);
    strncpy(head.author, author_.c_str(), KMSONGPACKAGE_MAXTEXT);
    head.songcount=songs_.size();
    head.descriptionsize=description_.length();

    stream.write((char*)&head, sizeof(head));

    // write description
    stream.write(description_.c_str(), description_.length());

    // write songs
    for (songs_t::iterator i=songs_.begin(); i!=songs_.end(); i++)
    {
        std::stringstream filedata;

        memset(&song, 0, sizeof(song));
        strncpy(song.magic, KMSONGPACKAGE_MAGIC_SONG, KMSONGPACKAGE_MAGIC_SONG_SIZE);
        song.id=i->second.GetId();
        strncpy(song.filename, kmutil_getfilename(i->second.filename_).c_str(), KMSONGPACKAGE_MAXPATH);
        strncpy(song.title, i->second.title_.c_str(), KMSONGPACKAGE_MAXTEXT);
        strncpy(song.artist, i->second.artist_.c_str(), KMSONGPACKAGE_MAXTEXT);
        strncpy(song.artist2, i->second.artist2_.c_str(), KMSONGPACKAGE_MAXTEXT);
        strncpy(song.genre, i->second.genre_.c_str(), KMSONGPACKAGE_MAXGENRE);
        strncpy(song.subgenre, i->second.subgenre_.c_str(), KMSONGPACKAGE_MAXGENRE);
        strncpy(song.startlyrics, i->second.startlyrics_.c_str(), KMSONGPACKAGE_MAXTEXT);
        song.melodytrack=i->second.melodytrack_;
        song.transpose=i->second.transpose_;
        strncpy(song.tags, i->second.Tags().GetTags().c_str(), KMSONGPACKAGE_MAXTAGS);

        i->second.GetFileData(filedata);
        filedata.seekg(0, std::ios::end);
        song.filesize=filedata.tellg();
        filedata.seekg(0);

        // update song information
        i->second.filesize_=song.filesize;
        i->second.filename_=kmutil_getfilename(i->second.filename_);

        stream.write((char*)&song, sizeof(song));

        // write file data
        i->second.filepos_=stream.tellp();
        kmutil_copystream(filedata, stream);
    }
}

void KMSongPackage::Save(const std::string &filename)
{
    std::string tfilename(filename+kmutil_format(".%d", time(NULL)));

    std::fstream f(tfilename.c_str(), std::ios::out|std::ios::binary|std::ios::trunc);
    Save(f);
    f.close();

    // move temp file to file
    if (file_)
    {
        delete file_;
        file_=NULL;
    }
    remove(filename.c_str());
    if (rename(tfilename.c_str(), filename.c_str())!=0)
        throw KMException("Error renaming file");
    file_=new std::fstream(filename.c_str(), std::ios::in|std::ios::binary);
}

};
