/*
 * AirJoyCommon.cpp
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-2-29
 *
 */

#include "AirJoyCommon.h"

#ifdef _WIN32
#   include <windows.h>
#else
#   include <time.h>
#   include <sys/time.h>
#   include <unistd.h>
#endif // _WIN32

using namespace airjoy;

void AirJoySleep(int second)
{
#ifdef _WIN32
    Sleep(second * 1000);
#else
    sleep(second);
#endif
}

