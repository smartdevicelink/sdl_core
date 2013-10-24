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

import QtQuick 2.0
import "Internal.js" as Internal

QtObject {
    property int hours
    property int minutes
    property int seconds

    property int updateMode

    property real progress: 1 / 3

    property Timer timer: Timer {
        interval: 1000
        repeat: true
        running: true
        onTriggered: onTimer()
    }

    function onTimer () {
        switch (updateMode) {
            case Internal.MediaClockUpdateMode.MCU_COUNTUP:
                if (60 === ++seconds) {
                    seconds = 0
                    if (60 === ++minutes) {
                        minutes = 0
                        ++hours
                    }
                }
                break
            case Internal.MediaClockUpdateMode.MCU_COUNTDOWN:
                if (0 === seconds) {
                    if (0 === minutes) {
                        if (0 === hours) {
                            timer.stop()
                            console.log("countdown timer stopped")
                        }
                        else {
                            --hours
                            minutes = 59
                            seconds = 59
                        }
                    }
                    else {
                        --minutes
                        seconds = 59
                    }
                }
                else {
                    --seconds
                }
                break
        }
    }
}
