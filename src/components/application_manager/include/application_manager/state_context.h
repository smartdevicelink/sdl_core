/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTEXT_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTEXT_H_

#include <inttypes.h>

namespace application_manager {
/**
* @brief The StateContext implement acessing to data, that is required by HmiState
*/
class StateContext {
  public:

    /**
     * @brief is_navi_app check if app is navi
     * @param app_id application id
     * @return true if app is navi, otherwise return false
     */
    bool is_navi_app(const uint32_t app_id) const;

    /**
     * @brief is_meida_app check if app is is meida
     * @param app_id application id
     * @return @return true if meida_app, otherwise return false
     */
    bool is_meida_app(const uint32_t app_id) const;

    /**
     * @brief is_voice_comunication_app check if app is voice comunication
     * @param app_id application id
     * @return @return true if voice_comunication_app, otherwise return false
     */
    bool is_voice_comunication_app(const uint32_t app_id) const;

    /**
     * @brief is_attenuated_supported check if HMI support attenuated mode
     * @return true if supported, otherwise return false
     */
    bool is_attenuated_supported() const;
};

}
#endif // STATE_CONTEXT_H

