#ifndef H_KMUTIL_H
#define H_KMUTIL_H

#include <ostream>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

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

typedef std::iostream KMStream;
typedef std::istream KMInputStream;
typedef std::ostream KMOutputStream;


};

#endif //H_KMUTIL_H
