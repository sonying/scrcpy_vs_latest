#pragma once
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#define DEFAULT_BIT_RATE 8000000

#define DEFAULT_LOCAL_PORT_RANGE_FIRST 27183

#define DEFAULT_LOCAL_PORT_RANGE_LAST 27199

#define DEFAULT_LOCK_VIDEO_ORIENTATION -1

#define DEFAULT_MAX_SIZE 0

#define HIDPI_SUPPORT

//#define NDEBUG

#define PORTABLE

#define PREFIX "/usr/local"

#define SCRCPY_VERSION "1.16"

#undef SERVER_DEBUGGER

#define SERVER_DEBUGGER_METHOD_NEW

#define WINDOWS_NOCONSOLE
