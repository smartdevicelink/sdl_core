/**
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_XML_PARSER_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_XML_PARSER_H_

#include <string>
#include <dirent.h>
#include <map>
#include <algorithm>
#include <cstring>

#include "smart_objects/smart_object.h"
#include "libxml/xmlreader.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "formatters/formatter_json_rpc.h"

using namespace NsSmartDeviceLink::NsJSONHandler::strings;
using namespace NsSmartDeviceLink::NsSmartObjects;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;
using namespace std;

namespace test {
namespace app_manager_test {
namespace xml_parser {

  /**
  *@brief Typedef for string params
  */
  typedef std::map<std::string,std::string> SSMap;
  
  /**
  *@brief Singleton class for parsing XML-file with requests 
  */
  class XmlParser
  {
   public:
    static XmlParser* instance();  

    /**
    *@brief Sets current dir path with XML-files 
    *@param std::string pith path
    */
    void SetXmlDir(const std::string& path);
    
    /**
    *@brief Gets current dir path with XML-files
    *@return returns std::string with current path 
    */
    std::string GetXmlDir() const;
    
    /**
    *@brief Gets current xml-file name
    *@return returns std::string with current xml-file name 
    */
    std::string GetXmlFileName() const;    
    
    /**
    *@brief Opens a dir with xml-files
    *@return true if succesfull, otherwise false 
    */
    bool OpenDir();
    
    /**
    *@brief Finds first xml-file in dir and then next xml-files
    *@return true if succesfull, otherwise false 
    */
    bool NextXml();
    
    /**
    *@brief Reads current xml-file
    *@return true if succesfull, otherwise false 
    */
    bool ReadXml();   
    
    /**
    *@brief Closes current xml-file     
    */
    void CloseXml();

    /**
    *@brief Closes current dir with xml-files     
    */
    void CloseDir();
    
    /**
    *@brief Finds first and then next test nodes
    *@return true if succesfull
    */
    bool MoveToNextTestNode();
    
    /**
    *@brief Fill smartobject with test params
    *@param SmartObject to fill
    *@return true if succesfull
    */
    bool GetRequestFromTest(SmartObject& so);
    
   private:
    XmlParser();
    ~XmlParser();
    
   private:  
    
    /**
    *@brief Finds first test node in xml-file
    *@return true if succesfull
    */
    bool FindFirstTestNode();
    
    /**
    *@brief Recursive function that finds test nodes
    *@param xmlNode* to find in
    *@param xmlNode*& found test node
    *@return true if succesfull
    */
    bool SearchInNodes(xmlNode* node,xmlNode*& testNode);
    
    /**
    *@brief Moves to next test node
    *@return true if succesfull
    */
    bool NextTestNode();
    
    /**
    *@brief Reads test attributes and puts them into SmartObject
    *@param SmartObject& to fill    
    */
    void SetTestAttributes(SmartObject& so);  
  
    /**
    *@brief Recursively fills smartobject with params in test node
    *@param xmlNode* to start with
    *@param SmartObject& to fill    
    */
    void PutParamsToSO(xmlNode* node,SmartObject& so);
    
    /**
    *@brief Recursively puts  vector param to smartobject
    *@param xmlNode* vector node
    *@param SmartObject& to fill    
    */
    void PutVectorToSO(xmlNode* node,SmartObject& so);  
    
    /**
    *@brief Puts an integer param in smartobject
    *@param xmlAttr* pointer to parameter
    *@param SmartObject& to put in    
    */
    void SetIntegerMsgParam(const xmlAttr* xmlParam,SmartObject& so);
    
    /**
    *@brief Puts a boolean parameter in smartobject
    *@param xmlAttr* pointer to parameter
    *@param SmartObject& to put in    
    */
    void SetBooleanMsgParam(const xmlAttr* xmlParam,SmartObject& so);
    
    /**
    *@brief Puts a string parameter in smartobject
    *@param xmlAttr* pointer to parameter
    *@param SmartObject& to put in    
    */
    void SetStringMsgParam(const xmlAttr* xmlParam,SmartObject& so);
  
    /**
    *@brief just moves to next valid tag (not text tag or comment tag)
    *@param xmlNode*& node to start from    
    */
    void MoveToTag(xmlNode*& node);
     
    /**
    *@brief converts a xmlChar string to std::string
    *@param xmlChar* string    
    *@return std::string with converted xmlChar string
    */
    std::string ToString(const xmlChar* str);
    
    /**
    *@brief veifies if a string has no invalid chars
    *@param std::string& to check   
    *@return true if a string has no invalid chars, otherwise false
    */
    bool VerifyString(const std::string& param);
    
    /**
    *@brief checks if a string has only number chars and converts it to integer
    *@param std::string& to check and convert   
    *@return converted value if a string has been converted, otherwise -1
    */
    int StrToInt(const std::string& param);
    
    /**
    *@brief checks if a string consists only of "true" or "false"
    *@param std::string& to check and convert   
    *@return if string consists of "true", returns true, other occations - false
    */
    bool StrToBool(const std::string& param);
    
    

    std::string sXmlDir;
    DIR* xmlDir;
    dirent* xmlFile;
    std::string sXmlFileName;
    xmlDocPtr xmlDoc;
    xmlNode* xmlTestNode;
    
    bool dirOpened;
    bool xmlOpened;
    bool xmlFileExists;
    bool firstTestNodeFound;

    SSMap params;
  };
}//xml_parser
}//app_manager_test
}//test



#endif //TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_XML_PARSER_H_


