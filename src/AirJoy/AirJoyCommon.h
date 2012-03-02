/*
 * jxfengzi@gmail.com 
 *
 * AirJoyCommon.h
 *
 * 2012-3-1
 *
 */

#ifndef __AIRJOY_COMMON_H__
#define __AIRJOY_COMMON_H__

namespace airjoy
{

    typedef int AirJoySessionId;

    class Util
    {
    public:
      static void airJoySleep(int second);
      static bool initSocket(void);
    };
}

#endif // __AIRJOY_COMMON_H__
