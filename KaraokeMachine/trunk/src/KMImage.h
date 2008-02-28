#ifndef H_KMIMAGE_H
#define H_KMIMAGE_H

#include "KMDefs.h"
#include "KMUtil.h"

#include <string>
#include <deque>
#include <map>
#include <ostream>

namespace KaraokeMachine {

// forward declarations
class KMImagePackage;

/**
 * @class KMImagePackageItem
 *
 * @brief Image package image
 */
class KMImagePackageItem {
public:
    KMImagePackageItem(KMImagePackage *package, unsigned short id, std::string title = "",
        std::string filename = "") : package_(package), id_(id),
        title_(title), filename_(filename), iswide_(false),
        filepos_(0), filesize_(0), tags_() {}

    KMImagePackage *Package() const { return package_; }

    unsigned short GetId() const { return id_; }
    const std::string &GetTitle() const { return title_; }
    const std::string &GetFilename() const { return filename_; }
    bool GetIsWide() const { return iswide_; }

    void SetTitle(const std::string &v) { title_=v; }
    void SetFilename(const std::string &v) { filename_=v; }
    void SetIsWide(bool v) { iswide_=v; }

    KMTags &Tags() { return tags_; }

    void GetFileData(std::ostream &stream) const;
private:
    KMImagePackage *package_;
    unsigned short id_;
    std::string title_, filename_;
    bool iswide_;
    unsigned int filepos_, filesize_;
    KMTags tags_;

    friend class KMImagePackage;
};

/**
 * @class KMImagePackage
 *
 * @brief Image package
 */
class KMImagePackage {
public:
    KMImagePackage(unsigned int id) : id_(id),
        maxid_(0),
        title_(""), author_(""), description_(""),
        images_(), file_(NULL) {}
    ~KMImagePackage();

    unsigned short Add(const std::string &imagename,
        const std::string &filename, unsigned short id = 0);
    bool Exists(unsigned short id);
    KMImagePackageItem &Get(unsigned short id);
    void Remove(unsigned short id);
    void ChangeId(unsigned short oldid, unsigned short newid);
    void Ids(KMArrayInt &ids);

    unsigned int GetId() { return id_; }
    const std::string &GetTitle() { return title_; }
    const std::string &GetAuthor() { return author_; }
    const std::string &GetDescription() { return description_; }

    void SetId(unsigned int v) { id_=v; }
    void SetTitle(const std::string &v) { title_=v; }
    void SetAuthor(const std::string &v) { author_=v; }
    void SetDescription(const std::string &v) { description_=v; }

    void Load(const std::string &filename);
    void Save(const std::string &filename);

    KMInputStream *GetFile() { return file_; }
private:
    typedef std::map<unsigned short, KMImagePackageItem> images_t;

    unsigned short Add(unsigned short id);

    void Load(KMInputStream &stream);
    void Save(KMOutputStream &stream);

    unsigned int id_;
    unsigned short maxid_;
    std::string title_, author_, description_;
    images_t images_;
    KMStream *file_;
};

};

#endif //H_KMIMAGE_H
