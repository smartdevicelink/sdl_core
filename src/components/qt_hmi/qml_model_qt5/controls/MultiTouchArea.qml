/**
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
import "../hmi_api/Common.js" as Common

MultiPointTouchArea {
    property date created

    minimumTouchPoints: 1
    maximumTouchPoints: 10

    MouseArea {
        anchors.fill: parent

        function mouseTouchEvent() {
            var now = new Date()
            var touchEvents = [
                {
                    id: 0,
                    ts: [now.valueOf() - created.valueOf()],
                    c: [{x: mouseX, y: mouseY}]
                }
            ]
            return touchEvents
        }

        onPressed: {
            sdlUI.onTouchEvent(Common.TouchType.BEGIN, mouseTouchEvent())
        }
        onReleased: {
            sdlUI.onTouchEvent(Common.TouchType.END, mouseTouchEvent())
        }
        onCanceled: {
            sdlUI.onTouchEvent(Common.TouchType.END, mouseTouchEvent())
        }
        onPositionChanged: {
            sdlUI.onTouchEvent(Common.TouchType.MOVE, mouseTouchEvent())
        }
    }

    function touchEvents() {
        var now = new Date()
        var touchEvents = []
        for (var i = 0; i < touchPoints.length; ++i) {
            touchEvents.push(
                {
// pointId is guaranteed to be unique but is not guaranteed to fit in range
// perhaps we will have to edit protocol xml
                    id: touchPoints[i].pointId,
                    ts: [now.valueOf() - created.valueOf()],
                    c: [{x: touchPoints[i].x, y: touchPoints[i].y}]
                }
            )
        }
        return touchEvents
    }

    onPressed: {
        sdlUI.onTouchEvent(Common.TouchType.BEGIN, touchEvents())
    }
    onReleased: {
        sdlUI.onTouchEvent(Common.TouchType.END, touchEvents())
    }
    onCanceled: {
        sdlUI.onTouchEvent(Common.TouchType.END, touchEvents())
    }
    onUpdated: {
        sdlUI.onTouchEvent(Common.TouchType.MOVE, touchEvents())
    }
    Component.onCompleted: {
        created = new Date()
    }
}
