#include "KMImage.h"
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
//      KMImagePackageItem
/////////////////////////////////

void KMImagePackageItem::GetFileData(std::ostream &stream) const
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
//      KMImagePackage
/////////////////////////////////

KMImagePackage::~KMImagePackage()
{
    if (file_)
        delete file_;
}

int KMImagePackage::Add(const std::string &filename)
{
    images_.push_back( KMImagePackageItem(this, filename) );
    return images_.size()-1;
}

int KMImagePackage::Add()
{
    images_.push_back( KMImagePackageItem(this) );
    return images_.size()-1;
}

void KMImagePackage::Remove(int index)
{
    images_.erase(images_.begin()+index);
}

KMImagePackageItem &KMImagePackage::Get(int index)
{
    return images_[index];
}

void KMImagePackage::Load(KMInputStream &stream)
{
    kmimagepackage_header_t head;
    kmimagepackage_image_t image;

    char tmp[KMIMAGEPACKAGE_MAXPATH+1];

    // read head
    stream.read((char*)&head, sizeof(head));
    if (strncmp(head.sig, KMIMAGEPACKAGE_SIG, KMIMAGEPACKAGE_SIG_SIZE)!=0)
        throw KMException("Invalid file");

    memset(tmp, 0, sizeof(tmp));
    strncpy(tmp, head.title, KMIMAGEPACKAGE_MAXTEXT);
    title_=tmp;

    memset(tmp, 0, sizeof(tmp));
    strncpy(tmp, head.author, KMIMAGEPACKAGE_MAXTEXT);
    author_=tmp;

    description_="";
    if (head.descriptionsize>0)
    {
        std::auto_ptr<char> ddata(new char[head.descriptionsize+1]);
        ddata.get()[head.descriptionsize]='\0';
        stream.read((char*)ddata.get(), head.descriptionsize);
        description_=ddata.get();
    }

    // read images
    images_.clear();

    for (int i=0; i<head.imagecount; i++)
    {
        stream.read((char*)&image, sizeof(image));
        if (stream.gcount()!=sizeof(image))
            throw KMException("Invalid file");
        if (strncmp(image.magic, KMIMAGEPACKAGE_MAGIC_IMAGE, KMIMAGEPACKAGE_MAGIC_IMAGE_SIZE)!=0)
            throw KMException("Invalid file");

        KMImagePackageItem *newimage=&Get(Add());

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, image.filename, KMIMAGEPACKAGE_MAXPATH);
        newimage->filename_=tmp;

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, image.title, KMIMAGEPACKAGE_MAXTEXT);
        newimage->title_=tmp;

        newimage->iswide_=image.iswide;

        newimage->filepos_=stream.tellg();
        newimage->filesize_=image.filesize;

        stream.seekg(image.filesize, std::ios::cur);
    }
}

void KMImagePackage::Load(const std::string &filename)
{
    if (file_)
    {
        delete file_;
        file_=NULL;
    }

    file_=new std::fstream(filename.c_str(), std::ios::in|std::ios::binary);
    Load(*file_);
}

void KMImagePackage::Save(KMOutputStream &stream)
{
    kmimagepackage_header_t head;
    kmimagepackage_image_t image;

    // write head
    memset(&head, 0, sizeof(head));
    strncpy(head.sig, KMIMAGEPACKAGE_SIG, KMIMAGEPACKAGE_SIG_SIZE);
    head.version=KMIMAGEPACKAGE_VERSION;
    strncpy(head.title, title_.c_str(), KMIMAGEPACKAGE_MAXTEXT);
    strncpy(head.author, author_.c_str(), KMIMAGEPACKAGE_MAXTEXT);
    head.imagecount=images_.size();
    head.descriptionsize=description_.length();

    stream.write((char*)&head, sizeof(head));

    // write description
    stream.write(description_.c_str(), description_.length());

    // write images
    for (images_t::iterator i=images_.begin(); i!=images_.end(); i++)
    {
        std::stringstream filedata;

        memset(&image, 0, sizeof(image));
        strncpy(image.magic, KMIMAGEPACKAGE_MAGIC_IMAGE, KMIMAGEPACKAGE_MAGIC_IMAGE_SIZE);
        strncpy(image.filename, kmutil_getfilename(i->filename_).c_str(), KMIMAGEPACKAGE_MAXPATH);
        strncpy(image.title, i->title_.c_str(), KMIMAGEPACKAGE_MAXTEXT);
        image.iswide=i->iswide_;

        i->GetFileData(filedata);
        filedata.seekg(0, std::ios::end);
        image.filesize=filedata.tellg();
        filedata.seekg(0);

        // update image information
        i->filesize_=image.filesize;
        i->filename_=kmutil_getfilename(i->filename_);

        stream.write((char*)&image, sizeof(image));

        // write file data
        i->filepos_=stream.tellp();
        kmutil_copystream(filedata, stream);
    }
}

void KMImagePackage::Save(const std::string &filename)
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
