/**
 * @file object_optional_schema_item.cc
 * @brief Schema item of object with optional parameters.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "smart_objects/object_optional_schema_item.h"
#include "smart_objects/always_false_schema_item.h"
#include "smart_objects/smart_object.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

namespace so_ns = NsSmartDeviceLink::NsSmartObjects;

const char* so_ns::ObjectOptionalSchemaItem::sOptionalGenericFieldName1
  = "<-- @@@@$$$&&& uniqe1 uancdm,vn 991188 --->";


//----------------------------------------------------------------------------

utils::SharedPtr<so_ns::ObjectOptionalSchemaItem>
  so_ns::ObjectOptionalSchemaItem::create(
    const std::map<std::string,
    so_ns::CObjectSchemaItem::SMember> & members) {
  return new ObjectOptionalSchemaItem(members);
}

//----------------------------------------------------------------------------

so_ns::Errors::eType so_ns::ObjectOptionalSchemaItem::validate(
    const so_ns::SmartObject & object) {
  so_ns::Errors::eType result = so_ns::Errors::ERROR;

  if (NsSmartDeviceLink::NsSmartObjects::SmartType_Map == object.getType()) {
    result = so_ns::Errors::OK;
    const std::set<std::string> object_keys = object.enumerate();

    for (std::map<std::string, CObjectSchemaItem::SMember>
          ::const_iterator i = mMembers.begin(); i != mMembers.end(); ++i) {
      if (object_keys.end() != object_keys.find(i->first)) {
          result = i->second.mSchemaItem->validate(object.getElement(i->first));
      } else if (true == i->second.mIsMandatory
          && false == IsOptionalName(i->first)) {
        result = so_ns::Errors::MISSING_MANDATORY_PARAMETER;
      }
      if (so_ns::Errors::OK != result) {
        break;
      }
    }

    bool have_optional_parameter = false;
    std::map<std::string, CObjectSchemaItem::SMember>::const_iterator
      optional_iterator = mMembers.find(sOptionalGenericFieldName1);
    if (so_ns::Errors::OK == result) {
      for (std::set<std::string>::const_iterator k = object_keys.begin();
            k != object_keys.end(); ++k) {
        if (mMembers.end() == mMembers.find(*k)) {
          // this key is supposed as optional
          if (mMembers.end() != optional_iterator) {
            // validate optional (having any name) object member
            have_optional_parameter = true;
            result = optional_iterator->
                     second.mSchemaItem->validate(object.getElement(*k));
          } else {
            result = so_ns::Errors::UNEXPECTED_PARAMETER;
            break;
          }
          if (so_ns::Errors::OK != result) {
            break;
          }
        }
      }
      if (false == have_optional_parameter
          && mMembers.end() != optional_iterator
          && true == optional_iterator->second.mIsMandatory) {
        result = so_ns::Errors::MISSING_MANDATORY_UNTITLED_PARAMETER;
      }
    }
  } else {
    result = so_ns::Errors::INVALID_VALUE;
  }

  return result;
}

//----------------------------------------------------------------------------

bool so_ns::ObjectOptionalSchemaItem::IsOptionalName(std::string name) {
  return (0 == name.compare(sOptionalGenericFieldName1));
}

//----------------------------------------------------------------------------

so_ns::ObjectOptionalSchemaItem::ObjectOptionalSchemaItem(
    const std::map<std::string, so_ns::CObjectSchemaItem::SMember> & members):
  CObjectSchemaItem(members) {
}

//----------------------------------------------------------------------------

std::set<std::string>
ObjectOptionalSchemaItem::GetOptionalObjectKeys(SmartObject &root_obj) {
  std::set<std::string> optional_objects;

  const std::set<std::string> object_keys = root_obj.enumerate();
  typedef std::set<std::string>::const_iterator Iter;
  for (Iter k = object_keys.begin(); k != object_keys.end(); ++k) {
    // if an object is not among the schema items then it is optional
    if (mMembers.end() == mMembers.find(*k)) {
      optional_objects.insert(*k);
    }
  }

  return optional_objects;
}

//----------------------------------------------------------------------------

void ObjectOptionalSchemaItem::applySchema(SmartObject & Object) {
  // At first apply schema for the regular objects
  CObjectSchemaItem::applySchema(Object);

  if (0 == mMembers.count(sOptionalGenericFieldName1)) {
    return;             // There are no optional items
  }

  // Then apply schema for all the optional objects
  std::set<std::string> optionals = GetOptionalObjectKeys(Object);
  typedef std::set<std::string>::const_iterator Iter;
  for (Iter key = optionals.begin(); key != optionals.end(); ++key) {

    mMembers.at(sOptionalGenericFieldName1).mSchemaItem->applySchema(
      Object[*key]);
  }
}

//----------------------------------------------------------------------------

void ObjectOptionalSchemaItem::unapplySchema(SmartObject & Object) {
  CObjectSchemaItem::unapplySchema(Object);

  if (0 == mMembers.count(sOptionalGenericFieldName1)) {
    return;             // There are no optional items
  }

  std::set<std::string> optionals = GetOptionalObjectKeys(Object);
  typedef std::set<std::string>::iterator Iter;
  for (Iter key = optionals.begin(); key != optionals.end(); ++key) {

    mMembers.at(sOptionalGenericFieldName1).mSchemaItem->unapplySchema(
      Object[*key]);
  }
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink

