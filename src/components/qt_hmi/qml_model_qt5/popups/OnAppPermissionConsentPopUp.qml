/**
 * @file OnAppPermissionConsent.qml
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
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import "../models"
import "../controls"
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants

PopUp {

    width: Constants.popupWidth - 200
    height: Constants.popupHeigth - 200

    property int appID
    property ListModel permissionItems: ListModel{}

    function activate(appId) {
        console.debug("onAppPermissionConsentPopUp activate enter");
        appID = appId
        show()
        console.debug("onAppPermissionConsentPopUp activate exit");
    }

    function getFunctionalID(messageCode) {

        console.debug(appID);

        var app = dataContainer.getApplication(appID);

        for (var i = 0; i < app.allowedFunctions.length; i++) {
            if (messageCode == app.allowedFunctions[i].name) {

                return app.allowedFunctions[i].id;
            }
        }
    }

    function deactivate() {
        console.debug("onAppPermissionConsentPopUp deactivate enter");
        hide()

        var consentedFunctions = [];

        for (var i = 0; i < permissionItems.count; i++) {
            consentedFunctions.push({
                                        "name": permissionItems.get(i).messageCode,
                                        "id": getFunctionalID(permissionItems.get(i).messageCode),
                                        "allowed": permissionItems.get(i).allowed
                                    })

            console.debug("consentedFunctions.push", consentedFunctions[i].name,consentedFunctions[i].id,consentedFunctions[i].allowed);
        }

        sdlSDL.onAppPermissionConsent(appID, consentedFunctions, Common.ConsentSource.GUI);
        console.debug("onAppPermissionConsentPopUp deactivate exit");
    }

     Column {
        anchors.fill: parent

        Component {
            id: listDelegate

            Item {
                height: 70
                width: parent.width

                CheckBox {
                    id: checkBox
                    height: 20

                    style: CheckBoxStyle {
                        label: Text {
                            color: Constants.panelTextColor
                            text: permissionItems.get(index).label
                        }
                    }
                    onClicked: {
                        permissionItems.setProperty(index, "allowed", !allowed)
                    }
                }

                Text {
                    id: label
                    color: Constants.primaryColor
                    font.pixelSize: 0
                    text: textBody
                    wrapMode: TextEdit.Wrap
                    width: parent.width
                    anchors.top: checkBox.bottom
                }
            }
        }

        ScrollableListView {
            id: onAppPermissonList
            anchors.fill: parent;
            anchors.margins: 5
            anchors.bottomMargin: 100
            model: permissionItems
            delegate: listDelegate
        }

        Item {
            id: bottomPanel
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            height: 100
            width: parent.width

            OvalButton {
                anchors.centerIn: parent
                text: "Done"
                onClicked: {
                    deactivate()
                }
            }
        }
    }
}
