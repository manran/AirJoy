/*
 * MiniNotifyInfo.h
 *
 * jxfengzi@gmail.com
 *
 *
 */

#ifndef __MINI_NOTIFY_INFO_H__
#define __MINI_NOTIFY_INFO_H__


#include "MiniNotifyTypedef.h"
#include <string>

#ifdef _WIN32
  #include <windows.h>
#else

#endif

namespace miniipc
{

  class MiniNotifyInfo
  {
  public:
    MiniNotifyInfo(const std::string &name);
    ~MiniNotifyInfo();

    const std::string & name() { return m_name; }
    bool send(MiniNotifyResult result);
    bool recv(MiniNotifyResult *result, int second);

  private:
    std::string       m_name;
    MiniNotifyResult  m_result;

#ifdef _WIN32
    HANDLE            m_event;
#else
    pthread_mutex_t   m_mutex;
    pthread_cond_t    m_cond;
#endif // _WIN32

  };




}

#endif // __MINI_NOTIFY_INFO_H__