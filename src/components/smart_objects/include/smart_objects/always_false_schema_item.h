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

#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_ALWAYS_FALSE_SCHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_ALWAYS_FALSE_SCHEMA_ITEM_H_

#include "smart_objects/schema_item.h"

namespace ns_smart_device_link {
namespace ns_smart_objects {
/**
 * @brief Always false schema item.
 **/
class CAlwaysFalseSchemaItem : public ISchemaItem {
 public:
  /**
   * @brief Create a new schema item.
   * @return Shared pointer to a new schema item.
   **/
  static std::shared_ptr<CAlwaysFalseSchemaItem> create();

  /**
   * @brief Validate smart object.
   * @param Object Object to validate.
   * @param report__ object for reporting errors during validation
   * @param MessageVersion to check mobile RPC version against RPC Spec History
   * @return ns_smart_objects::errors::eType
   **/
  errors::eType validate(
      const SmartObject& Object,
      rpc::ValidationReport* report__,
      const utils::SemanticVersion& MessageVersion = utils::SemanticVersion(),
      const bool allow_unknown_parameters = false) OVERRIDE;

 private:
  CAlwaysFalseSchemaItem();
  DISALLOW_COPY_AND_ASSIGN(CAlwaysFalseSchemaItem);
};
}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_ALWAYS_FALSE_SCHEMA_ITEM_H_
