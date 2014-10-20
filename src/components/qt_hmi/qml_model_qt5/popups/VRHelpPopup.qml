/**
 * @file VRHelpPopup.qml
 * @brief Popup view for VR help
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
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common

PopUp {
    property alias title: title.text
    Text {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Constants.popupMargin
        text: {
            // not in application
            if (!dataContainer.applicationContext) {
                return dataContainer.currentApplication.vrHelpTitleDefault
            // in application
            } else {
                if (interactionPopup.performInteractionIsActiveNow) {
                    return dataContainer.currentApplication.vrHelpTitlePerformInteraction
                } else if (dataContainer.currentApplication.vrHelpTitle) {
                    return dataContainer.currentApplication.vrHelpTitle
                } else {
                    return dataContainer.currentApplication.vrHelpTitleDefault
                }
            }
        }
        font.pixelSize: Constants.titleFontSize
        color: Constants.primaryColor
    }

    ScrollableListView {
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Constants.popupMargin

        model: {            
            // not in application
            if (!dataContainer.applicationContext) {
                return dataContainer.currentApplication.vrHelpItemsDefault
            // in application
            } else {
                if (interactionPopup.performInteractionIsActiveNow) {
                    return dataContainer.currentApplication.vrHelpItemsPerformInteraction
                } else if (dataContainer.currentApplication.vrHelpItems.count > 0) {
                    return dataContainer.currentApplication.vrHelpItems
                } else {
                    return dataContainer.currentApplication.vrHelpItemsDefault
                }
            }
        }

        delegate:
            Row {
            spacing: Constants.iconItemListSpacing
            Icon {
                source: model.image
                anchors.verticalCenter: parent.verticalCenter
                width: Constants.iconItemListSize
                height: Constants.iconItemListSize
            }

            Text {
                id: text
                anchors.verticalCenter: parent.verticalCenter
                text: model.text
                color: Constants.primaryColor
                font.pixelSize: Constants.fontSize
            }
        }
    }

    function complete(reason, data) {
        hide()
    }
}
