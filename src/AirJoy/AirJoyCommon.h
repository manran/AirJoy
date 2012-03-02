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

  // 请求缺省超时时间: 1s
  #define DefaultAirJoyRequestTimeout 1

  class Util
  {
  public:
    static void airJoySleep(int second);
    static bool initSocket(void);
  };

}

#endif // __AIRJOY_COMMON_H__
