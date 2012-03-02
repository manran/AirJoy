/*
* MiniNotifyInfo.cpp
*
* jxfengzi@gmail.com
*
*
*/

#include "MiniNotifyInfo.h"

#ifdef _WIN32
#else
#include <time.h>
#endif

//#include <iostream>

using namespace miniipc;

MiniNotifyInfo::MiniNotifyInfo(const std::string &name)
: m_name(name),
m_result(MiniNotifyResultNull)
{
  //std::cout << "new MiniNotifyInfo: name: " << name << std::endl;

#ifdef _WIN32 
  m_event = CreateEvent (NULL, TRUE, FALSE, m_name.c_str());
#else
  pthread_mutex_init(&m_mutex, NULL);
  pthread_cond_init(&m_cond, NULL);
#endif

}

MiniNotifyInfo::~MiniNotifyInfo()
{
#ifdef _WIN32
  CloseHandle(m_event);
#else
  pthread_mutex_destroy(&m_mutex);
  pthread_cond_destroy(&m_cond);
#endif
}

bool MiniNotifyInfo::send(MiniNotifyResult result)
{
  //std::cout << "MiniNotifyInfo::send: " << result << std::endl;

#ifdef _WIN32
  DWORD ret = 0;
  m_result = result;
  ret = SetEvent(m_event);
#else
  pthread_mutex_lock(&m_mutex);
  m_result = result;
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);
#endif

  return true;
}

bool MiniNotifyInfo::recv(MiniNotifyResult *result, int second)
{
  //std::cout << "MiniNotifyInfo::recv" << std::endl;

#ifdef _WIN32
  DWORD ret = 0;
  DWORD dwMilliseconds = 0;
  dwMilliseconds = (second == 0) ? INFINITE : (second * 1000);
  ret = WaitForSingleObject(m_event, dwMilliseconds);
  if (ret == WAIT_OBJECT_0)
  {
    *result = m_result;
    ResetEvent(m_event);
    return true;
  }
#else
  int ret = 0;
  struct timespec ts;

  ts.tv_sec   = time(0);
  ts.tv_sec  += (second == 0) ? 3600 : second ;
  ts.tv_nsec  = 0;

  pthread_mutex_lock(&m_mutex);
  ret = pthread_cond_timedwait(&m_cond, &m_mutex, &ts);
  if (ret == 0)
  {
    *result = m_result;
    pthread_mutex_unlock(&m_mutex);
    return true;
  }
  pthread_mutex_unlock(&m_mutex);
#endif

  return false;
}