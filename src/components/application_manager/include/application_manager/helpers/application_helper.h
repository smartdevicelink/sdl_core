/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HELPERS_APPLICATION_HELPER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HELPERS_APPLICATION_HELPER_H_

#include "application_manager/application.h"
#include "application_manager/application_manager.h"

/*
 * This file is for code w/o direct usage of ApplicationManagerImpl internals
 */
namespace application_manager {

/**
 * Helper function for lookup through applications list and returning first
 * application satisfying predicate logic
 */
template <class UnaryPredicate>
ApplicationSharedPtr FindApp(DataAccessor<ApplicationSet> accessor,
                             UnaryPredicate finder) {
  ApplicationSet::iterator it = std::find_if(
      accessor.GetData().begin(), accessor.GetData().end(), finder);
  if (accessor.GetData().end() == it) {
    return ApplicationSharedPtr();
  }
  ApplicationSharedPtr app = *it;
  return app;
}

template <class UnaryPredicate>
ApplicationSharedPtr FindPendingApp(
    DataAccessor<AppsWaitRegistrationSet> accessor, UnaryPredicate finder) {
  AppsWaitRegistrationSet::iterator it = std::find_if(
      accessor.GetData().begin(), accessor.GetData().end(), finder);
  if (accessor.GetData().end() == it) {
    return ApplicationSharedPtr();
  }
  ApplicationSharedPtr app = *it;
  return app;
}

/**
 * Helper function for lookup through applications list and returning all
 * applications satisfying predicate logic
 */
template <class UnaryPredicate>
std::vector<ApplicationSharedPtr> FindAllApps(
    DataAccessor<ApplicationSet> accessor, UnaryPredicate finder) {
  std::vector<ApplicationSharedPtr> result;
  ApplicationSetConstIt it = std::find_if(
      accessor.GetData().begin(), accessor.GetData().end(), finder);
  while (it != accessor.GetData().end()) {
    result.push_back(*it);
    it = std::find_if(++it, accessor.GetData().end(), finder);
  }
  return result;
}

/**
 * @brief RecallApplicationData cleans up data within application instance to
 * its initial state and notifies HMI to delete same data on its side
 * @param app Application to clean its data
 * @param app_manager Application manager
 */
void DeleteApplicationData(ApplicationSharedPtr app,
                           ApplicationManager& app_manager);
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HELPERS_APPLICATION_HELPER_H_
