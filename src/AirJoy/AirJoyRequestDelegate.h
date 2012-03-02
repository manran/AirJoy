/*
 * AirJoyRequestDelegate.h
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-3-1
 *
 */

#ifndef __AIRJOY_REQUEST_DELEGATE_H__
#define __AIRJOY_REQUEST_DELEGATE_H__

#include "AirJoyCommon.h"

namespace airjoy
{

  class AirJoyMessage;

  class AirJoyRequestDelegate
  {
  public:
    AirJoyRequestDelegate() {}
    virtual ~AirJoyRequestDelegate() {}

    virtual void didNotInitSocket(void) {}
    virtual void didNotConnectServer(void) {}
    virtual void didNotSendToServer(void) {}
    virtual void didNotRecvFromServer(void) {}

    virtual void didConnectServer(void) {}
    virtual void didSendToServer(void) {}
    virtual void didRecvFromServer(AirJoyMessage &response, AirJoySessionId sessionId) {}
  };


}

#endif // __AIRJOY_REQUEST_DELEGATE_H__
