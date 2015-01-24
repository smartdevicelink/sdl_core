/*
 Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_DELEGATES_APP_PERMISSION_DELEGATE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_DELEGATES_APP_PERMISSION_DELEGATE_H_


#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/conditional_variable.h"

#include "policy/policy_types.h"

namespace policy {

  /**
 * @brief The AppPermissionDelegate class allows to call OnAppPermissionConsent
 * in async way.
 */
class AppPermissionDelegate: public threads::ThreadDelegate {
  public:
    /**
     * @brief AppPermissionDelegate constructor, contains parameters
     * which will be pass to the called function.
     *
     * @param connection_key connection key.
     *
     * @param permissions new permissions
     */
    AppPermissionDelegate(const uint32_t connection_key,
                          const PermissionConsent &permissions);

    /**
     * @brief threadMain run the needed function.
     */
    virtual void threadMain();

    /**
     * @brief exitThreadMain do some stuff before exit from thread
     *
     * @return true in case when thread has been finished properly
     */
    virtual void exitThreadMain();

  private:
    uint32_t connection_key_;
    PermissionConsent permissions_;
};

} // namespace policy

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_DELEGATES_APP_PERMISSION_DELEGATE_H_
