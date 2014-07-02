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
import "../models/Constants.js" as Constants
import "../models/RequestToSDL.js" as RequestToSDL

Item {

    property string popUpName
    property int appId
    property ListModel permissionItems: ListModel{}

    signal itemActivated(string item)

    onItemActivated: {
        this
    }

    function activate(appId) {
        console.debug("onAppPermissionConsentPopUp activate enter");
        appId = appId
        visible = true;
        console.debug("onAppPermissionConsentPopUp activate exit");
    }

    function deactivate() {
        console.debug("onAppPermissionConsentPopUp deactivate enter");
        visible = false;
        //OnAppPermissionConsent
        sdlSDL.onAppPermissionConsent()
        console.debug("onAppPermissionConsentPopUp deactivate exit");
    }

    Rectangle {
        width: parent.width -200
        height: parent.height - 100
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: Constants.secondaryColor
        border.width: 1
        border.color: Constants.popUpBorderColor
        radius: padding
        z: 1000

        Component {
            id: listDelegate

            Item {
                height: 70
                width: parent.width

                Row {
                    Row {
                        height: 20
                        CheckBox {
                            style: CheckBoxStyle {
                                label: Text {
                                    color: Constants.panelTextColor
                                    text: label
                                }
                            }
                            onClicked: {
                                permissionItems.setProperty(index, "allowed", !allowed)
                            }
                        }
                    }
                    Row {
                        height: 50
                        Text {
                            id: label
                            color: Constants.primaryColor
                            //anchors.centerIn: parent
                            //verticalAlignment: Text.AlignVCenter
                            //horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 0
                            elide: Text.ElideRight
                            text: textBody
                            wrapMode: TextEdit.Wrap
                            width: parent.width
                        }
                    }
                }
            }
        }

        ScrollableListView {
            id: onAppPermissonList
            anchors.fill: parent;
            anchors.margins: 5
            model: permissionItems
            delegate: listDelegate
        }

        Item {
            id: bottomPanel
            // 1/4 bottom screen
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            height: 1/4 * parent.height
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
