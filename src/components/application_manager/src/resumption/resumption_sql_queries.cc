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
#include "application_manager/resumption/resumption_sql_queries.h"

namespace resumption {

const std::string kCreateSchema;

const std::string kSelectHMILevel =
    "SELECT `hmiLevel` FROM `application` "
    "WHERE `deviceID` = ? AND `AppID` = ?";

const std::string kCheckHMIId =
    "SELECT `hmiAppID` FROM `application` "
    "WHERE `hmiAppID` = ?";

const std::string kSelectHMIId =
    "SELECT `hmiAppID` FROM `application` "
    "WHERE `deviceID` = ? AND `AppID` = ?";

const std::string kSelectHashId =
    "SELECT `hashID` FROM `application` "
    "WHERE `deviceID` = ? AND `AppID` = ?";

const std::string kSelectIgnOffTime =
    "SELECT `last_ign_off_time` FROM `resumption`";

const std::string kCheckApplication =
    "SELECT COUNT (`deviceID`) FROM `application` "
    "WHERE `deviceID` = ? AND `AppID` = ?";

const std::string kSelectDataForLoadResumeData =
    "SELECT `hmiLevel`, `ign_off_count`, `timeStamp`, `appID`, `deviceID` "
    "FROM `application`"
    "WHERE `hmiLevel` IS NOT NULL AND `ign_off_count` IS NOT NULL "
    "AND `timeStamp` IS NOT NULL AND `appID` IS NOT NULL AND `deviceID` IS NOT NULL";

const std::string kUpdateHMILevel =
    "UPDATE `application` "
    "SET `hmiLevel` = ? "
    "WHERE `deviceID` = ? AND `AppID` = ?";

const std::string kDeleteApplication =
    "DELETE "
    "FROM `application` "
    "WHERE `deviceID` = ? AND `AppID` = ?";

const std::string kUpdateIgnOffCount =
    "UPDATE `application` "
    "SET `ign_off_count` = `ign_off_count` - 1"
    "WHERE `ign_off_count` > 0";

const std::string kDeleteApplicationsAccordingWithIgnOffCount =
    "DELETE "
    "FROM `application` "
    "WHERE `ign_off_count` >= ?";

const std::string kUpdateSuspendData =
    "UPDATE `application` "
    "SET `ign_off_count` = `ign_off_count` + 1, "
    "`suspend_count` = `suspend_count` + 1";

const std::string KUpdateLastIgnOffTime =
    "UPDATE `resumption` "
    "SET `last_ign_off_time` = ?";

}  // namespace resumption
