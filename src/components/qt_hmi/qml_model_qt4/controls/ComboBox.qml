/**
 * @file ComboBox.qml
 * @brief Combo box
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

Item {
    id: container
    width: Constants.defaultComboboxWidth
    height: main.height

    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    property alias currentText: textMain.text
    property string textRole

    function choose(name) {
        var count = listView.count
        for (var i = 0; i < count; ++i) {
            if (name === listView.model.get(i).name) {
                listView.currentIndex = i
                break
            }
        }

        textMain.text = name
    }

    Rectangle {
        id: main

        anchors.left: parent.left
        anchors.right: parent.right
        height: label.font.pixelSize + Constants.generalSpacing
        color: Constants.panelTextColor
        border.color: Constants.controlsBorderColor
        border.width: Constants.controlsBorderWidth
        radius: Constants.controlsRadius

        Text {
            id: textMain
            anchors.left: parent.left
            anchors.right: arrow.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: Constants.secondaryColor
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: Constants.generalSpacing
            anchors.rightMargin: Constants.generalSpacing
            elide: Text.ElideRight
        }

        Rectangle {
            id: arrow
            anchors.right: parent.right
            height: parent.height; width: height

            color: Constants.panelTextColor
            border.color: Constants.controlsBorderColor
            border.width: Constants.controlsBorderWidth
            radius: Constants.controlsRadius

            Text {
                anchors.fill: parent
                text: "v"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: Constants.secondaryColor
            }
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: listView.visible = !listView.visible
        }
    }

    ScrollableListView {
        id: listView
        anchors.top: main.bottom
        anchors.left: main.left
        anchors.right: main.right
        height: 150
        visible: false
        delegate: Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            height: label.font.pixelSize + Constants.generalSpacing
            color: Constants.panelTextColor
            border.color: Constants.controlsBorderColor
            border.width: Constants.controlsBorderWidth
            Text {
                id: label
                anchors.fill: parent
                anchors.leftMargin: Constants.generalSpacing
                anchors.rightMargin: Constants.generalSpacing
                verticalAlignment: Text.AlignVCenter
                text: model.name
                color: Constants.secondaryColor
                elide: Text.ElideRight
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: { choose(model.name); listView.visible = false }
                onEntered: { parent.color = Constants.primaryColorPressed }
                onExited: { parent.color = Constants.panelTextColor }
            }
        }
    }
}
