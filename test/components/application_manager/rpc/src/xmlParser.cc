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

#include "rpc/xmlParser.h"

namespace test
{
namespace app_manager_test
{
namespace xml_parser
{

  const char* XML_TEST_NODE_NAME = "test";
  const char* XML_TEST_MESSAGE_REQUEST = "request";
  const char* XML_TEST_MESSAGE_RESPONSE = "response";

  const char* XML_TEST_FUNCTION_PARAM_VECTOR = "Vector";
  const char* XML_TEST_FUNCTION_PARAM_BOOLEAN = "Boolean";
  const char* XML_TEST_FUNCTION_PARAM_STRING = "String";
  const char* XML_TEST_FUNCTION_PARAM_INTEGER = "Integer";
  

  XmlParser::XmlParser():
      sXmlDir()
      ,xmlDir(0)
      ,xmlFile(0)
      ,xmlDoc(0)
      ,sXmlFileName()
      ,xmlTestNode(0)
      ,dirOpened(false)
      ,xmlOpened(false)
      ,xmlFileExists(false)
      ,firstTestNodeFound(false)
      ,params() {
    params.insert(SSMap::value_type("params",jhs::S_PARAMS));
    params.insert(SSMap::value_type("message_params",jhs::S_MSG_PARAMS));
    params.insert(SSMap::value_type("function_id",jhs::S_FUNCTION_ID));
    params.insert(SSMap::value_type("correlation_id",jhs::S_CORRELATION_ID));
    params.insert(SSMap::value_type("protocol_type",jhs::S_PROTOCOL_TYPE));
    params.insert(SSMap::value_type("protocol_version",jhs::S_PROTOCOL_VERSION));
    params.insert(SSMap::value_type("message_type",jhs::S_MESSAGE_TYPE));
  }

  XmlParser::~XmlParser() {
    CloseDir();
    CloseXml();
  }

  XmlParser* XmlParser::instance() {
    static XmlParser instance;
    return &instance;
  }

  void XmlParser::SetXmlDir(const std::string& path) {
    sXmlDir.clear();
    sXmlDir.append(path);
  }   
  
  std::string XmlParser::GetXmlDir() const {
    return sXmlDir;
  }

  bool XmlParser::OpenDir() {
    xmlDir = opendir(sXmlDir.data());
    if (xmlDir) {
      return (dirOpened = true);
    }   
    return (dirOpened = false);
  }

  bool XmlParser::NextXml() {
    if (!dirOpened) return false;
    xmlFile = readdir(xmlDir);
    if (xmlFile) {
      sXmlFileName.clear();
      sXmlFileName.append(xmlFile->d_name);
      return (xmlFileExists = true);
    }
    return (xmlFileExists = false);
  }
  
  bool XmlParser::ReadXml() {
    if (!xmlFileExists) return false;
    
    std::string filePath;
             
    filePath.clear();
    filePath.append(sXmlDir);
    filePath.append("/");
    filePath.append(sXmlFileName);
    
    if (!sXmlFileName.compare(".") || !sXmlFileName.compare("..")) return false;
    xmlDoc = xmlReadFile(filePath.data(),0,0);
    if (xmlDoc) {
      return (xmlOpened = true);
    }
    return (xmlOpened = false);   
  }

  void XmlParser::CloseXml() {
    if (xmlOpened) xmlFreeDoc(xmlDoc);
    xmlOpened = false;
    firstTestNodeFound = false;
    xmlFileExists = false;
  }

  void XmlParser::CloseDir() {
    CloseXml();
    if (dirOpened) closedir(xmlDir);
    dirOpened = false;  
  }

  bool XmlParser::MoveToNextTestNode() {   
    if (!xmlOpened) return false;
    if (!firstTestNodeFound) return (firstTestNodeFound = FindFirstTestNode());
    return NextTestNode();    
  }

  bool XmlParser::GetRequestFromTest(SmartObject& so) {
    if (!xmlOpened) return false;
    xmlNode* xmlTestParams = xmlTestNode->children;
    so = SmartType::SmartType_Null;    
    SetTestAttributes(so);  
    MoveToTag(xmlTestParams);
    if (!xmlTestParams) return false;  
    PutParamsToSO(xmlTestParams,so);  
    return true;
  }
  
  std::string XmlParser::GetXmlFileName() const {
    return sXmlFileName;    
  }

  bool XmlParser::FindFirstTestNode() {
    xmlNode* rootNode = xmlDoc->children;
    return SearchInNodes(rootNode,xmlTestNode);  
  }

  bool XmlParser::SearchInNodes(xmlNode* node,xmlNode*& testNode) {
    bool found = false;
    std::string nodeName;
    while (node) {
      MoveToTag(node);
      if (!node) break; 
      
      found = found || SearchInNodes(node->children,testNode);
      if (found) return found;
    
      nodeName.clear();
      nodeName.append(ToString(node->name));
    
      if (!nodeName.compare(XML_TEST_NODE_NAME)) {
        testNode = node;
        return true;
      }
      node = node->next;
    }
    return false;
  }

  bool XmlParser::NextTestNode() {
    std::string nodeName;
    xmlTestNode = xmlTestNode->next;
    MoveToTag(xmlTestNode);
    
    if (!xmlTestNode) return false;
    do {
      nodeName.clear();
      nodeName.append(ToString(xmlTestNode->name));
      if (nodeName.compare(XML_TEST_NODE_NAME)) {
        xmlTestNode = xmlTestNode->next;
      } else {
        break;
      }
    }
    while (xmlTestNode);
    if (!xmlTestNode) return false;
    return true;
  }


  void XmlParser::SetTestAttributes(SmartObject& so) {
    xmlAttr* xmlTestNodeAttr = xmlTestNode->properties;
    std::string attrName;
    std::string attrValue;
    while (xmlTestNodeAttr) {
      attrName.clear();
      attrName.append(ToString(xmlTestNodeAttr->name));  
      attrValue.clear();
      attrValue.append(ToString(xmlTestNodeAttr->children->content));
      so[attrName] = attrValue;   
      xmlTestNodeAttr = xmlTestNodeAttr->next;
    }  
  }

  void XmlParser::PutParamsToSO(xmlNode* node,SmartObject& so) {
    std::string nodeName;
  
    while (node) {
      MoveToTag(node);
      if (!node) break;
    
      nodeName.clear();
      nodeName.append(ToString(node->name));
      
      bool flag = true;
    
      if (!nodeName.compare(XML_TEST_FUNCTION_PARAM_STRING)) {
        SetStringMsgParam(node->properties,so);
        flag = false;
      }
      if (!nodeName.compare(XML_TEST_FUNCTION_PARAM_INTEGER)) {
        SetIntegerMsgParam(node->properties,so);
        flag = false;
      }
      if (!nodeName.compare(XML_TEST_FUNCTION_PARAM_BOOLEAN)) {
        SetBooleanMsgParam(node->properties,so);
        flag = false;
      }
      if (!nodeName.compare(XML_TEST_FUNCTION_PARAM_VECTOR)) {
        if ((node->properties)&&
            (node->properties->children)&&
            (node->properties->children->content)) {
          std::string vectorName(ToString(node->properties->children->content));
          PutVectorToSO(node->children,so[vectorName]);
        }
        flag = false;
      }
      if (flag) {
        SSMap::iterator elem = params.find(nodeName);
        if(elem!=params.end()) {
          PutParamsToSO(node->children,so[elem->second]);
        } else {        	
          PutParamsToSO(node->children,so[nodeName]);
        }
      }        
      node = node->next;
    }
  }
  
  void XmlParser::PutVectorToSO(xmlNode* node,SmartObject& so) {  
    int index = 0;
    while (node) {
      MoveToTag(node);
      if (!node) break;
    
      std::string nodeName(ToString(node->name)); 
      
      bool flag = true;    
      if (!nodeName.compare(XML_TEST_FUNCTION_PARAM_STRING)) {
        SetStringMsgParam(node->properties,so[index]);
        flag = false;
      }    
      if(flag) PutParamsToSO(node->children,so[index]);    
      index++;
      node = node->next;
    }
  }

  void XmlParser::SetIntegerMsgParam(const xmlAttr* xmlParam,SmartObject& so) {
    if (!xmlParam) return;    
    
    std::string msgParam(ToString(xmlParam->name));
    if ((!xmlParam->children)||(!xmlParam->children->content)) return;
  
    std::string msgValue(ToString(xmlParam->children->content));
    
    if (!VerifyString(msgParam)) return;
    
    so[msgParam] = StrToInt(msgValue);
  }

  void XmlParser::SetBooleanMsgParam(const xmlAttr* xmlParam,SmartObject& so) {
    if (!xmlParam) return;
    
    std::string msgParam(ToString(xmlParam->name));
    if ((!xmlParam->children)||(!xmlParam->children->content)) return;
  
    std::string msgValue(ToString(xmlParam->children->content));    
        
    if (!VerifyString(msgParam)) return;
    
    so[msgParam] = StrToBool(msgValue);
  }

  void XmlParser::SetStringMsgParam(const xmlAttr* xmlParam,SmartObject& so) {   
    if (!xmlParam) return;

    std::string msgParam(ToString(xmlParam->name));
    if ((!xmlParam->children)||(!xmlParam->children->content)) return;

    std::string msgParamValue(ToString(xmlParam->children->content));
    
    if (!VerifyString(msgParam)) return;
    if (!VerifyString(msgParamValue)) return;
      
    SSMap::iterator elem = params.find(msgParam);
    if (elem!=params.end()) {    
      so[elem->second] = msgParamValue;
      return;
    }  
    if (!msgParam.compare("add")) {
      so = msgParamValue;
      return;
    }  
    so[msgParam] = msgParamValue;
  }   

  void XmlParser::MoveToTag(xmlNode*& node) {   
    while (node) {
      if (XML_TEXT_NODE==node->type || XML_COMMENT_NODE==node->type) {
        node = node->next;
      } else {
        break;
      }
    }
  }

  std::string XmlParser::ToString(const xmlChar* str) {
    return std::string((char*)str);
  }
  
  bool XmlParser::VerifyString(const std::string& param) {
  	bool good = true;
  	const char invalidChars[7] = "\\/&\"<>";
  	
  	for (int i=0;i<param.length();i++) {
  	  if (strchr(invalidChars,param[i])) {
  	    good = false;
  		break;
  	  }
  	}  	
  	return good;
  }
  
  int XmlParser::StrToInt(const std::string& param) {
  	bool good = true;  	
  	const char validChars[11] = "0123456789";
  	int val = -1;
  	
  	for (int i=0;i<param.length();i++) {
  	  if (!strchr(validChars,param[i])) {
  		good = false;
  		break;
  	  }
  	}  	
  	if (good) val = atoi(param.data());  	    
  	
  	return val;
  }
  
  bool XmlParser::StrToBool(const std::string& param) {
  	bool good = false;
  	std::string toCheck;
  	toCheck.clear();
  	toCheck.append(param);
  	std::transform(toCheck.begin(),toCheck.end(),toCheck.begin(),::tolower);
  	
  	if (!toCheck.compare("true")) good = true;
  	return good;
  }

}//namespace xml_parser
}//namespace app_manager_test
}//namespace test
