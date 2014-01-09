/**
 * @file TurnByTurnView.qml
 * @brief View for TurnByTurn.
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

GeneralView {
    applicationContext: true

    Item {
        id: turnByTurnView
        anchors.fill: parent

        Item {
            // top 3/4 of screen
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: parent.height * 3/4

            Item {
                // row of oval buttons
                id: ovalButtonsRow
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width
                height: 1/4 * parent.height

                PagedFlickable {
                    id: buttonsRow
                    width: parent.width
                    spacing: (width - 4 * elementWidth) / 3
                    anchors.verticalCenter: parent.verticalCenter
                    snapTo: Constants.ovalButtonWidth + spacing
                    elementWidth: Constants.ovalButtonWidth

                    OvalButton {
                        id: turnListButton
                        text: "TurnList"
                        onClicked: {
                            console.log("Go to TurnListView");
                            contentLoader.go("./views/TurnListView.qml");
                        }
                    }

                    Repeater {
                        model: dataContainer.navigationModel.softButtons ?
                                   dataContainer.navigationModel.softButtons.count :
                                   0
                        delegate:
                            SoftButton {
                                appId: dataContainer.navigationModel.appId
                                button: dataContainer.navigationModel.softButtons.get(index)
                       }
                    }
                }
            }

            Column {
                // Picture + text information + media clock
                id: mediaContent
                width: parent.width
                height: 3/4 * parent.height
                anchors.left: parent.left
                anchors.top: ovalButtonsRow.bottom

                Row {
                    // picture + text info
                    width: parent.width
                    height: 3/4 * parent.height
                    spacing: Constants.margin

                    Image {
                        id: image
                        height: parent.height
                        width: height
                        source: dataContainer.navigationModel.turnIcon
                    }

                    Column {
                        // text info
                        id: textInfo
                        height: parent.height
                        width: parent.width - image.width - parent.spacing
                        spacing: (height - 5 * navText1.height) / 4

                        Text {
                            id: navText1
                            anchors.left: parent.left
                            anchors.right: parent.right
                            color: Constants.primaryColor
                            text: dataContainer.navigationModel.text1
                            font.pixelSize: Constants.fontSize
                            visible: dataContainer.navigationModel.text1
                        }

                        Text {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            color: Constants.primaryColor
                            text: dataContainer.navigationModel.text2
                            font.pixelSize: Constants.fontSize
                            visible: dataContainer.navigationModel.text2
                        }

                        Text {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            color: Constants.primaryColor
                            text: "Total distance: " + dataContainer.navigationModel.totalDistance
                            font.pixelSize: Constants.fontSize
                            visible: dataContainer.navigationModel.totalDistance
                        }

                        Text {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            color: Constants.primaryColor
                            text: "Time to destination: " + dataContainer.navigationModel.timeToDestination
                            font.pixelSize: Constants.fontSize
                            visible: dataContainer.navigationModel.timeToDestination
                        }

                        Text {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            color: Constants.primaryColor
                            text: "ETA: " + dataContainer.navigationModel.eta
                            font.pixelSize: Constants.fontSize
                            visible: dataContainer.navigationModel.eta
                        }
                    }
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 1/4 * parent.height
                    width: 2/3 * parent.width

                    Rectangle {
                       id: coveredDistance
                       anchors.verticalCenter: parent.verticalCenter
                       height: Constants.distanceBarHeight
                       width: dataContainer.navigationModel.distanceToManeuver / dataContainer.navigationModel.distanceToManeuverScale * parent.width
                       color: "white"
                    }

                    Rectangle {
                       anchors.verticalCenter: parent.verticalCenter
                       height: Constants.distanceBarHeight
                       width: parent.width - coveredDistance.width
                       color: Constants.primaryColor
                    }
                }
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
}
