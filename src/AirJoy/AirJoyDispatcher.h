/*
 * AirJoyDispatcher.h
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-3-1
 *
 */

#ifndef __AIRJOY_DISPATCHER_H__
#define __AIRJOY_DISPATCHER_H__

namespace airjoy
{

  class AirJoyMessage;

  class AirJoyDispatcher
  {
  public:
    AirJoyDispatcher() {}
    virtual ~AirJoyDispatcher() {}

    // return true if you handle this request, otherwise return false;
    virtual bool didReceive(AirJoyProcessor *processor, AirJoyMessage &request) { return false; }
  };


}

#endif // __AIRJOY_DISPATCHER_H__
