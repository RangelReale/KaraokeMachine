#ifndef H_KMDEFS_H
#define H_KMDEFS_H

namespace KaraokeMachine {

typedef long kmtime_t; // time in ms

#pragma pack(push, 1)

#define KMSONGPACKAGE_SIG           "KMSP"
#define KMSONGPACKAGE_SIG_SIZE      4

#define KMSONGPACKAGE_VERSION       1
#define KMSONGPACKAGE_MAXTEXT       100
#define KMSONGPACKAGE_MAXPATH       255
#define KMSONGPACKAGE_MAXGENRE      40
#define KMSONGPACKAGE_MAXTAGS       255
#define KMSONGPACKAGE_MAGIC_SONG   "KMSS"
#define KMSONGPACKAGE_MAGIC_SONG_SIZE   4

#define KMIMAGEPACKAGE_SIG           "KMIP"
#define KMIMAGEPACKAGE_SIG_SIZE      4

#define KMIMAGEPACKAGE_VERSION       1
#define KMIMAGEPACKAGE_MAXTEXT       100
#define KMIMAGEPACKAGE_MAXPATH       255
#define KMIMAGEPACKAGE_MAXTAGS       255
#define KMIMAGEPACKAGE_MAGIC_IMAGE   "KMII"
#define KMIMAGEPACKAGE_MAGIC_IMAGE_SIZE   4

typedef struct __attribute__ ((__packed__)) kmsongpackage_header_t
{
    char sig[KMSONGPACKAGE_SIG_SIZE];
    unsigned short version;
    unsigned int packageid;
    char title[KMSONGPACKAGE_MAXTEXT];
    char author[KMSONGPACKAGE_MAXTEXT];
    unsigned short songcount;
    unsigned short descriptionsize;
};

typedef struct __attribute__ ((__packed__)) kmsongpackage_song_t
{
    char magic[4];
    unsigned short id;
    char filename[KMSONGPACKAGE_MAXPATH];
    char title[KMSONGPACKAGE_MAXTEXT];
    char artist[KMSONGPACKAGE_MAXTEXT];
    char artist2[KMSONGPACKAGE_MAXTEXT];
    char genre[KMSONGPACKAGE_MAXGENRE];
    char subgenre[KMSONGPACKAGE_MAXGENRE];
    char startlyrics[KMSONGPACKAGE_MAXTEXT];
    char melodytrack;
    char transpose;
    char tags[KMSONGPACKAGE_MAXTAGS];
    unsigned int filesize;
};

typedef struct __attribute__ ((__packed__)) kmimagepackage_header_t
{
    char sig[KMIMAGEPACKAGE_SIG_SIZE];
    unsigned short version;
    char title[KMIMAGEPACKAGE_MAXTEXT];
    char author[KMIMAGEPACKAGE_MAXTEXT];
    unsigned short imagecount;
    unsigned short descriptionsize;
};

typedef struct __attribute__ ((__packed__)) kmimagepackage_image_t
{
    char magic[4];
    unsigned short id;
    char filename[KMSONGPACKAGE_MAXPATH];
    char title[KMSONGPACKAGE_MAXTEXT];
    unsigned char iswide;
    char tags[KMIMAGEPACKAGE_MAXTAGS];
    unsigned int filesize;
};


#pragma pack(pop)
};

#endif //H_KMDEFS_H

