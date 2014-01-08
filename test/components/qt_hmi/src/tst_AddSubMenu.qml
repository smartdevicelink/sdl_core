/**
 * @file tst_ScrollableMessage.qml
 * @brief Test Case for ScrollableMessageView.
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


import QtTest 1.0
import QtQuick 2.0
import QtMultimedia 5.0
import com.ford.sdl.hmi.dbus_adapter 1.0
import com.ford.sdl.hmi.hw_buttons 1.0
import com.ford.sdl.hmi.log4cxx 1.0
import "../../../../src/components/qt_hmi/qml_model_qt5/controls"
import "../../../../src/components/qt_hmi/qml_model_qt5/views"
import "../../../../src/components/qt_hmi/qml_model_qt5/hmi_api" as HmiApi
import "../../../../src/components/qt_hmi/qml_model_qt5/models"
import "../../../../src/components/qt_hmi/qml_model_qt5/popups"
import "../../../../src/components/qt_hmi/qml_model_qt5/hmi_api/Common.js" as Common
import "../../../../src/components/qt_hmi/qml_model_qt5/models/Constants.js" as Constants
import "../../../../src/components/qt_hmi/qml_model_qt5/models/Internal.js" as Internal

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
            mainWindowLoader.source = "../../../../src/components/qt_hmi/qml_model_qt5/MainWindow.qml"
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

        //Create AddSubMenu and check data
        function test_01_addSubMenu() {
            console.debug("enter")
            var initData = {
                menuID: 100,
                menuParams: {
                    position: 100,
                    menuName: "Submenu1"
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

        //Create AddSubMenu and check data
        function test_02_1000_SubMenu() {
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
    }
}
