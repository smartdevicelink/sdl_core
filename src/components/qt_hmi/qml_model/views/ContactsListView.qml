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

        ScrollableListView {
            id: contactsListView
            anchors.fill: parent
            clip: true
            spacing: 1/2 * Constants.fontSize
            model: ContactsListModel {  }

            section.property: "name"
            section.criteria: ViewSection.FirstCharacter
            section.delegate: Text {
                color: Constants.primaryColor
                font.pixelSize: Constants.fontSize
                text: section.toUpperCase()
            }

            delegate: Item {
                anchors.left: parent.left
                width: parent.width - Constants.scrollBarWidth
                height: typeText.height

                Text  {
                    id: contactName
                    anchors.left: parent.left
                    text: name
                    width: Constants.contactListNameFieldSize * parent.width
                    color: Constants.contactTextColor
                    font.pixelSize: Constants.fontSize
                    elide: Text.ElideRight
                }
                Text  {
                    id: phoneText
                    anchors.left: contactName.right
                    width: Constants.contactListPhoneFieldSize * parent.width
                    text: phone
                    color: Constants.contactTextColor
                    font.pixelSize: Constants.fontSize
                    horizontalAlignment: Text.AlignHCenter
                }
                Text  {
                    id: typeText
                    anchors.right: parent.right
                    width: Constants.contactListDescriptionFieldSize * parent.width
                    text: type
                    color: Constants.contactTextColor
                    font.pixelSize: Constants.fontSize
                    horizontalAlignment: Text.AlignHCenter
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
