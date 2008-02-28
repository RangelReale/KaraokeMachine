#include "KMSongFactory.h"

#include "KMSong_TSE3.h"

namespace KaraokeMachine {

/////////////////////////////////
// CLASS
//      KMSongFactory
/////////////////////////////////
KMSong* KMSongFactory::Load(const std::string &filename)
{
    return new KMSong_TSE3(filename);
}

KMSong* KMSongFactory::Load(KMInputStream &stream)
{
    return new KMSong_TSE3(stream);
}


//KMSongFactory songfactory;

};

