/*
 * Copyright (c) 2014, Ford Motor Company
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
#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_SCHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_SCHEMA_ITEM_H_

#include <map>
#include <set>
#include <string>

#include <boost/optional.hpp>
#include "utils/macro.h"
#include "utils/semantic_version.h"

#include "smart_objects/schema_item.h"
#include "smart_objects/schema_item_parameter.h"

namespace ns_smart_device_link {
namespace ns_smart_objects {

/**
 * @brief Object member.
 **/
struct SMember {
  /**
   * @brief Default constructor.
   **/
  SMember();
  /**
   * @brief Constructor.
   * @param SchemaItem Member schema item.
   * @param IsMandatory true if member is mandatory, false
   *                    otherwise. Defaults to true.
   **/

  SMember(const ISchemaItemPtr SchemaItem,
          const bool IsMandatory = true,
          const std::string& Since = "",
          const std::string& Until = "",
          const bool IsDeprecated = false,
          const bool IsRemoved = false,
          const std::vector<SMember>& history_vector = {});

  /**
   * @brief Checks the version a parameter was removed (until)
   * If the mobile's msg version is greater than or
   **/
  bool CheckHistoryFieldVersion(
      const utils::SemanticVersion& MessageVersion) const;

  /**
   * @brief Member schema item.
   **/
  ISchemaItemPtr mSchemaItem;
  /**
   * @brief true if member is mandatory, false otherwise.
   **/
  bool mIsMandatory;
  boost::optional<utils::SemanticVersion> mSince;
  boost::optional<utils::SemanticVersion> mUntil;
  bool mIsDeprecated;
  bool mIsRemoved;
  std::vector<SMember> mHistoryVector;
};
typedef std::map<std::string, SMember> Members;

/**
 * @brief Object schema item.
 **/
class CObjectSchemaItem : public ISchemaItem {
 public:
  /**
   * @brief Create a new schema item.
   *
   * @param Members Map of member name to SMember structure
   *                describing the object member.
   *
   * @return Shared pointer to a new schema item.
   **/
  static std::shared_ptr<CObjectSchemaItem> create(const Members& Members);

  /**
   * @brief Validate smart object.
   * @param Object Object to validate.
   * @param report object for reporting errors during validation
   * @param MessageVersion to check mobile RPC version against RPC Spec History
   * @param allow_unknown_enums
   *   false - unknown enum values (left as string values after applySchema)
   *   will be considered invalid.
   *   true - such values will be considered valid.
   * @return ns_smart_objects::errors::eType
   **/
  errors::eType validate(
      const SmartObject& Object,
      rpc::ValidationReport* report,
      const utils::SemanticVersion& MessageVersion = utils::SemanticVersion(),
      const bool allow_unknown_enums = false) OVERRIDE;

  bool filterInvalidEnums(SmartObject& Object,
                          const utils::SemanticVersion& MessageVersion,
                          rpc::ValidationReport* report) OVERRIDE;

  /**
   * @brief Apply schema.
   *
   * @param Object Object to apply schema.
   * @param remove_unknown_parameters contains true if need to remove unknown
   * parameters from smart object, otherwise contains false.
   * @param MessageVersion the version of the schema to be applied
   **/
  void applySchema(SmartObject& Object,
                   const bool remove_unknown_parameters,
                   const utils::SemanticVersion& MessageVersion =
                       utils::SemanticVersion()) OVERRIDE;
  /**
   * @brief Unapply schema.
   * @param Object Object to unapply schema.
   * @param remove_unknown_parameters contains true if need to remove unknown
   *                                  parameters
   **/
  void unapplySchema(SmartObject& Object,
                     const bool remove_unknown_parameters) OVERRIDE;
  /**
   * @brief Build smart object by smart schema having copied matched
   *        parameters from pattern smart object
   * @param pattern_object pattern object
   * @param result_object object to build
   */
  void BuildObjectBySchema(const SmartObject& pattern_object,
                           SmartObject& result_object) OVERRIDE;
  /**
   * @brief Get size mMembers map
   * @return Size of mMembers
   */
  size_t GetMemberSize() OVERRIDE;

  TypeID GetType() OVERRIDE;

  boost::optional<SMember&> GetMemberSchemaItem(
      const std::string& member_key) OVERRIDE;

  void AddMemberSchemaItem(const std::string& member_key,
                           SMember& member) OVERRIDE;

 protected:
  /**
   * @brief Constructor.
   * @param Members Map of member name to SMember structure
   *                describing the object member.
   **/
  CObjectSchemaItem(const Members& Members);

  /**
   * @brief Removes unknown parameters from object.
   * @param Object Object to remove unknown parameters.
   * @param MessageVersion The version to check against for which parameters are
   *                       unknown.
   **/
  void RemoveUnknownParams(SmartObject& Object,
                           const utils::SemanticVersion& MessageVersion);

  /**
   * @brief Returns the correct schema item based on message version.
   * @param member Schema member
   * @param MessageVersion Semantic Version of mobile message.
   * @return Pointer to correct schema item if item found or nullptr, if item
   *         was not found.
   **/
  const SMember* GetCorrectMember(const SMember& member,
                                  const utils::SemanticVersion& messageVersion);

  /**
   * @brief Map of member name to SMember structure describing the object
   *        member.
   **/
  Members mMembers;
  DISALLOW_COPY_AND_ASSIGN(CObjectSchemaItem);
};
}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_SCHEMA_ITEM_H_
