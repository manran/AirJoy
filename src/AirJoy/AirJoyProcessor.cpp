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
#include "AirJoyProcessorDelegate.h"

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
  m_delegate    = NULL;

  this->setAirJoyConfig(NULL);
}

AirJoyProcessor::~AirJoyProcessor()
{
  if (m_delegate)
    delete m_delegate;
}

AirJoyProcessor::AirJoyProcessor(AirJoyConfig *config)
{
  m_tcpWorker   = NULL;
  m_delegate    = NULL;

  this->setAirJoyConfig(config);
}

void AirJoyProcessor::setDelegate(AirJoyProcessorDelegate *processorDelegate)
{
  if (m_delegate)
    delete m_delegate;

  m_delegate = processorDelegate;
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
  if (m_delegate)
  {
    if (m_delegate->didReceiveRequest(this, request))
    {
      // server close socket!
      return false;
    }
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
  response.setXmlns(request.xmlns());
  response.setAppXmlns(request.appXmlns());
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
