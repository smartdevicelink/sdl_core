/**
 * @file WarningInfo.qml
 * @brief Screen with warning information.
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
import "../models/Constants.js" as Constants

Rectangle {
    id: warningInfo
    anchors.fill: parent
    color: Constants.secondaryColor
    Text {
        anchors.top: parent.top
        anchors.left: parent.left
        height: 1/4 * parent.height
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: "WARNING!"
        color: Constants.warningColor
        font.pixelSize: Constants.titleFontSize
        font.bold: true
        style: Text.Raised
        styleColor: "gray"
    }

    Text {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Constants.margin
        height: 1/2 * parent.height
        width: parent.width
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap

        text: "This application is intended to be used as a demonstrative aid only, "
            + "while the vehicle is parked. Don't use it while driving. <br /><br />"
            + "Use extreme caution when using any device that takes your attention off the road. "
            + "Ford recommends against the use of any hand-held device while driving and that you comply "
            + "with all applicable laws. Your primary responsibility is the safe operation of the vehicle."
        color: "white"
        font.pixelSize: 25
        font.bold: true
    }

    function showOkButton() {
        okButtonAnimation.start()
    }

    Item {
        id: ok
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height
        opacity: 0

        SequentialAnimation
        {
            id: okButtonAnimation
            // Decrease this value while debugging
            PauseAnimation { duration: Constants.warningScreenDuration }
            NumberAnimation {
                target: ok
                duration:  Constants.animationDuration
                property: "opacity"
                from: 0; to: 1;
            }
        }

        Image {
            id: okButton
            anchors.centerIn: parent
            source: "../res/buttons/longest_oval_btn.png"

            Text {
                id: text
                anchors.centerIn: parent
                text: "OK"
                font.pixelSize: Constants.fontSize
                color: Constants.primaryColor
            }

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    parent.source = "../res/buttons/longest_oval_btn_pressed.png"
                    text.color = Constants.secondaryColor
                }
                onReleased:  {
                    parent.source = "../res/buttons/longest_oval_btn.png"
                    text.color = Constants.primaryColor
                }
                onClicked: {
                    console.log("WarningInfo OkButton - on clicked enter");
                    warningInfo.visible = false
                }
            }
        }
    }
}
