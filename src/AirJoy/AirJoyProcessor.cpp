/*
 * AirJoyProcessor.cpp
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-2-29
 *
 */

#include "AirJoyProcessor.h"
#include "AirJoyMessage.h"
#include "TcpWorker.h"

#include <stdlib.h>

#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

using namespace airjoy;

//---------------------------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------------------------

AirJoyProcessor::AirJoyProcessor()
{
  m_tcpWorker = NULL;
  m_config    = NULL;
}

AirJoyProcessor::~AirJoyProcessor()
{
}

AirJoyProcessor::AirJoyProcessor(AirJoyConfig *config)
{
  this->setAirJoyConfig(config);
}

void AirJoyProcessor::setAirJoyConfig(AirJoyConfig *config)
{
  m_config = config;
}

bool AirJoyProcessor::start(TcpWorker *pWorker, const char *data)
{
  m_tcpWorker = pWorker;

  AirJoyMessage message(AirJoyMessageTypeError, "001", "w500", "ouyang");
  message.setXmlns("http://www.airjoy.cn/query");
  message.setAppXmlns("http://www.airjoy.cn/query/getsharedfolder");
  message.setAppResult("hahaha");

  const char *text = message.toText();
  
  pWorker->send(text, strlen(text));

  return false;
}


//---------------------------------------------------------------------------------------------
// Private API
//---------------------------------------------------------------------------------------------


