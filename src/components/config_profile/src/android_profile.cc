/*
 * Copyright (c) 2021, Ford Motor Company
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

#include "config_profile/android_profile.h"
#include <string>

namespace profile {

AndroidProfile::AndroidProfile(const std::string &internal_storage_dir,
                               const std::string &external_storage_dir)
                               : internal_storage_dir_(internal_storage_dir)
                               , external_storage_dir_(external_storage_dir) {}

void AndroidProfile::SubstituteMacroInString(std::string &in_string) const {
    std::string out_string = in_string;
    const std::map<std::string, std::string> replace_macro = {
            {"%ANDROID_INTERNAL_DIR%", internal_storage_dir_},
            {"%ANDROID_EXTERNAL_DIR%", external_storage_dir_}
    };

    for (const auto& macro : replace_macro) {
        size_t index = 0;
        while (true) {
            index = out_string.find(macro.first, index);
            if (index == std::string::npos) break;

            out_string.replace(index, macro.first.size(), macro.second);
            index += macro.first.size();
        }
    }

    in_string = out_string;
}

}