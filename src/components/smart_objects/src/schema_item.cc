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

#include "smart_objects/schema_item.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

Errors::eType ISchemaItem::validate(const SmartObject& Object) {
  return Errors::ERROR;
}

Errors::eType ISchemaItem::validate(const SmartObject& object,
                                    rpc::ValidationReport* report__) {
  return Errors::ERROR;
}

bool ISchemaItem::setDefaultValue(SmartObject& Object) {
  return false;
}

bool ISchemaItem::hasDefaultValue(SmartObject& Object) {
  return false;
}

void ISchemaItem::applySchema(SmartObject& Object,
                              const bool RemoveFakeParameters) {}

void ISchemaItem::unapplySchema(SmartObject& Object) {}

void ISchemaItem::BuildObjectBySchema(const SmartObject& pattern_object,
                                      SmartObject& result_object) {}

size_t ISchemaItem::GetMemberSize() {
  return 0;
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
