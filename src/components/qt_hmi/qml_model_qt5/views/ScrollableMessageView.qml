/**
 * @file ScrollableMessageView.qml
 * @brief Scrollable Message View.
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
import "../hmi_api/Async.js" as Async
import "../models/Constants.js" as Constants
import "../models"
import "../popups"

GeneralView {
    applicationContext: true
    systemContext: Common.SystemContext.SYSCTXT_HMI_OBSCURED
    onLeaveScreen: {
        timer.stop()
        dataContainer.scrollableMessageModel.running = false
        DBus.sendReply(dataContainer.scrollableMessageModel.async, { __retCode: dataContainer.scrollableMessageModel.result })
    }
    Component.onCompleted: {
        dataContainer.scrollableMessageModel.result = Common.Result.ABORTED
        dataContainer.scrollableMessageModel.running = true
        timer.start()
    }

    Timer {
        id: timer
        interval: dataContainer.scrollableMessageModel.timeout
        onTriggered: {
            dataContainer.scrollableMessageModel.result = Common.Result.SUCCESS
            contentLoader.back()
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Constants.secondaryColor

        Item {
            // top 1/5 of screen
            id: top
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: dataContainer.scrollableMessageModel.softButtons.count > 0 ? parent.height * 1/5 : 1

            PagedFlickable {
                id: flickRow
                width: top.width
                spacing: (width - elementWidth * 4) / 3
                snapTo: Constants.ovalButtonWidth + spacing
                elementWidth: Constants.ovalButtonWidth
                Repeater {
                    model: dataContainer.scrollableMessageModel.softButtons ?
                               dataContainer.scrollableMessageModel.softButtons.count :
                               0
                    delegate:
                        SoftButton {
                            appId: dataContainer.scrollableMessageModel.appId
                            button: dataContainer.scrollableMessageModel.softButtons.get(index)
                            onKeepContext: { timer.restart() }
                            onDefaultAction: {
                                dataContainer.scrollableMessageModel.result = Common.Result.ABORTED
                                contentLoader.back()
                            }
                            onStealFocus: {
                                dataContainer.scrollableMessageModel.result = Common.Result.SUCCESS
                                contentLoader.back()
                            }
                        }
                }
            }
        }

        Item {
            id: middle
            anchors.top: top.bottom
            anchors.left: parent.left
            anchors.bottom: bottom.top
            width: parent.width
            clip: true

            Flickable {
                id:flickable
                anchors.fill: middle
                flickableDirection: Flickable.VerticalFlick
                contentWidth: longMessageText.width
                contentHeight: longMessageText.height
                clip: true

                Text {
                    id: longMessageText
                    wrapMode: Text.Wrap
                    width: middle.width
                    text: dataContainer.scrollableMessageModel.longMessageText
                    color: Constants.scrollableMessageTextColor
                    font.pixelSize: Constants.fontSize
                }
            }
            Rectangle {
                id: scrollbar
                anchors.right: flickable.right
                y: flickable.visibleArea.yPosition * flickable.height
                width: Constants.scrollableMessageScrollBarWidth
                height: flickable.visibleArea.heightRatio * flickable.height
                visible: !(flickable.visibleArea.heightRatio > 1)
                color: Constants.scrollableMessageScrollBarColor
            }
        }
        Item {
            // 1/5 bottom screen
            id: bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: 1/5 * parent.height
            OvalButton {
                anchors.centerIn: parent
                text: "Close"
                onClicked: {
                    dataContainer.scrollableMessageModel.result = Common.Result.ABORTED
                    contentLoader.back()
                }
            }
        }
    }
    /**
      * Test Support Section
      *
      */
    function getTimer(){
        return timer
    }
    function getSoftButtonsListView(){
        return softButtons
    }
    function getScrollbar(){
        return scrollbar
    }
    function getTextArea(){
        return middle
    }
    function getBackButton(){
        return backButton
    }

    /**
      * Test Support Section End
      */
}
