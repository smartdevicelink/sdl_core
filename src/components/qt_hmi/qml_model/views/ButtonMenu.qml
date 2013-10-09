/**
 * @file InternalMenu.qml
 * @brief Parent class for internal menus.
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

GeneralView {
    id: menuView
    property ListModel listModel
    property int countOfUpperRowItems: Math.ceil(listModel.count / 2)

    // top 3/4 of screen with flick menu
    Flickable {
        id: flicker
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        contentWidth: flickItem.width

        Item {
            id: flickItem
            width: upperRow.width
            height: parent.height
            anchors.centerIn: parent

            Row {
                id: upperRow
                anchors.top: parent.top

                Repeater {
                    model: menuView.countOfUpperRowItems
                    delegate: Item {
                        id: item
                        width: flicker.width / 3
                        height: flicker.height / 2
                        opacity: 0

                        LongOvalButton {
                            id: button
                            text: menuView.listModel.get(index).title
                            pixelSize: 20
                            dest: menuView.listModel.get(index).qml
                            anchors.centerIn: parent
                        }

                        SequentialAnimation {
                            id: upRowAnimation
                            PauseAnimation {duration: index * 100 }
                            NumberAnimation {
                                target: item
                                duration:  500
                                property: "opacity"
                                from: 0; to: 1;
                            }
                        }
                        Component.onCompleted: {
                            upRowAnimation.start()
                        }
                    }
                }
            }

            Row {
                id: lowerRow
                anchors.bottom: parent.bottom

                Repeater {
                    model: menuView.countOfUpperRowItems
                    delegate: Item {
                        id: item2
                        width: flicker.width / 3
                        height: flicker.height / 2
                        opacity: 0

                        LongOvalButton {
                            id: buttonLowerRow
                            text: menuView.listModel.get(index + menuView.countOfUpperRowItems).title
                            pixelSize: 20
                            dest: menuView.listModel.get(index + menuView.countOfUpperRowItems).qml
                            anchors.centerIn: parent
                        }
                        SequentialAnimation {
                            id: lowRowAnimation
                            PauseAnimation {duration: 300 + index * 100 }
                            NumberAnimation {
                                target: item2
                                duration:  500
                                property: "opacity"
                                from: 0; to: 1;
                            }
                        }
                        Component.onCompleted: {
                            lowRowAnimation.start()
                        }
                    }
                }
            }
        }

        property int snapTo: width / 3
        onMovementEnded: {
            var rest = flicker.contentX % snapTo
            var time = 0.25
            if (rest > flicker.snapTo / 2) { rest = rest - flicker.snapTo }
            var vel = 2 * rest / time
            flickDeceleration = Math.abs(vel) / time
            flick(vel, 0)
            flickDeceleration = 1500
        }
}

    Pager {
        id: pager
        space: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top

        pages: Math.ceil(menuView.countOfUpperRowItems / itemsInRowOnScreen) // 3 items in a row on 1 screen
        activePage: Math.round(pages * (flicker.contentX / flicker.contentWidth + 0.005))
    }

    Item {
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        BackButton {
            id: backButton
            anchors.centerIn: parent
            opacity: 0

            SequentialAnimation {
                id: backButtonAnimation
                PauseAnimation {duration: 700}
                NumberAnimation {
                    target: backButton
                    duration:  500
                    property: "opacity"
                    from: 0; to: 1;
                }
            }

            Component.onCompleted: {
                backButtonAnimation.start()
            }
        }
    }


}
