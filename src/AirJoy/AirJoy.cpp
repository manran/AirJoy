/*
 * AirJoy.cpp
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-2-29
 *
 */

#include "AirJoy.h"
#include "TcpMaster.h"
#include "AirJoyProcessor.h"


using namespace airjoy;

//---------------------------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------------------------

AirJoy::AirJoy()
{
  m_tcpMaster = new TcpMaster;
  m_tcpMaster->setRequestHandler(this);

  m_processor = NULL;
}

AirJoy::~AirJoy()
{
  if (m_processor)
    delete m_processor;

  delete m_tcpMaster;
}

bool AirJoy::start(void)
{
  return m_tcpMaster->start();
}

void AirJoy::stop(void)
{
  m_tcpMaster->stop();
}

bool AirJoy::isRunning(void)
{
  return m_tcpMaster->isRunning();
}

int AirJoy::numberOfConnections(void)
{
  return m_tcpMaster->numberOfTcpConnections();
}

void AirJoy::setProcessor(AirJoyProcessor *processor)
{
  m_processor = processor;
  m_processor->setAirJoyConfig(this);
}

//---------------------------------------------------------------------------------------------
// Virtual API - HttpConfig
//---------------------------------------------------------------------------------------------
int AirJoy::port(void)
{
  return m_tcpMaster->port();
}

void AirJoy::setPort(int port)
{
  m_tcpMaster->setPort(port);
}

//---------------------------------------------------------------------------------------------
// Virtual API - TcpRequestHandler
//---------------------------------------------------------------------------------------------
bool AirJoy::handleRequest(TcpWorker *pWorker, const char *data, int length)
{
  if (m_processor == NULL)
    m_processor = new AirJoyProcessor(this);

  return m_processor->start(pWorker, data);
}