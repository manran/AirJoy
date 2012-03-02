/*
 * AirJoyProcessorDelegate.h
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-3-1
 *
 */

#ifndef __AIRJOY_PROCESSOR_DELEGATE_H__
#define __AIRJOY_PROCESSOR_DELEGATE_H__

namespace airjoy
{

  class AirJoyMessage;

  class AirJoyProcessorDelegate
  {
  public:
    AirJoyProcessorDelegate() {}
    virtual ~AirJoyProcessorDelegate() {}

    // return true if you handle this request, otherwise return false;
    virtual bool didReceiveRequest(AirJoyProcessor *processor, AirJoyMessage &request) { return false; }
  };


}

#endif // __AIRJOY_PROCESSOR_DELEGATE_H__
