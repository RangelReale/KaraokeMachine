#ifndef H_KMUTIL_H
#define H_KMUTIL_H

#include <ostream>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>

namespace KaraokeMachine {

typedef std::vector<int> KMArrayInt;

std::string kmutil_format(const std::string &format, ...);
std::vector<std::string> kmutil_tokenize(const std::string& str,const std::string& delimiters);
std::string kmutil_getfilename(const std::string &path);
void kmutil_copystream(std::istream &source, std::ostream &dest);

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
