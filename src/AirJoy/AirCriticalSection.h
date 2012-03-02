/*
 * AirCriticalSection.h
 *
 *
 * 
 *
 * copy from Zeroconf
 *
 */

#ifndef __AirCriticalSection_H__
#define __AirCriticalSection_H__

#ifndef WIN32
#include <pthread.h>
#endif

namespace airjoy
{

  class CriticalSection
  {
  public:
    CriticalSection();
    ~CriticalSection();

    void enter() const;
    bool tryEnter() const;
    void exit() const;

  private:
#ifdef WIN32
    char details[44];
#else
    mutable pthread_mutex_t details;
#endif
  };

  class ScopedLock
  {
  public:
    inline ScopedLock (const CriticalSection& lock) 
      : mLock(lock) 
    {
      mLock.enter(); 
    }
    inline ~ScopedLock() 
    {
      mLock.exit(); 
    }
  private:
    const CriticalSection& mLock;
  };

}

#endif // __AirCriticalSection_H__
