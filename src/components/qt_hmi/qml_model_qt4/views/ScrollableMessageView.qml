/**
 * @file ScrollableMessageView.qml
 * @brief Scrollable Message View .
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
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants
import "../models"
import "../popups"

GeneralView {
    applicationContext: true
    id: scrollableMessageView

    function complete(abort){
        console.debug("enter")
        timer.stop()
        dataContainer.scrollableMessageModel.running = false

        dataContainer.popups--
        dataContainer.applicationContext = dataContainer.applicationSavedContext

        if(abort) {
            console.debug("send error")
            DBus.sendError(dataContainer.scrollableMessageModel.async, Common.Result.ABORTED)
        } else {
            console.debug("send ok")
            DBus.sendReply(dataContainer.scrollableMessageModel.async, {})
        }

        contentLoader.back()
        console.debug("exit")
    }


    Connections {
        target: contentLoader
        onStatusChanged: {
            if (status == Component.Ready) {
                console.debug("enter")
                console.debug("timeout in timer", timer.interval)
                dataContainer.popups++
                dataContainer.applicationSavedContext = dataContainer.applicationContext
                dataContainer.scrollableMessageModel.running = true
                timer.start()
                console.debug("exit")
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Constants.secondaryColor

        Timer {
            id: timer
            interval: dataContainer.scrollableMessageModel.timeout
            onTriggered: {
                complete(false)
            }
        }

        Item {
            // top 1/6 of screen
            id: top
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: dataContainer.scrollableMessageModel.softButtons.count > 0 ? parent.height * 1/6 : 1

            OvalButton {
                //this button is used only to get item size for PagedFlickable
                id: fakeButton
                text: "defaultBtnSize"//remove this line when default size for btn will be introduced
                visible:false
            }

            PagedFlickable {
                id: flickRow
                width: top.width
                spacing: (width - fakeButton.width * 4) / 3
                snapTo: fakeButton.width + spacing
                elementWidth: fakeButton.width


                ListView {
                    id: softButtons
                    width: model.count * fakeButton.width + (model.count - 1) * flickRow.spacing
                    height: fakeButton.height
                    spacing: flickRow.spacing

                    orientation: ListView.Horizontal
                    interactive: false
                    model: dataContainer.scrollableMessageModel.softButtons

                    delegate: SoftButton {
                        appId: dataContainer.currentApplication.appId
                        button: model
                        onKeepContext: timer.restart()
                        onDefaultAction: scrollableMessageView.complete(true)
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
            // 1/4 bottom screen
            id: bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: 1/4 * parent.height

            BackButton {
                id:backButton
                anchors.centerIn: parent
                onClicked: {
                    console.debug("enter")
                    complete(true)
                    console.debug("exit")
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
