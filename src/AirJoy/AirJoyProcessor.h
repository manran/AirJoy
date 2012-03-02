/*
 * AirJoyProcessor.h
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-2-29
 *
 */

#ifndef __AIRJOY_PROCESSOR_H__
#define __AIRJOY_PROCESSOR_H__

#include "AirJoyMessage.h"

namespace airjoy
{

  class TcpWorker;
  class AirJoyConfig;
  class AirJoyDispatcher;

  class AirJoyProcessor
  {
  public:
      AirJoyProcessor();
      AirJoyProcessor(AirJoyConfig *config);
      virtual ~AirJoyProcessor();

      void setAirJoyConfig(AirJoyConfig *config);
      bool start(TcpWorker *pWorker, const char *data, int length);

      bool send(AirJoyMessage &message);
      bool responseForRequest(AirJoyMessage &request, AirJoyMessageType type, const std::string &result);

  private:
      bool notSupportForRequest(AirJoyMessage &request);
      bool notSupportForRequest(void);

  private:
      TcpWorker             *m_tcpWorker;
      AirJoyConfig          *m_config;
      AirJoyDispatcher      *m_dispatcher;
  };


}


#endif // __AIRJOY_PROCESSOR_H__
