/**
 * @file ApplicationModel.qml
 * @brief Own fields for each application.
 * Copyright (c) 2013, Ford Motor Company
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

import QtQuick 1.1
import "../controls"

QtObject {
    property int appId
    property string appName
    property string ngnMediaScreenAppName
    property Icon appIcon: Icon { }
    property string deviceName
    property int hmiDisplayLanguageDesired
    property bool isMediaApplication
    property variant appType
    property int hmiUITextAlignment

    // media player
    property string playPauseState
    property ApplicationData hmiUIText: ApplicationData { }
    property MediaClockModel mediaClock: MediaClockModel { }

    property string helpPrompt
    property string timeoutPrompt
    property ListModel options: ListModel { }

    property ListModel currentSubMenu: options
    property ListModel turnList
    property ListModel softButtons
    property ListModel turnListSoftButtons
    property ListModel customPresets
    property int languageTTSVR
    // This place is for adding new properties

    function reset () {
        console.debug("enter")
        appId = -1
        appName = ""
        ngnMediaScreenAppName = ""
        appIcon.reset()
        deviceName = ""
        hmiDisplayLanguageDesired = -1
        isMediaApplication = false
        appType = undefined
        playPauseState = ""
        hmiUIText.reset()
// no need to reset mediaClock
        helpPrompt = ""
        timeoutPrompt = ""
        options.clear()
        currentSubMenu = options
        turnList = null
        softButtons = null
        turnListSoftButtons = null
        customPresets = null
        languageTTSVR = -1
        hmiUITextAlignment = null
        console.debug("exit")
    }
}
