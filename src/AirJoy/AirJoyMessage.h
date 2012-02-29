/*
 * AirJoyMessage.h
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-2-29
 *
 */

#ifndef __AIRJOY_MESSAGE_H__
#define __AIRJOY_MESSAGE_H_

#include <string>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

namespace airjoy
{

	/*
	 *
	   <iq type="get" id="001" from="fox" to="alex">
	     <query xmlns="http://www.airjoy.cn/query">
	       <application xmlns="http://www.airjoy.cn/query/getsharedfolder">
	         <param>$PARAM$</param>
	       </application>
	     </query>
	   </iq>

	   <iq type="result" id="001" from="fox" to="alex">
	     <query xmlns="http://www.airjoy.cn/query">
	       <application xmlns="http://www.airjoy.cn/query/getsharedfolder">
	         <result>$RESULT$</result>
	       </application>
	     </query>
	   </iq>

	   <iq type="error" id="001" from="fox" to="alex">
	     <query xmlns="http://www.airjoy.cn/query">
	       <application xmlns="http://www.airjoy.cn/query/getsharedfolder">
	         <result>$RESULT$</result>
	       </application>
	     </query>
	   </iq>
	 */

  typedef enum _AirJoyMessageType
  {
    AirJoyMessageTypeGet     = 0,
    AirJoyMessageTypeSet     = 1,
    AirJoyMessageTypeResult  = 2,
    AirJoyMessageTypeError   = 3,
    AirJoyMessageTypeInvalid,
  } AirJoyMessageType;

  class AirJoyMessage
  {
  public:
    AirJoyMessage();
    AirJoyMessage(AirJoyMessageType type, 
                  const std::string &id, 
                  const std::string &from, 
                  const std::string &to);
    virtual ~AirJoyMessage();

    void setFrom(const std::string &from) { m_from = from; }
    const std::string & from(void) {return m_from; }

    void setTo(const std::string &to) { m_to = to; }
    const std::string & to(void) {return m_to; }

    void setId(const std::string &id) { m_id = id; }
    const std::string & id(void) { return m_id; } 

    void setXmlns(const std::string &xmlns) { m_xmlns = xmlns; }
    const std::string & xmlns(void) { return m_xmlns; } 

    void setAppXmlns(const std::string &xmlns) { m_appXmlns = xmlns; }
    const std::string & appXmlns(void) { return m_appXmlns; } 

    void setAppParam(const std::string &param) { m_appParam = param; }
    const std::string & appParam(void) { return m_appParam; } 

    void setAppResult(const std::string &result) { m_appResult = result; }
    const std::string & appResult(void) { return m_appResult; } 

    void setType(AirJoyMessageType type) { m_type = type; }
    AirJoyMessageType type(void) { return m_type; }

    bool loadText(char *data);
    const char * toText(void);
  
  private:
    static AirJoyMessageType textToMessageType(const char *text);
    static const char * messageTypeToText(AirJoyMessageType type);

  private:
    std::string         m_from;
    std::string         m_to;
    std::string         m_id;
    std::string         m_xmlns;
    std::string         m_appXmlns;
    std::string         m_appParam;
    std::string         m_appResult;
    AirJoyMessageType   m_type;

  private:
    rapidxml::xml_document<>  m_doc;
    rapidxml::xml_node<>     *m_iq;
    rapidxml::xml_node<>     *m_iqQuery;
    rapidxml::xml_node<>     *m_iqQueryApp;
    rapidxml::xml_node<>     *m_iqQueryAppParam;
    rapidxml::xml_node<>     *m_iqQueryAppResult;
    std::string               m_buffer;
  };

}

#endif // __AIRJOY_MESSAGE_H__
