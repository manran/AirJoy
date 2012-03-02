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

      // ����Ϣ���У���������
      
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

  // ɾ���������Ԫ��
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

  // ��sessionid תΪid
  message->setId(thisSessionId);

  // �ڵ�������ʱ�ͷ�
  AirRequestInfo *info = new AirRequestInfo;
  info->ip        = ip;
  info->port      = port;
  info->sessionId = thisSessionId;
  info->message   = message;

  m_requestQ.push(info); 

  // ����֪ͨ���÷����̷߳�������
  m_miniNotify->send("RequestQ", MiniNotifyResultOk);

  return thisSessionId;
}

void AirJoyRequestThread::notifyThreadWillExit(void)
{
  // ����֪ͨ���÷����߳�ֹͣ
  m_miniNotify->send("RequestQ", MiniNotifyResultError);
}