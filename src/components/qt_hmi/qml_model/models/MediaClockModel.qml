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
    property int hmsTime
    property real magic // either difference or sum with current time - depends on update mode

    property int updateMode

    property real progress: 1 / 3

    property Timer timer: Timer {
        interval: 1000
        repeat: true
        onTriggered: onTimer()
    }

    function onTimer () {
        switch (updateMode) {
            case Internal.MediaClockUpdateMode.MCU_COUNTUP:
                ++hmsTime
                break
            case Internal.MediaClockUpdateMode.MCU_COUNTDOWN:
                if (--hmsTime === 0) {
                    timer.stop()
                    console.log("countdown timer stopped")
                }
                break
            case Internal.MediaClockUpdateMode.MCU_STOPPED:
                timer.stop()
                console.debug("warning: onTimer() launched for stopped clock")
                break
        }
    }

    function restore (updateMode, runningMode, magic) {
        console.debug("MediaClockModel::restore(" + updateMode + ", " + magic + ")")
        timer.stop()
        var date = new Date()
        var secondsSinceEpoch = date.getTime() / 1000
        this.updateMode = updateMode
        this.magic = magic
        var toStart
        switch (runningMode) {
            case Internal.MediaClockRunMode.MCR_RUNNING:
                switch (updateMode) {
                    case Internal.MediaClockUpdateMode.MCU_COUNTUP:
                        hmsTime = Math.floor(secondsSinceEpoch - magic) // floor() works finer than round()
                        toStart = true
                        break
                    case Internal.MediaClockUpdateMode.MCU_COUNTDOWN:
                        hmsTime = Math.floor(magic - secondsSinceEpoch) // floor() works finer than round()
                        if (hmsTime > 0) {
                            toStart = true
                        }
                        else {
                            console.log("MediaClockModel::restore(): countdown timer overdue")
                            hmsTime = 0
                            toStart = false
                        }
                        break
                }
                if (toStart) {
                    timer.start()
                }
                break
            case Internal.MediaClockRunMode.MCR_STOPPED:
                hmsTime = magic
                break
        }
        console.debug("MediaClockModel::restore(): exit")
    }
}
