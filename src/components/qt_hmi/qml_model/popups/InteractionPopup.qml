/**
 * @file InteractionPopup.qml
 * @brief Interaction popup view.
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
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants

PopUp {
    Text {
        id: initialText
        text: dataContainer.interactionModel.initialText
        anchors.top: parent.top
        anchors.topMargin: Constants.popupMargin
        anchors.left: parent.left
        anchors.leftMargin: Constants.popupMargin
        font.pixelSize: Constants.fontSize
        color: Constants.primaryColor
    }

    ListView {
        anchors.top: initialText.bottom
        anchors.topMargin: Constants.popupMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Constants.popupMargin
        anchors.left: parent.left
        anchors.leftMargin: Constants.popupMargin
        anchors.right: parent.right
        anchors.rightMargin: Constants.popupMargin
        model: dataContainer.interactionModel.choice
        delegate: OvalButton {
            width: parent.width
            text: name
            onClicked: {
                timer.stop()
                DBus.sendReply(dataContainer.interactionModel.async, {"choiceID": id})
                deactivate()
            }
        }
    }

    Item {
        Timer {
            id: timer
            onTriggered: {
                DBus.sendError(dataContainer.interactionModel.async, Common.Result.TIMED_OUT)
                deactivate()
            }
        }
    }

    function activate () {
        console.debug("InteractionPopup::activate()")
        dataContainer.systemSavedContext = dataContainer.systemContext
        dataContainer.systemContext = Common.SystemContext.SYSCTXT_HMI_OBSCURED
        timer.interval = dataContainer.interactionModel.timeout
        timer.start()
        show()
        console.debug("InteractionPopup::activate(): exit")
    }

    function deactivate () {
        console.debug("InteractionPopup::deactivate()")
        dataContainer.systemContext = dataContainer.systemSavedContext
        hide()
        console.debug("InteractionPopup::deactivate(): exit")
    }
}
