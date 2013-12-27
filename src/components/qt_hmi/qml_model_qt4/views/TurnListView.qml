/**
 * @file TurnListView.qml
 * @brief View for TurnByTurn list.
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
import "../hmi_api/Common.js" as Common

GeneralView {
    applicationContext: true

    Item {
        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.width
        height: 3/4 * parent.height

        Item {
            id: sotfButtons
            width: parent.width
            height: 1/4 * parent.height

            PagedFlickable {
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                elementWidth: Constants.ovalButtonWidth
                snapTo: elementWidth + spacing
                spacing: (width - 4 * elementWidth) / 3

                Repeater {
                    model: dataContainer.currentApplication.turnListSoftButtons ?
                               dataContainer.currentApplication.turnListSoftButtons.count :
                               0
                    delegate:
                        SoftButton {
                            appId: dataContainer.currentApplication.appId
                            button: dataContainer.currentApplication.turnListSoftButtons.get(index)
                        }
                }
            }
        }

        ScrollableListView {
            id: scrollableList
            anchors.top: sotfButtons.bottom
            anchors.left: parent.left
            width: parent.width
            height: 3/4 * parent.height

            model: dataContainer.currentApplication.turnList

            delegate:
                ListItem {
                    width: scrollableList.width
                    height: Constants.iconItemListSize
                    text: dataContainer.currentApplication.turnList.get(index).navigationText.fieldText
                    fontSize: Constants.fontSize
                    icon: dataContainer.currentApplication.turnList.get(index).turnIcon
            }
        }
    }

    Item {
        // 1/4 bottom screen
        id: back
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height
        BackButton { anchors.centerIn: parent }
    }
}
