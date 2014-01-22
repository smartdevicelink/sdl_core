/**
 * @file BasicCommunication.qml
 * @brief Implement of interface Basic Communication.
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

Item {
    function updateDeviceList (deviceList) {
        var deviceListLog = "";
        if (deviceList) {
            for (var i = 0; i < deviceList.length; i++) {
                deviceListLog += "{name: '" + deviceList[i].name + "', " +
                        "id: '" + deviceList[i].id + "'},";
            }
        }
        console.log("Message Received - {method: 'BasicCommunication.UpdateDeviceList', params:{ " +
                    "deviceList: [" + deviceListLog + "]" +
                    "}}")
        dataContainer.deviceList.clear();
        for(var i = 0; i < deviceList.length; i++) {
            dataContainer.deviceList.append({ name: deviceList[i].name, devid: deviceList[i].id })
        }
    }

    function updateAppList (applications) {
        var applicationsLog = "",
            appTypeLog = "";
        if (applications) {
            for (var i = 0; i < applications.length; i++) {
                appTypeLog = "";

                for (var j = 0; j < applications[i].appType.length; i++) {
                    appTypeLog += "'" + applications[i].appType + "', "
                }

                applicationsLog += "{name: '" + applications[i].name + "', " +
                        "appName: '" + applications[i].appName + "', " +
                        "ngnMediaScreenAppName: '" + applications[i].ngnMediaScreenAppName + "', " +
                        "icon: '" + applications[i].icon + "', " +
                        "deviceName: '" + applications[i].deviceName + "', " +
                        "appID: " + applications[i].appID + ", " +
                        "hmiDisplayLanguageDesired: '" + applications[i].hmiDisplayLanguageDesired + "', " +
                        "isMediaApplication: " + applications[i].isMediaApplication + ", " +
                        "appType: [" + applications[i].deviceName + "]" +
                        "},";
            }
        }
        console.log("Message Received - {method: 'BasicCommunication.UpdateAppList', params:{ " +
                    "applications: [" + applicationsLog + "]" +
                    "}}")


        dataContainer.applicationList.clear();
        for(var i = 0; i < applications.length; i++) {
            dataContainer.applicationList.append({
                 appName: applications[i].appName,
                 ngnMediaScreenAppName: applications[i].ngnMediaScreenAppName,
                 icon: applications[i].icon,
                 deviceName: applications[i].deviceName,
                 appId: applications[i].appID,
                 hmiDisplayLanguageDesired: applications[i].hmiDisplayLanguageDesired,
                 isMediaApplication: applications[i].isMediaApplication,
                 appType: applications[i].appType
            });
        }
    }

    function allowDeviceToConnect (device) {
        console.log("Message Received - {method: 'BasicCommunication.AllowDeviceToConnect', params:{ " +
                    "device: {name: '" + device.name + "', " +
                        "id: '" + device.id + "'}," +
                    "}}")
        return {
            allow: true
        }
    }

    function mixingAudioSupported() {
        console.log("Message Received - {method: 'BasicCommunication.MixingAudioSupported'}")
        return {
            attenuatedSupport: true
        }
    }

    function allowAllApps() {
        console.log("Message Received - {method: 'BasicCommunication.AllowAllApps'}")
        return {
            allowed: true
        }
    }

    function allowApp(app, appPermissions) {
        console.log("Message Received - {method: 'BasicCommunication.AllowApp'}")
        return {
            allowed: true
        }
    }

    function activateApp() {
        console.log("Message Received - {method: 'BasicCommunication.ActivateApp', params:{ " +
                    //"appID: " + appID +
                    "}}")
        contentLoader.go("views/SDLPlayerView.qml", appID);
    }
}
