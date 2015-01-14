/**
 * @file SimulationView.qml
 * @brief Area of screen responsible for simulation actions
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
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import "../controls"
import "../hmi_api/Common.js" as Common
import "../views"
import "../popups"
import "../models/Constants.js" as Constants

Rectangle {
    width: controlArea.width + controlArea.anchors.margins
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    color: Constants.panelColor

    Item {
        id: showArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height - (controlArea.childrenRect.height + controlArea.anchors.margins)

        ExitAllApplicationsPopup {
            id: exitAllApplicationsPopup
            anchors.fill: parent
            visible: false
        }

        VehicleInfoPopUp {
            id: viPopUp
            anchors.fill: parent
        }

        TBTClientStatePopUp {
            id: tbtClientStatePopUp
            anchors.fill: parent
            visible: false
        }
    }

    Item {
        id: controlArea
        anchors.bottom: parent.bottom
        anchors.margins: Constants.panelPadding
        anchors.horizontalCenter: parent.horizontalCenter
        width: table.width

        ListModel {
            id: languagesList

            Component.onCompleted: {
                for (var name in Common.Language) {
                    if (settingsContainer.sdlLanguagesList.indexOf(Common.Language[name]) != -1) {
                        append({name: name.replace('_', '-')});
                    }
                }
            }
        }

        Text {
            id: label
            text: "Languages"
            color: Constants.panelTextColor
            anchors.bottom: table.top
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Grid {
            id: table
            spacing: 5
            columns: 2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: globalProperties.top

            Text {
                text: "HMI UI"
                color: Constants.panelTextColor
            }

            Text {
                text: "HMI TTS + VR"
                color: Constants.panelTextColor
            }

            ComboBox {
                width: table.width / table.columns - table.spacing
                model: languagesList
                onCurrentIndexChanged: {
                    dataContainer.hmiUILanguage = settingsContainer.sdlLanguagesList[currentIndex];
                    sdlUI.onLanguageChange(dataContainer.hmiUILanguage);
                }
                z: 1000
            }

            ComboBox {
                width: table.width / table.columns - table.spacing
                model: languagesList
                onCurrentIndexChanged: {
                    dataContainer.hmiTTSVRLanguage = settingsContainer.sdlLanguagesList[currentIndex];
                    sdlTTS.onLanguageChange(dataContainer.hmiTTSVRLanguage);
                    sdlVR.onLanguageChange(dataContainer.hmiTTSVRLanguage);
                }
                z: 1000
            }

            Text {
                text: "Application UI"
                color: Constants.panelTextColor
            }

            Text {
                text: "Application TTS + VR"
                color: Constants.panelTextColor
            }

            Text {
                id: uiLanguageLabel
                color: Constants.panelTextColor
                text: " "
                Connections {
                    target: dataContainer
                    onCurrentApplicationChanged: {
                        for (var s in Common.Language) {
                            if (Common.Language[s] === dataContainer.currentApplication.hmiDisplayLanguageDesired) {
                                uiLanguageLabel.text = s;
                            }
                        }
                    }
                }
            }

            Text {
                id: ttsLanguageLabel
                color: Constants.panelTextColor
                text: " "
                Connections {
                    target: dataContainer
                    onCurrentApplicationChanged: {
                        for (var s in Common.Language) {
                            if (Common.Language[s] === dataContainer.currentApplication.languageTTSVR) {
                                ttsLanguageLabel.text = s;
                            }
                        }
                    }
                }
            }

            PushButton {
                id: vehicleInfo
                label: "Vehicle info"
                toggleMode: true
                onPressed: {
                    viPopUp.show();
                }
                onUnpressed: {
                    viPopUp.hide();
                }
            }

            PushButton {
                id: tbtClientState
                label: "TBT Client state"
                toggleMode: true
                onPressed: {
                    tbtClientStatePopUp.show();
                }
                onUnpressed: {
                    tbtClientStatePopUp.hide();
                }
            }

            PushButton {
                label: "Exit application"
                onClicked: {
                    if (dataContainer.applicationContext) {
                        sdlBasicCommunication.onExitApplication(dataContainer.currentApplication.appId)
                    }
                }
            }

            PushButton {
                id: exitAllAppsButton
                label: "Exit all apps"
                toggleMode: true
                onPressed: {
                    exitAllApplicationsPopup.show()

                }
                onUnpressed: {
                    exitAllApplicationsPopup.hide()
                }
                Connections {
                    target: exitAllApplicationsPopup
                    onVisibleChanged: {
                        if (!exitAllApplicationsPopup.visible) {
                            exitAllAppsButton.state = "unpressed"
                        }
                    }
                }
            }

            CheckBox {
                style: CheckBoxStyle {
                    label: Text {
                        color: Constants.panelTextColor
                        text: "Use URL"
                    }
                }
            }

            CheckBox {
                style: CheckBoxStyle {
                    label: Text {
                        color: Constants.panelTextColor
                        text: "DD"
                    }
                }
                onClicked: {
                    if (checked) {
                        dataContainer.driverDistractionState =
                                Common.DriverDistractionState.DD_ON;
                    } else {
                        dataContainer.driverDistractionState =
                                Common.DriverDistractionState.DD_OFF;
                    }
                }
            }
        }

        Rectangle {
            id: globalProperties
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: listGlobalProperties.height + Constants.panelPadding
            border.color: Constants.panelTextColor
            border.width: 1
            color: Constants.panelColor
            Column {
                id: listGlobalProperties
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: Constants.panelPadding / 4
                Text {
                    text: "HELP_PROMPT: " + dataContainer.currentApplication.helpPrompt
                    color: Constants.panelTextColor
                }
                Text {
                    text: "TIMEOUT_PROMPT: " + dataContainer.currentApplication.timeoutPrompt
                    color: Constants.panelTextColor
                }
                Text {
                    text: "AUTOCOMPLETE_TEXT: " // TODO(ALeshin): Function didn' realized yet
                    color: Constants.panelTextColor
                }
            }
        }
    }
}
