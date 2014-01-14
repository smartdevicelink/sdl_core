/**
 * @file ChangeDeviceView.qml
 * @brief Screen view with list of available devices.
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
import "../models"
import "../models/Constants.js" as Constants

GeneralView {
    Item {
        anchors.fill: parent

        Item {
            // 3/4 top screen
            height: parent.height * 3/4
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left

            Text {
                id: searchDeviceText
                anchors.left: parent.left
                anchors.top: parent.top
                height: 1/4 * parent.height
                width: parent.width                
                color: Constants.primaryColor
                font.pixelSize: Constants.titleFontSize
                text: "Devices available:"
                verticalAlignment: Text.AlignVCenter
            }

            ListView {
                id: changeDeviceListView
                anchors.left: parent.left
                anchors.top: searchDeviceText.bottom
                model: dataContainer.deviceList
                width:parent.width
                height:parent.height - searchDeviceText.height

                delegate: Text  {
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            sdlBasicCommunication.onDeviceChosen({ name: name, id: devid });
                            sdlBasicCommunication.onFindApplications({ name: name, id: devid })
                            contentLoader.go("./views/ApplicationListView.qml")
                        }
                    }
                    text: name
                    color: Constants.primaryColor
                    font.pixelSize: Constants.titleFontSize
                }
            }
        }
        Item {
            // 1/4 bottom screen
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: 1/4 * parent.height

            BackButton { anchors.centerIn: parent }
        }

        Component.onCompleted: {
            console.log("ChangeDeviceView Component.onCompleted enter");
            sdlBasicCommunication.onStartDeviceDiscovery();
            console.log("ChangeDeviceView Component.onCompleted exit");
        }
    }
}
