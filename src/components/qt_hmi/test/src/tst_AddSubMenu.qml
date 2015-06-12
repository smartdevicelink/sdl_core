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
        name: "AddSubMenu"
        when: windowShown
        property var dataContainer
        property var sdlUIProxy
        property var contentLoader
        property var messageModel

        //initialization for each test
        function createMessageView(appID) {
            mainWindowLoader.source = ""
            mainWindowLoader.source = "../qml_model_qt5/MainWindow.qml"
            var mainWindowObject = mainWindowLoader.item
            if(!mainWindowObject)
                qtest_fail("mainWindowObject is undefined");
            var mainScreen = mainWindowObject.getMainScreen()
            mainScreen.visible = true
            var warningInfo = mainWindowObject.getWarningInfo()
            warningInfo.visible = false
            contentLoader   = mainWindowObject.getContentLoader()
            contentLoader.asynchronous = false
            sdlUIProxy      = mainWindowObject.getUIProxy()
            dataContainer   = mainWindowObject.getDataStorage()
            dataContainer.addApplication({appName:"TestAppName", appId:appID})
        }

        //cleanup for each test
        function destroyView() {
            //Clear Loader with MainWindow, which delete own models and views
            mainWindowLoader.source = ""
        }

        /*
         * Test Cases
         */

        //Positive case and in boundary conditions
        function test_01_addSubMenu() {
            console.debug("enter")
            var initData = {
                menuID: 1000,
                menuParams: {
                    position: 500,
                    menuName: "SubMenu positive"
                },
                appID: 1
            }

            createMessageView(initData.appID)
            sdlUIProxy.addSubMenu(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(0).id, initData.menuID, "SubMenu id")
            compare(app.options.get(0).position, initData.menuParams.position, "SubMenu position")
            compare(app.options.get(0).name, initData.menuParams.menuName, "SubMenu name")

            destroyView()
            console.debug("exit")
        }

        // Only mandatory - without Position
        function test_02_addSubMenu_WithoutPosition() {
            console.debug("enter")
            var initData = {
                menuID: 1001,
                menuParams: {
                    menuName: "SubMenu mandatory only"
                },
                appID: 1
            }

            createMessageView(initData.appID)
            sdlUIProxy.addSubMenu(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(0).id, initData.menuID, "SubMenu id")
            //How to verify if value was not set?
            //compare(app.options.get(0).position, initData.menuParams.position, "SubMenu position")
            compare(app.options.get(0).name, initData.menuParams.menuName, "SubMenu name")

            destroyView()
            console.debug("exit")
        }

        //Create AddSubMenu and check data
        function test_03_1000_SubMenu() {
            console.debug("enter")
            var initData = {
                appID: 1,
                subMenus: []
            }

            for (var i = 0; i < 1010; i++) {

                initData.subMenus.push({
                    menuID: i,
                    menuParams: {
                        position: i,
                        menuName: "Submenu" + i
                    },
                    appID: 1
                })
            }

            createMessageView(initData.appID)
            for (var i = 0; i < initData.subMenus.length; i++) {
                sdlUIProxy.addSubMenu(initData.subMenus[i])
            }
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.count, 1000, "SubMenus count")

            for (var i = 0; i < 1000; i++) {
                compare(app.options.get(i).id, initData.subMenus[i].menuID, "SubMenu id")
                compare(app.options.get(i).position, initData.subMenus[i].menuParams.position, "SubMenu position")
                compare(app.options.get(i).name, initData.subMenus[i].menuParams.menuName, "SubMenu name")
            }

            destroyView()
            console.debug("exit")
        }

        //Create AddSubMenu and check data
        function test_04_addSubMenu_UpperBound() {
            console.debug("enter")
            var initData = {
                menuID: 2000000000,
                menuParams: {
                    position: 1000,
                    menuName: "00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890a00012å/678'90abc!def@ghi#jkl$mno%pqr^stu*vwx:yz()ABC-DEF_GHI=JKL+MNO|PQR~STU{}WXY[]Z,01234567890aAaaaaaaaaaaaaaa"
                },
                appID: 1
            }

            createMessageView(initData.appID)
            sdlUIProxy.addSubMenu(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(0).id, initData.menuID, "SubMenu id")
            compare(app.options.get(0).position, initData.menuParams.position, "SubMenu position")
            compare(app.options.get(0).name.length, 500, "SubMenu name")

            destroyView()
            console.debug("exit")
        }

        //Create AddSubMenu and check data
        function test_05_addSubMenu_LowerBound() {
            console.debug("enter")
            var initData = {
                menuID: 0,
                menuParams: {
                    position: 0,
                    menuName: "0"
                },
                appID: 1
            }

            createMessageView(initData.appID)
            sdlUIProxy.addSubMenu(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(0).id, initData.menuID, "SubMenu id")
            compare(app.options.get(0).position, initData.menuParams.position, "SubMenu position")
            compare(app.options.get(0).name.length, 1, "SubMenu name")

            destroyView()
            console.debug("exit")
        }

        // With fake parameter
        function test_06_addSubMenu_With_Fake_Parameter() {
            console.debug("enter")
            var initData = {
                menuID: 1001,
                menuParams: {
                    menuName: "SubMenu fake param",
                    position: 1,
                    fakeParam: "fakeParam"
                },
                appID: 1
            }

            createMessageView(initData.appID)
            sdlUIProxy.addSubMenu(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.get(0).id, initData.menuID, "SubMenu id")
            compare(app.options.get(0).position, initData.menuParams.position, "SubMenu position")
            compare(app.options.get(0).name, initData.menuParams.menuName, "SubMenu name")

            destroyView()
            console.debug("exit")
        }

        // With fake parameter
        function test_07_addSubMenu_menuIDOutLowerBound() {
            console.debug("enter")
            var initData = {
                menuID: -1,
                menuParams: {
                    menuName: "100",
                    position: 1,
                },
                appID: 1
            }

            createMessageView(initData.appID)
            sdlUIProxy.addSubMenu(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.count, 0, "SubMenus count")

            compare(app.options.get(1), !undefined, "SubMenu duplicated")

            destroyView()
            console.debug("exit")
        }

        // With fake parameter
        function test_08_addSubMenu_menuIDOutUpperBound() {
            console.debug("enter")
            var initData = {
                menuID: 2000000001,
                menuParams: {
                    menuName: "100",
                    position: 1,
                },
                appID: 1
            }

            createMessageView(initData.appID)
            sdlUIProxy.addSubMenu(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.count, 0, "SubMenus count")

            compare(app.options.get(1), !undefined, "SubMenu duplicated")

            destroyView()
            console.debug("exit")
        }

        // With fake parameter
        function test_09_addSubMenu_duplicatedSubMenus() {
            console.debug("enter")
            var initData = {
                menuID: 2000000001,
                menuParams: {
                    menuName: "100",
                    position: 1,
                },
                appID: 1
            }

            createMessageView(initData.appID)
            sdlUIProxy.addSubMenu(initData)
            sdlUIProxy.addSubMenu(initData)
            var app = dataContainer.getApplication(initData.appID)

            compare(app.options.count, 1, "SubMenus count")

            compare(app.options.get(1), undefined, "SubMenu duplicated")

            destroyView()
            console.debug("exit")
        }
    }
}
