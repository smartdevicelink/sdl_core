/**
 * @file ExitAllApplicationsPopup.qml
 * @brief Popup "Exit all applications"
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
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common
import "../controls"

ContextPopup {
    property variant async
    property int position: 1
    /**
      * Test Support Section
      */
    property int resultCode: -1
    signal onReady
    /**
      * Test Support Section End
      */

    function showSlider(){
        console.debug("enter")

        dataContainer.uiSlider.running = true
        dataContainer.applicationSavedContext = dataContainer.applicationContext
        if(dataContainer.uiSlider.footer.length === 0 || dataContainer.uiSlider.position === 0 ) {
            footerText.text = ""
        } else {
            footerText.text = dataContainer.uiSlider.footer.length === 1 ? dataContainer.uiSlider.footer[0] : dataContainer.uiSlider.footer[dataContainer.uiSlider.position - 1]
        }
        position = dataContainer.uiSlider.position
        show()
        timer.start()
        onReady()
        console.debug("exit")
    }

    function complete(reason){
        console.debug("enter reason = ", reason)
        timer.stop()
        dataContainer.applicationContext = dataContainer.applicationSavedContext
        dataContainer.uiSlider.running = false
        switch(reason) {
        case Common.Result.ABORTED:
            console.debug("aborted position is", position)
            resultCode = Common.Result.ABORTED
            DBus.sendReply(async, {__retCode: resultCode, sliderPosition: position})
            break
        case Common.Result.SUCCESS:
            console.debug("send position", position)
            resultCode = Common.Result.SUCCESS
            dataContainer.uiSlider.position = position
            DBus.sendReply(async, {sliderPosition:position})
            break
        default:
            break
        }
        hide()
        position = 1
        console.debug("exit")
    }


    Column
    {
        spacing: Constants.generalSpacing
        anchors.centerIn: parent
        Timer {
            id: timer
            interval: dataContainer.uiSlider.timeout
            onTriggered: {
                console.debug("triggered")
                complete(Common.Result.SUCCESS)
            }
        }

        Text {
            id: appNameText
            anchors.horizontalCenter: parent.horizontalCenter
            text: dataContainer.uiSlider.appName
            color: Constants.primaryColor
            font.pixelSize: Constants.fontSize * 2
        }

        Text {
            id: headerText
            width: borderRectangle.width
            elide: Text.ElideRight
            anchors.horizontalCenter: parent.horizontalCenter
            text: dataContainer.uiSlider.header
            color: Constants.sliderTextColor
            font.pixelSize: Constants.fontSize * 2
        }

        Rectangle {
            id: borderRectangle
            width: Constants.sliderBarWidth
            height: Constants.sliderBarHeight
            anchors.horizontalCenter: parent.horizontalCenter
            border.color: Constants.sliderBarBorderColor
            border.width: Constants.sliderBarBorderWidth
            radius: Constants.sliderBarRadius
            color: "black"

            Rectangle {
                id: rectangle
                color: Constants.sliderBarFillColor
                height: parent.height
                border.color: parent.border.color
                border.width: parent.border.width
                radius: parent.radius

                onVisibleChanged: {
                        var tickWidth = borderRectangle.width / dataContainer.uiSlider.numTicks
                        rectangle.width = dataContainer.uiSlider.position * tickWidth
                }
            }

            MouseArea{
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    onPositionChanged(mouse)
                }

                onPositionChanged: {
                    if(mouseX <= 0){
                        rectangle.width  = borderRectangle.width / dataContainer.uiSlider.numTicks
                        position = 1
                    }

                    if(mouseX > borderRectangle.width) {
                        rectangle.width  = borderRectangle.width
                        position = dataContainer.uiSlider.numTicks
                    }

                    if(mouseX > 0 && mouseX < borderRectangle.width) {
                        var tickWidth = borderRectangle.width / dataContainer.uiSlider.numTicks
                        position = Math.ceil(mouseX / tickWidth)
                        rectangle.width = position * tickWidth
                    }

                    if(dataContainer.uiSlider.footer.length > 1){
                        footerText.text = dataContainer.uiSlider.footer[position - 1]
                    }
                }
            }
        }

        Text {
            id:footerText
            width: borderRectangle.width
            elide: Text.ElideRight
            anchors.horizontalCenter: parent.horizontalCenter
            text: ""
            color: Constants.sliderTextColor
            font.pixelSize: Constants.fontSize * 2
        }

        OvalButton {
            text: "Close"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                console.debug("enter")
                complete(Common.Result.ABORTED)
                console.debug("exit")
            }
        }
    }

    function getTimer() {
        return timer
    }
    function getBackButton() {
        return backButton
    }
    function getFooterText() {
        return footerText
    }
    function getBorderRectangle() {
        return borderRectangle
    }
}
