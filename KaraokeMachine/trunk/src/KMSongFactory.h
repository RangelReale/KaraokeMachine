#ifndef H_KMSONGFACTORY_H
#define H_KMSONGFACTORY_H

#include "KMSong.h"
#include "KMUtil.h"


namespace KaraokeMachine {

/**
 * @class KMSongFactory
 *
 * @brief Song factory
 */
class KMSongFactory
{
public:
    static KMSong* Load(const std::string &filename);
    static KMSong* Load(KMInputStream &stream);
};


//extern KMSongFactory songfactory;


};

#endif //H_KMSONGFACTORY_H
