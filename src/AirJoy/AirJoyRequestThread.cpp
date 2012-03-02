/* 
 * AirJoyRequestThread.cpp
 *
 * jxfengzi@gmail.com
 * 2012-2-28
 *
 */

#include "AirJoyRequestThread.h"
#include "AirJoyRequest.h"
#include "AirJoyCommon.h"

using namespace airjoy;

AirJoyRequestThread::AirJoyRequestThread(AirJoyRequest *request)
      : m_request(request)
{
  m_miniNotify = new miniipc::MiniNotify();
  m_queueMaxSize = 1024;
}

AirJoyRequestThread::~AirJoyRequestThread()
{
  notifyThreadWillExit();

  if (! waitForThreadToExit(100))
  {
    stopThread(1);
  }

  delete m_miniNotify;
}

void AirJoyRequestThread::run()
{
  while (! threadShouldExit()) 
  {
    MiniNotifyResult result = MiniNotifyResultNull;
    if (m_miniNotify->recv("RequestQ", &result, 0))
    {
      if (result != MiniNotifyResultOk)
        break;

      // 读消息队列，发送请求
      
      while (1)
      {
        AirRequestInfo *info = NULL;
        {
          ScopedLock lock(m_criticalSection);
          
          if (m_requestQ.size() == 0)
            break;

          info = m_requestQ.front();
          m_requestQ.pop();
        }

        m_request->sendRequestAndWaitResponse(info->ip, info->port, info->message, info->sessionId);

        delete info->message;
        delete info;
      }
    }
    else
    {
      airjoy::Util::airJoySleep(1);
    }
  }

  // 删除队列里的元素
  ScopedLock lock(m_criticalSection);
  while (m_requestQ.size() > 0)
  {
    m_requestQ.pop();
  }
}

AirJoySessionId AirJoyRequestThread::genNewSessionId(void)
{
  static AirJoySessionId sessionId = 10000;

  sessionId ++;

  return sessionId;
}

AirJoySessionId AirJoyRequestThread::pushToQueue(const std::string &ip, int port, AirJoyMessage *message)
{
  ScopedLock lock(m_criticalSection);

  if (m_requestQ.size() > m_queueMaxSize)
    return 0;

  AirJoySessionId thisSessionId = genNewSessionId();

  // 将sessionid 转为id
  message->setId(thisSessionId);

  // 在弹出队列时释放
  AirRequestInfo *info = new AirRequestInfo;
  info->ip        = ip;
  info->port      = port;
  info->sessionId = thisSessionId;
  info->message   = message;

  m_requestQ.push(info); 

  // 发送通知，让发送线程发送请求
  m_miniNotify->send("RequestQ", MiniNotifyResultOk);

  return thisSessionId;
}

void AirJoyRequestThread::notifyThreadWillExit(void)
{
  // 发送通知，让发送线程停止
  m_miniNotify->send("RequestQ", MiniNotifyResultError);
}