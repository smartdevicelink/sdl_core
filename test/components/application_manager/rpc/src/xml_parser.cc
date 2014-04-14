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

#include "rpc/xml_parser.h"

namespace test {

  const char* XML_TEST_NODE_NAME = "test";
  const char* XML_TEST_MESSAGE_REQUEST  = "request";
  const char* XML_TEST_MESSAGE_RESPONSE = "response";

  const char* XML_TEST_FUNCTION_PARAM_VECTOR  = "Vector";
  const char* XML_TEST_FUNCTION_PARAM_BOOLEAN = "Boolean";
  const char* XML_TEST_FUNCTION_PARAM_STRING  = "String";
  const char* XML_TEST_FUNCTION_PARAM_INTEGER = "Integer";
  const char* XML_TEST_FUNCTION_PARAM_BINARY  = "Binary";
  const char* XML_TEST_FUNCTION_PARAM_GARBAGE = "Garbage";
  const char* XML_TEST_FUNCTION_PARAM_PATCH   = "Patch";
  const char* XML_TEST_FUNCTION_PARAM_ARRAY   = "Array";
  const char* XML_TEST_FUNCTION_PARAM_FLOAT   = "Float";

  XmlParser::XmlParser()
    : s_xml_dir_(),
      xml_dir_(0),
      xml_file_(0),
      xml_doc_(0),
      s_xml_file_name_(),
      xml_test_node_(0),
      dir_opened_(false),
      xml_opened_(false),
      xml_file_exists_(false),
      first_test_node_found_(false),
      params_() {
    params_.insert(SSMap::value_type("params", jhs::S_PARAMS));
    params_.insert(SSMap::value_type("message_params", jhs::S_MSG_PARAMS));
    params_.insert(SSMap::value_type("function_id", jhs::S_FUNCTION_ID));
    params_.insert(SSMap::value_type("correlation_id", jhs::S_CORRELATION_ID));
    params_.insert(SSMap::value_type("protocol_type", jhs::S_PROTOCOL_TYPE));
    params_.insert(SSMap::value_type("protocol_version",
                                     jhs::S_PROTOCOL_VERSION));
    params_.insert(SSMap::value_type("message_type", jhs::S_MESSAGE_TYPE));
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
    s_xml_dir_.clear();
    s_xml_dir_.append(path);
  }

  std::string XmlParser::GetXmlDir() const {
    return s_xml_dir_;
  }

  bool XmlParser::OpenDir() {
    xml_dir_ = opendir(s_xml_dir_.data());
    if (xml_dir_) {
      return (dir_opened_ = true);
    }
    return (dir_opened_ = false);
  }

  bool XmlParser::NextXml() {
    if (!dir_opened_) return false;
    xml_file_ = readdir(xml_dir_);
    if (xml_file_) {
      s_xml_file_name_.clear();
      s_xml_file_name_.append(xml_file_->d_name);
      return (xml_file_exists_ = true);
    }
    return (xml_file_exists_ = false);
  }

  bool XmlParser::ReadXml() {
    if (!xml_file_exists_) return false;

    std::string filePath;

    filePath.append(s_xml_dir_);
    filePath.append("/");
    filePath.append(s_xml_file_name_);

    if (!s_xml_file_name_.compare(".") || !s_xml_file_name_.compare("..")) {
      return false;
    }
    if (xml_doc_) {
      xmlFreeDoc(xml_doc_);
      first_test_node_found_ = false;
      xml_opened_ = false;
    }
    xml_doc_ = xmlReadFile(filePath.data(), 0, 0);
    if (xml_doc_) {
      return (xml_opened_ = true);
    }
    return (xml_opened_ = false);
  }

  void XmlParser::CloseXml() {
    if (xml_opened_) xmlFreeDoc(xml_doc_);
    xml_opened_ = false;
    first_test_node_found_ = false;
    xml_file_exists_ = false;
  }

  void XmlParser::CloseDir() {
    CloseXml();
    if (dir_opened_) closedir(xml_dir_);
    dir_opened_ = false;
  }

  bool XmlParser::MoveToNextTestNode() {
    if (!xml_opened_) return false;
    if (!first_test_node_found_) {
      return (first_test_node_found_ = FindFirstTestNode());
    }
    return NextTestNode();
  }

  bool XmlParser::GetRequestFromTest(smart::SmartObject* so) {
    if (!xml_opened_) return false;
    xmlNode* xml_test_params = xml_test_node_->children;
    *so = smart::SmartType::SmartType_Null;
    SetTestAttributes(so);
    MoveToTag(&xml_test_params);
    if (!xml_test_params) return false;
    PutParamsToSO(xml_test_params, so);
    return true;
  }

  std::string XmlParser::GetXmlFileName() const {
    return s_xml_file_name_;
  }

  bool XmlParser::FindFirstTestNode() {
    xmlNode* rootNode = xml_doc_->children;
    return SearchInNodes(rootNode, &xml_test_node_);
  }

  bool XmlParser::SearchInNodes(xmlNode* node, xmlNode** testNode) {
    bool found = false;
    std::string nodeName;
    while (node) {
      MoveToTag(&node);
      if (!node) break;

      found = found || SearchInNodes(node->children, testNode);
      if (found) return found;

      nodeName.clear();
      nodeName.append(ToString(node->name));

      if (!nodeName.compare(XML_TEST_NODE_NAME)) {
        *testNode = node;
        return true;
      }
      node = node->next;
    }
    return false;
  }

  bool XmlParser::NextTestNode() {
    std::string nodeName;
    xml_test_node_ = xml_test_node_->next;
    MoveToTag(&xml_test_node_);

    if (!xml_test_node_) return false;
    do {
      nodeName.append(ToString(xml_test_node_->name));
      if (nodeName.compare(XML_TEST_NODE_NAME)) {
        xml_test_node_ = xml_test_node_->next;
      } else {
        break;
      }
    } while (xml_test_node_);
    if (!xml_test_node_) return false;
    return true;
  }


  void XmlParser::SetTestAttributes(smart::SmartObject* so) {
    xmlAttr* xml_test_node_attr = xml_test_node_->properties;

    while (xml_test_node_attr) {
      std::string attrName;
      std::string attrValue;

      attrName.append(ToString(xml_test_node_attr->name));
      attrValue.append(ToString(xml_test_node_attr->children->content));
      (*so)[attrName] = attrValue;
      xml_test_node_attr = xml_test_node_attr->next;
    }
  }

  void XmlParser::PutParamsToSO(xmlNode* node, smart::SmartObject* so) {
    std::string node_name;

    while (node) {
      MoveToTag(&node);
      if (!node) break;

      node_name.clear();
      node_name.append(ToString(node->name));
      bool flag = true;

      if (!node_name.compare(XML_TEST_FUNCTION_PARAM_STRING)) {
        SetStringMsgParam(node->properties, so);
        flag = false;
      }
      if (!node_name.compare(XML_TEST_FUNCTION_PARAM_INTEGER)) {
        SetIntegerMsgParam(node->properties, so);
        flag = false;
      }
      if (!node_name.compare(XML_TEST_FUNCTION_PARAM_BOOLEAN)) {
        SetBooleanMsgParam(node->properties, so);
        flag = false;
      }
      if (!node_name.compare(XML_TEST_FUNCTION_PARAM_VECTOR)) {
        if ((node->properties)&&
            (node->properties->children)&&
            (node->properties->children->content)) {
          std::string vector_name(
              ToString(node->properties->children->content));
          PutVectorToSO(node->children, &(*so)[vector_name]);
        }
        flag = false;
      }
      if (!node_name.compare(XML_TEST_FUNCTION_PARAM_BINARY)) {
        SetBinaryMsgParam(node->properties, so);
        flag = false;
      }
      if (!node_name.compare("result")) {
        SetToResult(node->properties, so);
        flag = false;
      }
      if (!node_name.compare(XML_TEST_FUNCTION_PARAM_GARBAGE)) {
        (*so)[ToString(node->properties->name)];
        flag = false;
      }
      if (!node_name.compare(XML_TEST_FUNCTION_PARAM_PATCH)) {
        SetPatchMsgParam(node->properties, so);
        flag = false;
      }
      if (!node_name.compare(XML_TEST_FUNCTION_PARAM_ARRAY)) {
        std::string array_name(ToString(node->properties->children->content));
        (*so)[array_name] = smart::SmartObject(smart::SmartType_Array);
        PutParamsToSO(node->children, &((*so)[array_name]));
        flag = false;
      }
      if (!node_name.compare(XML_TEST_FUNCTION_PARAM_FLOAT)) {
        PutFloatToSO(node->properties, so);
        flag = false;
      }
      if (flag) {
        SSMap::iterator elem = params_.find(node_name);
        if (elem != params_.end()) {
          PutParamsToSO(node->children, &(*so)[elem->second]);
        } else {
          PutParamsToSO(node->children, &(*so)[node_name]);
        }
      }
      node = node->next;
    }
  }

  void XmlParser::PutVectorToSO(xmlNode* node, smart::SmartObject* so) {
    int index = 0;
    while (node) {
      MoveToTag(&node);
      if (!node) break;

      std::string nodeName(ToString(node->name));

      bool flag = true;
      if (!nodeName.compare(XML_TEST_FUNCTION_PARAM_STRING)) {
        SetStringMsgParam(node->properties, &(*so)[index]);
        flag = false;
      }
      if (flag)
            PutParamsToSO(node->children, &(*so)[index]);
      index++;
      node = node->next;
    }
  }

  void XmlParser::SetIntegerMsgParam(const xmlAttr* xmlParam,
                                     smart::SmartObject* so) {
    if (!xmlParam) return;

    std::string msgParam(ToString(xmlParam->name));
    if ((!xmlParam->children)||(!xmlParam->children->content)) return;

    std::string msgValue(ToString(xmlParam->children->content));
    if (!VerifyString(msgParam)) return;

    (*so)[msgParam] = StrToInt(msgValue);
  }

  void XmlParser::SetBooleanMsgParam(const xmlAttr* xmlParam,
                                     smart::SmartObject* so) {
    if (!xmlParam) return;

    std::string msgParam(ToString(xmlParam->name));
    if ((!xmlParam->children)||(!xmlParam->children->content)) return;

    std::string msgValue(ToString(xmlParam->children->content));
    if (!VerifyString(msgParam)) return;

    (*so)[msgParam] = StrToBool(msgValue);
  }

  void XmlParser::SetStringMsgParam(const xmlAttr* xmlParam,
                                    smart::SmartObject* so) {
    if (!xmlParam) return;

    std::string msgParam(ToString(xmlParam->name));
    if ((!xmlParam->children)||(!xmlParam->children->content)) return;

    std::string msgParamValue(ToString(xmlParam->children->content));
    if (!VerifyString(msgParam)) return;
    if (!VerifyString(msgParamValue)) return;

    SSMap::iterator elem = params_.find(msgParam);
    if (elem != params_.end()) {
      (*so)[elem->second] = msgParamValue;
      return;
    }
    if (!msgParam.compare("add")) {
      *so = msgParamValue;
      return;
    }
    (*so)[msgParam] = msgParamValue;
  }


  void XmlParser::SetToResult(const xmlAttr* xmlParam, smart::SmartObject* so) {
    if (!xmlParam) return;

    std::string attrName;
    std::string attrValue;
    while (xmlParam) {
      attrName.append(ToString(xmlParam->name));
      attrValue.append(ToString(xmlParam->children->content));
      if (attrName.compare("return")) {
        (*so)["message_params"][am::strings::result_code] = attrValue;
      } else {
        (*so)[am::strings::params][am::strings::correlation_id] = attrValue;
      }
      xmlParam = xmlParam->next;
    }
  }

  void XmlParser::SetBinaryMsgParam(const xmlAttr* xmlParam,
                                    smart::SmartObject* so) {
    if (!xmlParam) return;

    std::string msgParam(ToString(xmlParam->name));
    if ((!xmlParam->children) || (!xmlParam->children->content)) return;

    std::string msgValue(ToString(xmlParam->children->content));
    if (!VerifyString(msgParam)) return;

    std::vector<uint8_t> binStr;

    std::string fullPatch = profile::Profile::instance()->app_storage_folder();
    std::string full_file_path = fullPatch + "SyncProxyTester/" + msgValue;

    if (file_system::ReadBinaryFile(full_file_path, binStr)) {
      (*so)[am::strings::binary_data] = binStr;
    }
  }

  void XmlParser::SetPatchMsgParam(const xmlAttr* xmlParam,
                                   smart::SmartObject* so) {
    std::string msgParam(ToString(xmlParam->name));
    if ((!xmlParam->children)||(!xmlParam->children->content)) return;

    std::string msgValue(ToString(xmlParam->children->content));
    if (!VerifyString(msgParam)) return;

    std::string fullPatch = profile::Profile::instance()->app_storage_folder();
    std::string full_file_path = fullPatch + "SyncProxyTester/" + msgValue;

    // std::string fullPatch = "SyncProxyTester/" + msgValue;
    // std::string full_file_path = file_system::FullPath(fullPatch);

    (*so)[msgParam] = full_file_path;
  }

  void XmlParser::MoveToTag(xmlNode** node) {
    while (*node) {
      if ((XML_TEXT_NODE == (*node)->type) ||
          (XML_COMMENT_NODE == (*node)->type)) {
        *node = (*node)->next;
      } else {
        break;
      }
    }
  }

  std::string XmlParser::ToString(const xmlChar* str) {
    return std::string(reinterpret_cast<const char*>(str));
  }

  bool XmlParser::VerifyString(const std::string& param) {
    bool good = true;
    const char invalidChars[7] = "\\/&\"<>";

    for (int i = 0; i < param.length(); i++) {
      if (strchr(invalidChars, param[i])) {
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

    for (int i = 0; i < param.length(); i++) {
      if (!strchr(validChars, param[i])) {
        good = false;
        break;
      }
    }
    if (good) {
      val = atoi(param.data());
    }

    return val;
  }

  void XmlParser::PutFloatToSO(const xmlAttr* xmlParam,
                               smart::SmartObject* so) {
    if (!xmlParam) return;

    std::string msgParam(ToString(xmlParam->name));
    if ((!xmlParam->children) || (!xmlParam->children->content)) return;

    std::string msgValue(ToString(xmlParam->children->content));
    if (!VerifyString(msgParam)) return;

    (*so)[msgParam] = atof(msgValue.c_str());
  }

  bool XmlParser::StrToBool(const std::string& param) {
    bool good = false;
    std::string toCheck;

    toCheck.append(param);
    std::transform(toCheck.begin(), toCheck.end(), toCheck.begin(), ::tolower);

    if (!toCheck.compare("true")) {
      good = true;
    }
    return good;
  }
}  // namespace test
