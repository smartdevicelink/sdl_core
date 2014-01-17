/**
 * @file SDLPlayerOptionsListView.qml
 * @brief SDL player options screen view.
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
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants
import "../models/Internal.js" as Internal

GeneralView {
    applicationContext: true
    systemContext: Common.SystemContext.SYSCTXT_MENU
    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        ListView {
            id: sdlPlayerOptionsListView
            anchors.horizontalCenter: parent.horizontalCenter
            width:parent.width
            height:parent.height

            model: dataContainer.currentApplication.currentSubMenu

            delegate: Row {
                spacing: Constants.iconItemListSpacing
                Icon {
                    source: model.icon
                    width: Constants.iconItemListSize
                    height: Constants.iconItemListSize

                    MouseArea {
                        anchors.fill: parent
                        enabled: model.icon.value !== undefined

                        onPressed: text.press()
                        onReleased: text.release()
                        onCanceled: text.release()
                        onClicked: text.click()
                    }
                }

                ClickableText {
                    id: text
                    text: name + (type === Internal.MenuItemType.MI_SUBMENU ? " >" : "")
                    width: sdlPlayerOptionsListView.width - x
                    elide: Text.ElideRight
                    defaultColor: type === Internal.MenuItemType.MI_PARENT ?
                                      Constants.inactiveButtonTextColor :
                                      Constants.primaryColor
                    pressedColor: type === Internal.MenuItemType.MI_PARENT ?
                                      Constants.inactiveButtonTextColorPressed :
                                      Constants.primaryColorPressed
                    font.pixelSize: Constants.titleFontSize

                    function click() {
                        console.debug("enter")
                        switch (type) {
                            case Internal.MenuItemType.MI_NODE:
                                sdlUI.onCommand(model.id, dataContainer.currentApplication.appId)
                                contentLoader.back()
                                break;
                            case Internal.MenuItemType.MI_SUBMENU:
                            case Internal.MenuItemType.MI_PARENT:
                                dataContainer.currentApplication.currentSubMenu = subMenu
                                break;
                        }
                        console.debug("exit")
                    }
                    onClicked: click()
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
