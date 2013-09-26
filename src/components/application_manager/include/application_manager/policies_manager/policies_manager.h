/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICIES_MANAGER_POLICIES_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICIES_MANAGER_POLICIES_MANAGER_H_

#include <map>
#include <vector>
#include <string>
#include <utility>

#include "interfaces/MOBILE_API.h"

#include "utils/logger.h"
#include "utils/macro.h"

namespace application_manager {
namespace policies_manager {

typedef mobile_apis::FunctionID::eType ItemName;
typedef std::vector<mobile_apis::HMILevel::eType> HmiLevels;
typedef std::pair<ItemName, HmiLevels> PoliciesItem;
typedef std::map<ItemName, HmiLevels> PoliciesItems;

class PoliciesManager {
 public:
  PoliciesManager();
  ~PoliciesManager();

  bool Init();
  bool IsValidHmiStatus(mobile_apis::FunctionID::eType function,
                           mobile_apis::HMILevel::eType status);

 private:
  PoliciesItems items_;
  static log4cxx::LoggerPtr logger_;

  DISALLOW_COPY_AND_ASSIGN(PoliciesManager);
};

}  // namespace policies_manager
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICIES_MANAGER_POLICIES_MANAGER_H_
