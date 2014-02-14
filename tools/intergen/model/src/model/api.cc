/**
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

#include "model/api.h"

#include <cassert>
#include <iostream>

#include "model/constant.h"
#include "pugixml.hpp"

namespace codegen {

API::API(const ModelFilter* model_filter, bool auto_generate_func_ids)
    : model_filter_(model_filter),
      auto_generate_func_ids_(auto_generate_func_ids),
      interfaces_deleter_(&interfaces_) {
  assert(model_filter_);
}

bool API::init(const pugi::xml_document& xmldoc) {
  // Search for interfaces defined directly in given root document
  if (!AddInterfaces(xmldoc)) {
    return false;
  }
  // Then search for interfaces defined in <interfaces> elements
  for (pugi::xml_node i = xmldoc.child("interfaces"); i;
      i = i.next_sibling("interfaces")) {
    if (!AddInterfaces(i)) {
      return false;
    }
  }
  return true;
}

API::~API() {
}

const std::vector<Interface*>& API::interfaces() const {
  return interfaces_;
}

const Interface* API::InterfaceByName(const std::string& name) const {
  InterfacesIndex::const_iterator i = interfaces_index_.find(name);
  if (i != interfaces_index_.end()) {
    size_t interface_id = i->second;
    assert(interface_id < interfaces_.size());
    return interfaces_[interface_id];
  }
  return NULL;
}

bool API::AddInterfaces(const pugi::xml_node& xmldoc) {
  for (pugi::xml_node i = xmldoc.child("interface"); i;
      i = i.next_sibling("interface")) {
    Interface* interface = new Interface(this,
                                         auto_generate_func_ids_,
                                         &builtin_type_registry_,
                                         model_filter_);
    interfaces_.push_back(interface);
    if (!interface->init(i)) {
      return false;
    }
    bool inserted = interfaces_index_.insert(
          std::make_pair(
            interface->name(),
            interfaces_.size() - 1)
        ).second;
    if (!inserted) {
      std::cerr << "Duplicate interface: " << interface->name() << '\n';
      return false;
    }
  }
  return true;
}

}  // namespace codegen
