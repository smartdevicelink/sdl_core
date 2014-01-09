/**
 * @file MediaClockModel.qml
 * @brief Media clock model
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
import "Internal.js" as Internal

QtObject {
    property int updateMode
    property int runningMode
    property int startTime
    property int endTime
    property int upperTimeLimit: 60 * 60 * 60 - 1
    property int startTimeForProgress
    property real progress

    property Timer timer: Timer {
        id: timer
        interval: 1000
        repeat: true
        onTriggered: onTimer()
    }
    onRunningModeChanged: {
        if (runningMode === Internal.MediaClockRunningMode.MCR_STOPPED) {
            timer.stop()
        } else {
            timer.start()
        }
    }

    onStartTimeChanged: {
        if (startTime === -1) {
            progress = 0
        } else {
            if (updateMode === Internal.MediaClockUpdateMode.MCU_COUNTUP) {
                progress = (endTime !== -1) ? (startTime / endTime) : (startTime / upperTimeLimit)
            } else {
                progress = (endTime !== -1) ? ( (startTime - endTime) / (startTimeForProgress - endTime) )
                                            : (startTime / startTimeForProgress)
            }
        }
    }

    function onTimer () {
        switch (updateMode) {
            case Internal.MediaClockUpdateMode.MCU_COUNTUP:
                if (endTime !== -1) {
                    if (startTime < endTime) {
                        startTime++
                    } else {
                        timer.stop()
                        runningMode = Internal.MediaClockRunningMode.MCR_STOPPED
                        console.debug("count Up timer stopped")
                    }
                } else {
                    if (startTime < upperTimeLimit) {
                        startTime++
                    } else {
                        startTime = 0
                    }
                }
                break
            case Internal.MediaClockUpdateMode.MCU_COUNTDOWN:
                console.debug("count down")
                if (--startTime === 0) {
                    timer.stop()
                    runningMode = Internal.MediaClockRunningMode.MCR_STOPPED
                    startTime = endTime = -1
                    console.debug("count Down timer stopped")
                }
                break
        }
        dataContainer.setApplicationProperties(dataContainer.currentApplication.appId, {
                                     "mediaClock": {startTime: startTime,
                                                   endTime: endTime,
                                                   updateMode: updateMode,
                                                   runningMode: runningMode,
                                                   startTimeForProgress: startTimeForProgress
                                                   }
                                 })
    }
}
