#include "utils/log.hpp"
#include <stdarg.h>

namespace logger
{

int _vscprintf (const char * format, va_list pargs)
{
    int retval;
    va_list argcopy;
    va_copy(argcopy, pargs);
    retval = vsnprintf(NULL, 0, format, argcopy);
    va_end(argcopy);
    return retval;
}

void printLog(std::ostream& stream, unsigned int level, const char *file, int line, const char *func, const char *str, ...)
{
    va_list varptr;
    va_start( varptr, str);

    int n = _vscprintf(str, varptr);
    std::string buf;
    buf.resize(n + 1);

    vsprintf(&buf[0], str, varptr);

    va_end(varptr);

    int clevel = level & LOG_MASK;
    if (clevel != 0)
    {
        stream << "["<<file<<":"<<line<<"] "<<func<<"(): "<<buf<<std::endl;
    }

    va_end( varptr );
}

}
