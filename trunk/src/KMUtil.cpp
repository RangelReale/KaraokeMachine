#include "KMUtil.h"

#include <cstdarg>

#ifdef __WIN32__
#include <windows.h>
#endif
#ifdef unix
#include <unistd.h> // for usleep
#endif

namespace KaraokeMachine {

#ifdef __WIN32__
    const char *kmutil_pathsep="\\";
#elif defined(unix)
    const char *kmutil_pathsep="/";
#else
    #error "Unknown platform"
#endif

std::string kmutil_format(const std::string &format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    int asz=format.length()*10;
    char *rstr=new char[asz];

    int sz=vsnprintf(rstr, asz, format.c_str(), argptr);
    if (sz<0)
    {
        rstr[0]=0;
    }
    else if (sz>asz)
    {
        delete [] rstr;
        char *rstr=new char[sz];
        sz=vsnprintf(rstr, asz, format.c_str(), argptr);
        if (sz<0)
            rstr[0]=0;
    }

    va_end(argptr);

    std::string ret(rstr);
    delete [] rstr;
    return ret;
}


std::vector<std::string> kmutil_tokenize(const std::string& str,const std::string& delimiters)
{
	std::vector<std::string> tokens;

	// skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	// find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));

        // skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);

        // find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }

	return tokens;
}

std::string kmutil_getfilename(const std::string &path)
{
	int pos = 0;

	pos = static_cast<int>(path.find_last_of(kmutil_pathsep));           //find the last slash in the path
	if ( pos != static_cast<int>(std::string::npos) )                //if found
	{
		return path.substr(pos + 1, path.length() - pos );  //return the substring representing the file name
	}
	return path;
}

std::string kmutil_getfileext(const std::string &path)
{
	int pos = 0;

	pos = static_cast<int>(path.find_last_of("."));           //find the last dot in the path
	if ( pos != static_cast<int>(std::string::npos) )                //if found
	{
		return path.substr(pos + 1, path.length() - pos );  //return the substring representing the file ext
	}
	return "";
}

void kmutil_copystream(std::istream &source, std::ostream &dest)
{
    char buffer[1024];
    do {
        source.read(buffer, 1024);
        if (source.gcount()>0)
            dest.write(buffer, source.gcount());
        if (source.eof()||source.fail())
            break;
    } while(true);
}

void kmutil_usleep(unsigned int usecs)
{
#ifdef __WIN32__
   /* On Windows, round to the nearest millisecond, with a
    * minimum of 1 millisecond if usleep was called with a
    * a non-zero value. */
   if (usecs > 500)
    Sleep ((usecs+500)/1000);
   else if (usecs > 0)
     Sleep (1);
   else
     Sleep (0);
#elif defined(unix)
    usleep(usecs);
#else
    #error "Unknown platform"
#endif
}

std::string KMTags::GetTags()
{
    std::string ret;
    for (taglist_t::const_iterator i=taglist_.begin(); i!=taglist_.end(); i++)
    {
        if (!ret.empty())
            ret+=",";
        ret+=*i;
    }
    return ret;
}

void KMTags::SetTags(const std::string &v)
{
    taglist_.clear();
    std::vector<std::string> tokens=kmutil_tokenize(v, ",");
    for (std::vector<std::string>::const_iterator i=tokens.begin(); i!=tokens.end(); i++)
        taglist_.insert(*i);
}



};
