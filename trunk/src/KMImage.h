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
    KMImagePackageItem(KMImagePackage *package,
        std::string filename = "") : package_(package),
        title_(""), filename_(filename), iswide_(false),
        filepos_(0), filesize_(0) {}

    KMImagePackage *Package() const { return package_; }

    const std::string &GetTitle() const { return title_; }
    const std::string &GetFilename() const { return filename_; }
    bool GetIsWide() const { return iswide_; }

    void SetTitle(const std::string &v) { title_=v; }
    void SetFilename(const std::string &v) { filename_=v; }
    void SetIsWide(bool v) { iswide_=v; }

    void GetFileData(std::ostream &stream) const;
private:
    KMImagePackage *package_;
    std::string title_, filename_;
    bool iswide_;
    unsigned int filepos_, filesize_;

    friend class KMImagePackage;
};

/**
 * @class KMImagePackage
 *
 * @brief Image package
 */
class KMImagePackage {
public:
    KMImagePackage() :
        title_(""), author_(""), description_(""),
        images_(), file_(NULL) {}
    ~KMImagePackage();

    int Add(const std::string &filename);
    KMImagePackageItem &Get(int index);
    void Remove(int index);
    int GetCount() { return images_.size(); }

    const std::string &GetTitle() { return title_; }
    const std::string &GetAuthor() { return author_; }
    const std::string &GetDescription() { return description_; }

    void SetTitle(const std::string &v) { title_=v; }
    void SetAuthor(const std::string &v) { author_=v; }
    void SetDescription(const std::string &v) { description_=v; }

    void Load(const std::string &filename);
    void Save(const std::string &filename);

    KMInputStream *GetFile() { return file_; }
private:
    typedef std::deque<KMImagePackageItem> images_t;

    int Add();

    void Load(KMInputStream &stream);
    void Save(KMOutputStream &stream);

    std::string title_, author_, description_;
    images_t images_;
    KMStream *file_;
};

};

#endif //H_KMIMAGE_H
