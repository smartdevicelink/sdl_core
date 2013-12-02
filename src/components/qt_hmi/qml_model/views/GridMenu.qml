/**
 * @file GridMenu.qml
 * @brief Parent class for main menu.
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
import "../controls"
import "../models/Constants.js" as Constants
import "../models/Internal.js" as Internal

GeneralView {
    id: main
    property alias model: repeater.model
    property alias delegate: repeater.delegate
    property alias rows: grid.rows
    property int columnsOnPage: Constants.menuColumnCount

    Flickable {
        id: flicker
        anchors.fill: parent
        contentWidth: grid.width
        flickableDirection: Flickable.HorizontalFlick

        Grid {
            id: grid
            anchors.centerIn: parent
            rows: Constants.menuRowCount
            columns: Math.ceil(model.count / rows)
            flow: Grid.TopToBottom
            Repeater {
                id: repeater
            }
        }

        property int snapTo: width / parent.columnsOnPage
        onMovementEnded: {
            var rest = flicker.contentX % snapTo
            var time = 0.25
            if (rest > flicker.snapTo / 2) { rest = rest - flicker.snapTo }
            var vel = 2 * rest / time
            flickDeceleration = Math.abs(vel) / time
            flick(vel, 0)
            flickDeceleration = 1500
        }
    }

    Pager {
        id: pager
        space: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: Constants.margin

        pages: Math.ceil(grid.columns / parent.columnsOnPage)
        activePage: Internal.activePageChoose(flicker, pager.pages)
    }
}
