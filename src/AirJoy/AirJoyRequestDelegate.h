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

    virtual void didNotInitSocket(AirJoySessionId sessionId) {}
    virtual void didNotConnectServer(AirJoySessionId sessionId) {}
    virtual void didNotSendToServer(AirJoySessionId sessionId) {}
    virtual void didNotRecvFromServer(AirJoySessionId sessionId) {}

    virtual void didConnectServer(AirJoySessionId sessionId) {}
    virtual void didSendToServer(AirJoySessionId sessionId) {}
    virtual void didRecvFromServer(AirJoyMessage &response, AirJoySessionId sessionId) {}
  };


}

#endif // __AIRJOY_REQUEST_DELEGATE_H__
