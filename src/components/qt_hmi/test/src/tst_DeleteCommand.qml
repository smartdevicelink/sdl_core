/**
 * @file tst_ScrollableMessage.qml
 * @brief Test Case for ScrollableMessageView.
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

Item
{
    width : mainWindowLoader.width
    height: mainWindowLoader.height

    Loader
    {
        id: mainWindowLoader
        asynchronous : false
    }

    TestCase
    {
        name: "DeleteCommand"
        when: windowShown
        property var dataContainer
        property var sdlUIProxy
        property var contentLoader
        property var messageModel


        //initialization for each test
        function createMessageView(appID)
        {
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
        function destroyView()
        {
            //Clear Loader with MainWindow, which delete own models and views
            mainWindowLoader.source = ""
        }

        /*
         * Test Cases
         */

        function test_01_deleteCommand()
        {
            console.debug("enter")
            var initData = {
                cmdID: 1,
                menuParams:
                {
                    menuName:"",
                },
                cmdIcon:"",
                appID: 1
                           }
            var initData2 = {
                cmdID: 2,
                menuParams:
                {
                    menuName:"",
                },
                cmdIcon:"",
                appID: 1
                            }

            createMessageView(initData.appID)
            var app = dataContainer.getApplication(initData.appID)
            var MenuCount = app.options.count
            app.options.append(
                                   { id: initData.cmdID,
                                     name: initData.menuParams.menuName,
                                     type: Internal.MenuItemType.MI_NODE,
                                     position: Constants.positionOfElementWithoutPosition,
                                     icon: initData.cmdIcon ? cmdIcon : {},
                                     subMenu: []
                                   }
                              )

           compare(app.options.count, MenuCount+1, "Command into menu not added")
           sdlUIProxy.deleteCommand(initData2)

            var a = 0
            for(var i = 0;((a === 0) && ( i < app.options.count)); i++)
            {
                if (app.options.get(i).id===initData2.menuID)
                    {
                        a = 1
                    }
            }
            compare (a, 0, "Command in menu isn't removed")


           destroyView()
           console.debug("exit")
       }

        function test_02_deleteCommand() {
            console.debug("enter")
            var initData = {
                menuID: 1000,
                menuParams: {
                    position: 1,
                    menuName: "SubMenu"
                },
                appID: 1
            }

            var initData2 = {
                cmdID: 1,
                menuParams: {
                    menuName:"",
                },
                cmdIcon:"",
                appID: 1
            }

            createMessageView(initData.appID)
            var app = dataContainer.getApplication(initData.appID)
            var MenuCount = app.options.count
            app.options.append ({
                "id": initData.menuID,
                "name": initData.menuParams.menuName,
                "position": initData.menuParams.position,
                "type": Internal.MenuItemType.MI_SUBMENU,
                "icon": undefined,
                "subMenu":[{
                                "id":initData2.cmdID,
                                "name": initData2.menuParams.menuName,
                                "position": Constants.positionOfElementWithoutPosition,
                                "type": Internal.MenuItemType.MI_PARENT,
                                "icon": {
                                "imageType": Common.ImageType.DYNAMIC,
                                "value": "../res/nav/turnArrow.png"
                                 },

                                "subMenu": []
                           }]
            })

            compare(app.options.count, MenuCount+1, "SubMenu into added")
            var SubMenuCount = app.options.get(0).subMenu.count
            compare( SubMenuCount, 1, "Command is not added into SubMenu")

            sdlUIProxy.deleteCommand(initData2)

           compare(app.options.count , MenuCount+1, "Menu is changed")
           compare(app.options.get(0).subMenu.count, SubMenuCount-1, "Command into SubMenu is not removed")

           destroyView()
           console.debug("exit")
        }
    }
}
