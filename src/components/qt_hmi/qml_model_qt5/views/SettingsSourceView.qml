/**
 * @file SettingsSourceView.qml
 * @brief Settings source screen view.
 * Copyright (c) 2014, Ford Motor Company
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
import "../models"
import "../controls"
import "../models/Constants.js" as Constants

GeneralView {
    signal itemActivated(string item)

    onItemActivated: {
        switch (item) {
            case "update_sdl": RequestToSDL.SDL_UpdateSDL(settingsContainer.updateStatus); break;
            case "get_status_update": RequestToSDL.SDL_GetStatusUpdate(settingsContainer.updateStatus); break;
            case "get_urls": RequestToSDL.SDL_GetURLS(0, settingsContainer.startPTExchange); break;
        }
    }

    Item {
        anchors.fill: parent
        ScrollableListView {
            id: menu
            model: dataContainer.settingsSourceModel
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: bottomPanel.top
            delegate: OvalButton {
                text: title
                onReleased: {
                    if (qml) {
                        contentLoader.go(qml, appId);
                    } else {
                        itemActivated(action);
                    }
                }
                anchors.left: parent.left
                anchors.right: parent.right
                fontSize: Constants.fontSize
            }
        }

        Item {
            id: bottomPanel
            // 1/4 bottom screen
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            height: 1/4 * parent.height
            width: parent.width

            BackButton { anchors.centerIn: parent }
        }
    }
}



