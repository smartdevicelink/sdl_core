/**
 * @file tst_AddSubMenu.qml
 * @brief Test Case for OptionsView.
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

import QtTest 1.0
import QtQuick 2.0
import QtMultimedia 5.0
import com.ford.sdl.hmi.dbus_adapter 1.0
import com.ford.sdl.hmi.hw_buttons 1.0
import com.ford.sdl.hmi.log4cxx 1.0
import "../qml_model_qt5/controls"
import "../qml_model_qt5/views"
import "../qml_model_qt5/hmi_api" as HmiApi
import "../qml_model_qt5/models"
import "../qml_model_qt5/popups"
import "../qml_model_qt5/hmi_api/Common.js" as Common
import "../qml_model_qt5/models/Constants.js" as Constants
import "../qml_model_qt5/models/Internal.js" as Internal

Item {
    width : mainWindowLoader.width
    height: mainWindowLoader.height

    Loader {
        id: mainWindowLoader
        asynchronous : false
    }

    TestCase {
        name: "AddCommand"
        when: windowShown
        property var dataContainer
        property var sdlUIProxy
        property var contentLoader
        property var messageModel

        function createView(appID) {
            mainWindowLoader.source = "../qml_model_qt5/MainWindow.qml"
            var mainWindowObject = mainWindowLoader.item
            sdlUIProxy      = mainWindowObject.getUIProxy()
            dataContainer   = mainWindowObject.getDataStorage()
            dataContainer.addApplication({appName:"TestAppName", appId:appID})
        }

        function destroyView() {
            mainWindowLoader.source = ""
        }

        /*
         * Test Cases
         */

        function test_01_addCommand_Command_Positive() {
            console.debug("enter")

            var initData = {
                cmdID: 1,
                cmdIcon: {
                    value: "123.jpg",
                    imageType: "DYNAMIC"
                },
                menuParams: {
                    position: 500,
                    menuName: "Command positive"
                },
                appID: 1
            }

            createView(initData.appID)
            sdlUIProxy.addCommand(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(0).id, initData.cmdID, "Command id")
            compare(app.options.get(0).position, initData.menuParams.position, "Command position")
            compare(app.options.get(0).name, initData.menuParams.menuName, "Command name")
            compare(app.options.get(0).icon.value, initData.cmdIcon.value, "Image path")

            destroyView()
            console.debug("exit")
        }

        function test_02_addCommand_Without_Position() {
            console.debug("enter")
            var initData = {
                appID: 1,
                Commands: []
            }
            initData.Commands.push({
                cmdID:1,
                comIcon: {
                    value: "123.jpg",
                    imageType: "DYNAMIC"
                },
                menuParams: {
                    menuName: "Command 1"
                },
                appID: 1
            })
            initData.Commands.push({
                cmdID:2,
                cmdIcon: {
                    value: "345.jpg",
                    imageType: "DYNAMIC"
                },
                menuParams: {
                    position: 500,
                    menuName: "Command 1"
                },
                appID: 1
            })
            createView(initData.appID)
            sdlUIProxy.addCommand(initData.Commands[0])
            sdlUIProxy.addCommand(initData.Commands[1])
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(1).id, initData.Commands[0].cmdID, "Command id")
            compare(app.options.get(1).name, initData.Commands[0].menuParams.menuName, "Command name")
            compare(app.options.get(0).id, initData.Commands[1].cmdID, "Command id")
            compare(app.options.get(0).name, initData.Commands[1].menuParams.menuName, "Command name")

            destroyView()
            console.debug("exit")
        }

        function test_03_1010_addCommands() {
            console.debug("enter")
            var initData = {
                appID: 1,
                Commands: []
            }

            for (var i = 0; i < 1010; i++) {
                initData.Commands.push({
                    cmdID: i,
                    cmdIcon: {
                        value: "",
                        imageType: ""
                    },
                    menuParams: {
                        position: i,
                        menuName: "Command " + i
                    },
                    appID: 1
                })
            }

            createView(initData.appID)
            for (var i = 0; i < initData.Commands.length; i++) {
                sdlUIProxy.addCommand(initData.Commands[i])
            }
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.count, 1000, "Command count")

            for (var i = 0; i < 1000; i++) {
                compare(app.options.get(i).id, initData.Commands[i].cmdID, "Command id")
                compare(app.options.get(i).position, initData.Commands[i].menuParams.position, "Command position")
                compare(app.options.get(i).name, initData.Commands[i].menuParams.menuName, "Command name")
            }

            destroyView()
            console.debug("exit")
        }

        function test_04_Command_Upper_Bound() {
            console.debug("enter")
            var initData = {
                cmdID: 2000000000,
                cmdIcon: {
                    value: "00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890aAaaaaaaaaaaaaaa",
                    imageType: ""
                },
                menuParams: {
                    position: 100000,
                    menuName: "00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890aAaaaaaaaaaaaaaa"
                },
                appID: 1
            }

            createView(initData.appID)
            sdlUIProxy.addCommand(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(0).id, initData.cmdID, "Command id")
            compare(app.options.get(0).position, initData.menuParams.position, "Command position")
            compare(app.options.get(0).name.length, initData.menuParams.menuName.length, "Command name")
            compare(app.options.get(0).icon.value.length, initData.cmdIcon.value.length, "Image path")

            destroyView()
            console.debug("exit")
        }

        function test_05_addCommand_Lower_Bound() {
            console.debug("enter")
            var initData = {
                cmdID: 0,
                cmdIcon: {
                    value: "",
                    imageType: ""
                },
                menuParams: {
                    position: 0,
                    menuName: "0",
                    parentID: 0
                },
                appID: 0
            }

            createView(initData.appID)
            sdlUIProxy.addCommand(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(0).id, initData.cmdID, "Command id")
            compare(app.options.get(0).position, initData.menuParams.position, "Command position")
            compare(app.options.get(0).name, initData.menuParams.menuName, "Command name")
            compare(app.options.get(0).icon.value, initData.cmdIcon.value, "Image path")

            destroyView()
            console.debug("exit")
        }

        function test_06_addCommand_With_Fake_Parameter() {
            console.debug("enter")
            var initData = {
                cmdID: 1,
                cmdIcon: {
                    value: "345.jpg",
                    imageType: "DYNAMIC"
                },
                menuParams: {
                    menuName: "Command fake param",
                    position: 1,
                    fakeParam: "fakeParam"
                },
                appID: 1
            }

            createView(initData.appID)
            sdlUIProxy.addCommand(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(0).id, initData.cmdID, "Command id")
            compare(app.options.get(0).position, initData.menuParams.position, "Command position")
            compare(app.options.get(0).name, initData.menuParams.menuName, "Command name")
            compare(app.options.get(0).icon.value, initData.cmdIcon.value, "Image path")

            destroyView()
            console.debug("exit")
        }

        function test_07_addCommand_No_Menu_Params() {
            console.debug("enter")

            var initData = {
                appID: 1,
                Commands:[]
            }

            var n = 10;
            initData.Commands.push({
                cmdID: 123,
                menuParams: {},
                appID: 1
            })
            for (var i = 0; i < n; i++) {
                initData.Commands.push({
                    cmdID: i,
                    menuParams: {
                        position: i,
                        menuName: "Command " + i
                    },
                    appID: 1
                })
            }

            createView(initData.appID)
            for (var i = 0; i < initData.Commands.length; i++) {
                sdlUIProxy.addCommand(initData.Commands[i])
            }
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.count, initData.Commands.length, "Command count")
            compare(app.options.get(n).id, initData.Commands[0].cmdID, "Command 11 id")
            compare(app.options.get(0).id, initData.Commands[1].cmdID, "Command 12 id")

            destroyView()
            console.debug("exit")
        }
    }
}
