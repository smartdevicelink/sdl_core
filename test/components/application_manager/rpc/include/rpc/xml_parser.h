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
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"
#include "libxml/xmlreader.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "formatters/formatter_json_rpc.h"
#include "utils/file_system.h"
#include "config_profile/ini_file.h"
#include "config_profile/profile.h"

namespace am = application_manager;
namespace smart = NsSmartDeviceLink::NsSmartObjects;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;

namespace test {
  /**
   * @brief Typedef for string params
   */
  typedef std::map<std::string, std::string> SSMap;

  /**
   * @brief Singleton class for parsing XML-file with requests
   */
  class XmlParser {
   public:
    static XmlParser* instance();

    /**
     * @brief Sets current dir path with XML-files
     *
     * @param std::string pith path
     */
    void SetXmlDir(const std::string& path);

    /**
     * @brief Gets current dir path with XML-files
     *
     * @return returns std::string with current path
     */
    std::string GetXmlDir() const;

    /**
     * @brief Gets current xml-file name
     *
     * @return returns std::string with current xml-file name
     */
    std::string GetXmlFileName() const;

    /**
     * @brief Opens a dir with xml-files
     *
     * @return true if succesfull, otherwise false
     */
    bool OpenDir();

    /**
     * @brief Finds first xml-file in dir and then next xml-files
     *
     * @return true if succesfull, otherwise false
     */
    bool NextXml();

    /**
     * @brief Reads current xml-file
     *
     * @return true if succesfull, otherwise false
     */
    bool ReadXml();

    /**
     *@brief Closes current xml-file
     */
    void CloseXml();

    /**
     * @brief Closes current dir with xml-files
     */
    void CloseDir();

    /**
     * @brief Finds first and then next test nodes
     *
     * @return true if succesfull
     */
    bool MoveToNextTestNode();

    /**
     * @brief Fill smartobject with test params
     *
     * @param SmartObject to fill
     *
     * @return true if succesfull
     */
    bool GetRequestFromTest(smart::SmartObject* so);

   private:
    /**
     * @brief Constructor
     */
    XmlParser();

    /**
     * @brief Distructor
     */
    ~XmlParser();

    /**
     * @brief Finds first test node in xml-file
     *
     * @return true if succesfull
     */
    bool FindFirstTestNode();

    /**
     * @brief Recursive function that finds test nodes
     *
     * @param xmlNode* to find in
     * @param xmlNode*& found test node
     *
     * @return true if succesfull
     */
    bool SearchInNodes(xmlNode* node, xmlNode** testNode);

    /**
     * @brief Moves to next test node
     *
     * @return true if succesfull
     */
    bool NextTestNode();

    /**
     * @brief Reads test attributes and puts them into SmartObject
     *
     * @param SmartObject& to fill
     */
    void SetTestAttributes(smart::SmartObject* so);

    /**
     * @brief Recursively fills smartobject with params in test node
     *
     * @param xmlNode* to start with
     * @param SmartObject& to fill
     */
    void PutParamsToSO(xmlNode* node, smart::SmartObject* so);

    /**
     * @brief Recursively puts vector param to smartobject
     *
     * @param xmlNode* vector node
     * @param SmartObject& to fill
     */
    void PutVectorToSO(xmlNode* node, smart::SmartObject* so);

    /**
     * @brief Puts an integer param in smartobject
     *
     * @param xmlAttr* pointer to parameter
     * @param SmartObject& to put in
     */
    void SetIntegerMsgParam(const xmlAttr* xml_param, smart::SmartObject* so);

    /**
     * @brief Puts a boolean parameter in smartobject
     *
     * @param xmlAttr* pointer to parameter
     * @param SmartObject& to put in
     */
    void SetBooleanMsgParam(const xmlAttr* xml_param, smart::SmartObject* so);

    /**
     * @brief Puts a string parameter in smartobject
     *
     * @param xmlAttr* pointer to parameter
     * @param SmartObject& to put in
     */
    void SetStringMsgParam(const xmlAttr* xml_param, smart::SmartObject* so);

    /**
     * @brief Puts a "return" parameter in smartobject
     *
     * @param xmlAttr* pointer to parameter
     * @param SmartObject& to put in
     */
    void SetToResult(const xmlAttr* xml_param, smart::SmartObject* so);

    /**
     * @brief Puts a binary parameter in smartobject
     *
     * @param xmlAttr* pointer to parameter
     * @param SmartObject& to put in
     */
    void SetBinaryMsgParam(const xmlAttr* xml_param, smart::SmartObject* so);

    /**
     * @brief Puts a patch parameter in smartobject
     *
     * @param xmlAttr* pointer to parameter
     * @param SmartObject& to put in
     */
    void SetPatchMsgParam(const xmlAttr* xml_param, smart::SmartObject* so);

    /**
     * @brief just moves to next valid tag (not text tag or comment tag)
     *
     * @param xmlNode*& node to start from
     */
    void MoveToTag(xmlNode** node);

    /**
     * @brief converts a xmlChar string to std::string
     *
     * @param xmlChar* string
     *
     * @return std::string with converted xmlChar string
     */
    std::string ToString(const xmlChar* str);

    /**
     * @brief veifies if a string has no invalid chars
     *
     * @param std::string& to check
     *
     * @return true if a string has no invalid chars, otherwise false
     */
    bool VerifyString(const std::string& param);

    /**
     * @brief checks if a string has only number chars and converts it to
     * integer
     *
     * @param std::string& to check and convert
     *
     * @return converted value if a string has been converted, otherwise -1
     */
    int StrToInt(const std::string& param);

    /**
     * @brief Puts a float parameter in smartobject
     *
     * @param xmlAttr* pointer to parameter
     * @param SmartObject& to put in
     */
    void PutFloatToSO(const xmlAttr* xmlParam, smart::SmartObject* so);

    /**
     * @brief checks if a string consists only of "true" or "false"
     *
     * @param std::string& to check and convert
     *
     * @return if string consists of "true", returns true, other occations
     * - false
     */
    bool StrToBool(const std::string& param);

    std::string s_xml_dir_;
    DIR* xml_dir_;
    dirent* xml_file_;
    std::string s_xml_file_name_;
    xmlDocPtr xml_doc_;
    xmlNode* xml_test_node_;

    bool dir_opened_;
    bool xml_opened_;
    bool xml_file_exists_;
    bool first_test_node_found_;

    SSMap params_;
  };
}  // namespace test

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_XML_PARSER_H_
