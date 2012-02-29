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

namespace airjoy
{


  class TcpWorker;
  class AirJoyConfig;

  class AirJoyProcessor
  {
  public:
      AirJoyProcessor();
      AirJoyProcessor(AirJoyConfig *config);
      virtual ~AirJoyProcessor();

      void setAirJoyConfig(AirJoyConfig *config);
      bool start(TcpWorker *pWorker, const char *data);

  private:
      TcpWorker             *m_tcpWorker;
      AirJoyConfig          *m_config;
  };


}


#endif // __AIRJOY_PROCESSOR_H__
