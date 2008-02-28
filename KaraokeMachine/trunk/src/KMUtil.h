#ifndef H_KMUTIL_H
#define H_KMUTIL_H

#include <ostream>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>

namespace KaraokeMachine {

#ifdef __WIN32__
    extern const char *kmutil_pathsep;
#elif defined(unix)
    extern const char *kmutil_pathsep;
#else
    #error "Unknown platform"
#endif

typedef std::vector<int> KMArrayInt;

std::string kmutil_format(const std::string &format, ...);
std::vector<std::string> kmutil_tokenize(const std::string& str,const std::string& delimiters);
std::string kmutil_getfilename(const std::string &path);
std::string kmutil_getfileext(const std::string &path);
void kmutil_copystream(std::istream &source, std::ostream &dest);
void kmutil_usleep(unsigned int usecs);

class KMException : public std::runtime_error
{
public:
    KMException(const std::string &what) : std::runtime_error(what) {}
};

class KMTags
{
public:
    KMTags() : taglist_() {}

    std::string GetTags();
    void SetTags(const std::string &v);
private:
    typedef std::set<std::string> taglist_t;
    taglist_t taglist_;
};

typedef std::iostream KMStream;
typedef std::istream KMInputStream;
typedef std::ostream KMOutputStream;


};

#endif //H_KMUTIL_H
