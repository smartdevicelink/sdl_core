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

import QtQuick 2.0
import QtQuick.Controls 1.0
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common
import "../controls"

PopUp {
    width: Constants.popupWidth
    height: Constants.popupHeigth

    property var async
    property int position

    function showSlider(){
        console.debug("SliderPopup.showSlider entered")
        if (timer.running) { // we have alert already
            complete(true)
            console.debug("SliderPopup.showSlider exited")
            return;
        }

        dataContainer.systemSavedContext = dataContainer.systemContext
        dataContainer.systemContext = Common.SystemContext.SYSCTXT_HMI_OBSCURED
        dataContainer.applicationSavedContext = dataContainer.applicationContext

        show()
        timer.start()
        console.debug("SliderPopup.showSlider exited")
    }

    function complete(reason){
        console.debug("SliderPopup.complete entered isAbout = ", isAbort)
        timer.stop()
        dataContainer.systemContext = dataContainer.systemSavedContext
        dataContainer.applicationContext = dataContainer.applicationSavedContext
        dataContainer.uiSlider.running = false
        hide()
        switch(reason) {
        case Common.Result.ABORT:
            console.debug("aborted position is", dataContainer.uiSlider.position)
            DBus.sendError(async, Common.Result.ABORT)//todo(ykzakov): send abort with slider position
            break;
        case Common.Result.SUCCESS:
            console.debug("send position", position)
            dataContainer.uiSlider.position = position
            DBus.sendReply(async, {sliderPosition:position})
            break
        default:
            break
        }
        console.debug("SliderPopup.complete exited")
    }


    Column
    {
        spacing: Constants.generalSpacing
        anchors.centerIn: parent
        Timer {
            id: timer
            interval: dataContainer.uiSlider.timeout
            onTriggered: {
                complete(false)
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
                        rectangle.width = dataContainer.uiSlider.position * tickWidth;
                }
            }


            MouseArea{
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
                        rectangle.width = position * tickWidth;
                    }

                    if(dataContainer.uiSlider.footer.length > 1){
                        footerText.text = dataContainer.uiSlider.footer[position - 1]
                    }
                }
            }
        }

        Text {
            id:footerText
            anchors.horizontalCenter: parent.horizontalCenter
            text: {
                if(dataContainer.uiSlider.footer.length === 0 || dataContainer.uiSlider.position === 0 ) return "";
                return dataContainer.uiSlider.footer.length === 1 ? dataContainer.uiSlider.footer[0] : dataContainer.uiSlider.footer[dataContainer.uiSlider.position - 1]
            }
            color: Constants.sliderTextColor
            font.pixelSize: Constants.fontSize * 2
        }

        OvalButton {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Back"
            fontSize: Constants.fontSize
            onClicked: {
                complete(true)
            }
        }
    }

}
