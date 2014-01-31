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

#ifndef API_H_
#define API_H_
#include <vector>

#include "model/builtin_type_registry.h"
#include "model/interface.h"
#include "utils/macro.h"
#include "utils/stl_utils.h"

namespace pugi {
class xml_document;
}  // namespace pugi

namespace codegen {
class ModelFilter;

/*
 * Represents single parsed XML file that defines an API
 */
class API {
 public:
  API(const ModelFilter* model_filter);
  // Follows parsed |xmldoc| collecting and validating API definitions
  // Returns false and prints to cerr on error
  bool init(const pugi::xml_document& xmldoc);
  ~API();
  // List of all interfaces collected from xml document
  const std::vector<Interface*>& interfaces() const;

 private:
  const ModelFilter* model_filter_;
  BuiltinTypeRegistry builtin_type_registry_;
  std::vector<Interface*> interfaces_;
  utils::StdContainerDeleter<std::vector<Interface*> > interfaces_deleter_;
  DISALLOW_COPY_AND_ASSIGN(API);
};

}  // namespace codegen

#endif /* API_H_ */
