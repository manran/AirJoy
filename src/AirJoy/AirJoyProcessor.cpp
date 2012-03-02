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
#include "TcpWorker.h"
#include "AirJoyDispatcher.h"

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
  m_tcpWorker   = NULL;
  m_config      = NULL;
  m_dispatcher  = NULL;
}

AirJoyProcessor::~AirJoyProcessor()
{
  if (m_dispatcher)
    delete m_dispatcher;
}

AirJoyProcessor::AirJoyProcessor(AirJoyConfig *config)
{
  this->setAirJoyConfig(config);
}

void AirJoyProcessor::setAirJoyConfig(AirJoyConfig *config)
{
  m_config = config;
}

bool AirJoyProcessor::start(TcpWorker *pWorker, const char *data, int length)
{
  m_tcpWorker = pWorker;

  // load xml message
  AirJoyMessage request;
  if (! request.loadText(data, length))
  {
    this->notSupportForRequest();
    return false;
  }

  // dispatch xml message
  if (m_dispatcher == NULL)
    m_dispatcher = new AirJoyDispatcher;

  if (m_dispatcher->didReceive(this, request))
  {
    // server close socket!
    return false;
  }

  // not support message
  this->notSupportForRequest(request);
  return false;
}

bool AirJoyProcessor::send(AirJoyMessage &message)
{
  if (m_tcpWorker == NULL)
    return false;

  const char *text = message.toText();
  m_tcpWorker->send(text, strlen(text));

  return true;
}

bool AirJoyProcessor::responseForRequest(AirJoyMessage &request, AirJoyMessageType type, const std::string &result)
{
  AirJoyMessage response(type, request.id(), request.to(), request.from());
  response.setAppResult(result);

  return this->send(response);
}

//---------------------------------------------------------------------------------------------
// Private API
//---------------------------------------------------------------------------------------------

bool AirJoyProcessor::notSupportForRequest(AirJoyMessage &request)
{
  std::string result("not support");
  return responseForRequest(request, AirJoyMessageTypeError, result);
}

bool AirJoyProcessor::notSupportForRequest(void)
{
  std::string result("not support");
  
  AirJoyMessage response(AirJoyMessageTypeError, "0", "AirJoy", "unknown");
  response.setAppResult(result);

  return this->send(response);
}
