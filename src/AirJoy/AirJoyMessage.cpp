/*
 * AirJoyMessage.cpp
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-2-29
 *
 */

#include "AirJoyMessage.h"
#include <string.h>

using namespace rapidxml;
using namespace airjoy;

//---------------------------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------------------------

AirJoyMessage::AirJoyMessage()
{
  
  /*
	   <iq type="get" id="001" from="fox" to="alex">
	     <query xmlns="http://www.airjoy.cn/query">
	       <application xmlns="http://www.airjoy.cn/query/getsharedfolder">
	         <param>$PARAM$</param>
	       </application>
	     </query>
	   </iq>
  */

  m_iq = m_doc.allocate_node(node_element, "iq");
  m_iqQuery = m_doc.allocate_node(node_element, "query");
  m_iqQueryApp = m_doc.allocate_node(node_element, "application");
  m_iqQueryAppParam = m_doc.allocate_node(node_element, "param");
  m_iqQueryAppResult = m_doc.allocate_node(node_element, "result");
}

AirJoyMessage::AirJoyMessage(AirJoyMessageType type, 
                             const std::string &id, 
                             const std::string &from, 
                             const std::string &to)
{
  m_iq = m_doc.allocate_node(node_element, "iq");
  m_iqQuery = m_doc.allocate_node(node_element, "query");
  m_iqQueryApp = m_doc.allocate_node(node_element, "application");
  m_iqQueryAppParam = m_doc.allocate_node(node_element, "param");
  m_iqQueryAppResult = m_doc.allocate_node(node_element, "result");

  m_type = type;
  m_id = id;
  m_from = from;
  m_to = to;
}

AirJoyMessage::~AirJoyMessage()
{
}

bool AirJoyMessage::loadText(char *data)
{
  xml_document<> doc;
  doc.parse<parse_default>(data);

  // <iq>
  xml_node<> *iq = doc.first_node();
  if (iq == NULL)
    return false;

  if (strcmp(iq->name(), "iq") != 0)
    return false;

  xml_attribute<char> * iqType = iq->first_attribute("type");
  xml_attribute<char> * iqId = iq->first_attribute("id");
  xml_attribute<char> * iqFrom = iq->first_attribute("from");
  xml_attribute<char> * iqTo = iq->first_attribute("to");

  if (iqType == NULL)
    return false;

  if (iqId == NULL)
    return false;

  if (iqFrom == NULL)
    return false;

  if (iqTo == NULL)
    return false;

  m_type = textToMessageType(iqType->value());
  m_id = iqId->value();
  m_from = iqFrom->value();
  m_to = iqTo->value();

  // <query>
  xml_node<>* query = iq->first_node("query");
  if (query == NULL)
    return false;

  xml_attribute<char> * queryXmlns = iq->first_attribute("xmlns");
  if (queryXmlns == NULL)
    return false;

  m_xmlns = queryXmlns->value();

  // <application>
  xml_node<>* app = query->first_node("application");
  if (app == NULL)
    return false;

  xml_attribute<char> * appXmlns = app->first_attribute("xmlns");
  if (appXmlns == NULL)
    return false;

  m_appXmlns = appXmlns->value();

  // <param> or <result>
  xml_node<>* appParam = app->first_node("param");
  if (appParam)
    m_appParam = appParam->value();
  else
  {
    xml_node<>* appResult = app->first_node("result");
    if (appResult)
      m_appResult = appResult->value();
  }

  return true;
}

const char * AirJoyMessage::toText(void)
{
  m_iq->remove_all_attributes();
  m_iq->append_attribute(m_doc.allocate_attribute("type", messageTypeToText(m_type)));
  m_iq->append_attribute(m_doc.allocate_attribute("id", m_id.c_str()));
  m_iq->append_attribute(m_doc.allocate_attribute("from", m_from.c_str()));
  m_iq->append_attribute(m_doc.allocate_attribute("to", m_to.c_str()));

  m_iqQuery->remove_all_attributes();
  m_iqQuery->append_attribute(m_doc.allocate_attribute("xmlns", m_xmlns.c_str()));

  m_iqQueryApp->append_attribute(m_doc.allocate_attribute("xmlns", m_appXmlns.c_str()));

  m_doc.append_node(m_iq);
  m_iq->append_node(m_iqQuery);
  m_iqQuery->append_node(m_iqQueryApp);

  if (m_type == AirJoyMessageTypeResult || m_type == AirJoyMessageTypeError)
  {
    m_iqQueryApp->append_node(m_iqQueryAppResult);
    m_iqQueryAppResult->value(m_appResult.c_str());  
  }
  else
  {
    m_iqQueryApp->append_node(m_iqQueryAppParam);
    m_iqQueryAppParam->value(m_appParam.c_str());
  }

  m_buffer.clear();
  print(std::back_inserter(m_buffer), m_doc, 0);

  return m_buffer.c_str();
}

//---------------------------------------------------------------------------------------------
// Private API
//---------------------------------------------------------------------------------------------

AirJoyMessageType AirJoyMessage::textToMessageType(const char *text)
{
  if (strcmp(text, "get") == 0)
    return AirJoyMessageTypeGet;

  if (strcmp(text, "set") == 0)
    return AirJoyMessageTypeSet;
  
  if (strcmp(text, "result") == 0)
    return AirJoyMessageTypeResult;

  if (strcmp(text, "error") == 0)
    return AirJoyMessageTypeError;

  return AirJoyMessageTypeInvalid;
}

const char * AirJoyMessage::messageTypeToText(AirJoyMessageType type)
{
  switch (type)
  {
  case AirJoyMessageTypeGet:
    return "get";

  case AirJoyMessageTypeSet:
    return "set";

  case AirJoyMessageTypeResult:
    return "result";

  case AirJoyMessageTypeError:
    return "error";

  default:
    return NULL;
  }

  return NULL;
}
