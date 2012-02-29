/*
 * AirJoyConfig.h
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-2-29
 *
 */

#ifndef __AIRJOY_CONFIG_H__
#define __AIRJOY_CONFIG_H__

namespace airjoy
{

  class AirJoyConfig
  {
  public:
    AirJoyConfig() {};
    virtual ~AirJoyConfig() {}

    virtual int port(void) = 0;
    virtual void setPort(int port) = 0;
  };

}


#endif // __AIRJOY_CONFIG_H__
