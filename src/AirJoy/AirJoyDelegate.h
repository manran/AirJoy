/*
 * AirJoyDelegate.h
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-3-2
 *
 */

#ifndef __AIRJOY_DELEGATE_H__
#define __AIRJOY_DELEGATE_H__

namespace airjoy
{

  class AirJoyDelegate
  {
  public:
    AirJoyDelegate() {}
    virtual ~AirJoyDelegate() {}

    virtual void willStart(void) {}
    virtual void didStart(void) {}
    virtual void didNotStart(void) {}

    virtual void willStop(void) {}
    virtual void didStop(void) {}
  };


}

#endif // __AIRJOY_DELEGATE_H_
