/**
 * @file ContactsListView.qml
 * @brief Contact list screen view.
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
    function setCurrent(firstLetter){
        for(var i = 0; i < contactsListView.count; i ++) {
            if (contactsListView.model.get(i).name[0].toUpperCase() === firstLetter) {
                contactsListView.positionViewAtIndex(i, ListView.Beginning)
                break;
            }
        }
    }
    Component.onCompleted: setCurrent(dataContainer.contactsFirstLetter)

    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        ListView {
            id: contactsListView
            anchors.fill: parent
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            clip: true
            model: ContactsListModel {}
            spacing: 25
            section.property: "name"
            section.criteria: ViewSection.FirstCharacter
            section.delegate: Text {
                color: Constants.primaryColor
                font.pixelSize: Constants.fontSize
                text: section.toUpperCase()
            }

            delegate: Item {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                height: typeText.height

                Text  {
                    text: name
                    color: Constants.contactTextColor
                    font.pixelSize: Constants.fontSize
                    anchors.left: parent.left
                }
                Text  {
                    id: phoneText
                    text: phone
                    color: Constants.contactTextColor
                    font.pixelSize: Constants.fontSize
                    anchors.left: parent.horizontalCenter
                }
                Text  {
                    id: typeText
                    text: type
                    color: Constants.contactTextColor
                    font.pixelSize: Constants.fontSize
                    anchors.right: parent.right
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

        BackButton { anchors.centerIn: parent}
    }
}
