#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <iostream>

// logging and debug levels
#define LOG_INFO 0x002
#define LOG_ERROR 0x004
#define LOG_SYS 0x008
#define LOG_DEBUG_LEVEL1 0x010
#define LOG_DEBUG_LEVEL2 0x020
#define LOG_DEBUG_LEVEL3 0x040
#define LOG_DEBUG_LEVEL4 0x080
#define LOG_ALL 0x0FE

#define LOG_MASK (LOG_INFO | LOG_ERROR | LOG_SYS | LOG_DEBUG_LEVEL1 | LOG_DEBUG_LEVEL2 | LOG_DEBUG_LEVEL3 | LOG_DEBUG_LEVEL4)
#define OUT_STREAM std::cout

#ifdef ENABLE_LOG
#define LOG(msg,...) ( logger::printLog( OUT_STREAM, LOG_INFO, __FILE__, __LINE__, __FUNCTION__, (msg), ##__VA_ARGS__ ) )
#define LOG_DEBUG(level, msg,...) ( logger::printLog( OUT_STREAM, (level), __FILE__, __LINE__, __FUNCTION__, (msg), ##__VA_ARGS__ ) )
#else
#define DEBUG_BREAK()
#define LOG(msg,...)
#define LOG_DEBUG(level, msg,...)
#endif

namespace logger
{

void printLog(std::ostream& stream, unsigned int level, const char *file, int line, const char *func, const char *str, ...);

}

#endif
