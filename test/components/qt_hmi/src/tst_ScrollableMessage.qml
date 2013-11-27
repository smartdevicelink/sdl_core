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
import "../../../../src/components/qt_hmi/qml_model/controls"
import "../../../../src/components/qt_hmi/qml_model/views"
import "../../../../src/components/qt_hmi/qml_model/hmi_api" as HmiApi
import "../../../../src/components/qt_hmi/qml_model/models"
import "../../../../src/components/qt_hmi/qml_model/popups"
import "../../../../src/components/qt_hmi/qml_model/hmi_api/Common.js" as Common
import "../../../../src/components/qt_hmi/qml_model/models/Constants.js" as Constants
import "../../../../src/components/qt_hmi/qml_model/models/Internal.js" as Internal

Item {
    width : mainWindowLoader.width
    height: mainWindowLoader.height

    Loader {
        id: mainWindowLoader
        asynchronous : false
    }

    TestCase {
        name: "ScrollableMessageView"
        when: windowShown
        property var mainWindowComponent : Qt.createComponent("../../../../src/components/qt_hmi/qml_model/MainWindow.qml")
        property var mainWindowObject
        property var dataContainer
        property var sdlUIProxy
        property var contentLoader
        property var messageModel
        property var messageView
        property var timer

        property var softButtonsListExample : [
            {softButtonID:0, isHighlighted:true,  systemAction:Common.SystemAction.DEFAULT_ACTION,type:Common.SoftButtonType.SBT_TEXT ,text:"Default Action"},
            {softButtonID:1, isHighlighted:false, systemAction:Common.SystemAction.STEAL_FOCUS,   type:Common.SoftButtonType.SBT_IMAGE,text:"Steal Focus"},
            {softButtonID:2, isHighlighted:false, systemAction:Common.SystemAction.KEEP_CONTEXT,  type:Common.SoftButtonType.SBT_BOTH, text:"Keep context"}]

        //initialization for each test
        function createMessageView(appID) {
            mainWindowLoader.source = ""
            mainWindowLoader.source = "../../../../src/components/qt_hmi/qml_model/MainWindow.qml"
            mainWindowObject = mainWindowLoader.item
            console.assert(mainWindowObject !== undefined, "mainWindowObject is undefined");
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

        //get messageModel and messageView after call @scrollableMessage
        function getMessageViewModel() {
            messageModel    = dataContainer.scrollableMessageModel
            console.assert(messageModel !== undefined, "messageModel is undefined");
            messageView = contentLoader.item
            console.assert(messageView !== undefined, "messageView is undefined");
        }

        //cleanup for each test
        function destroyView() {
            mainWindowLoader.source = ""
        }

        //return button from messageView by ommon.SystemAction
        function findButtonByAction(systemAction){
            var listViewItems = messageView.getSoftButtonsListView()
            for (var i = 0, len = listViewItems.count; i < len; i++) {
                //get SoftButton by setting current item (ListView has no getter)
                listViewItems.currentIndex = i
                var buttonItem = listViewItems.currentItem
                if (buttonItem.button.systemAction === systemAction) {
                    return buttonItem
                }
            }
        }

        /*
         * Test Cases
         */

        //Create ScrollableMessageView and check data
        function test_01_create() {
            console.debug("enter")
            var initData  = {appID:1, timeout:2000, messageText:{fieldText:"Simple ScrollableMessage text"},   softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            if(result.__errno !== undefined)
                fail("ScrollableMessage return error state")
            timer = messageView.getTimer()
            console.assert(timer !== undefined, "timer is undefined");
            //check button equals to init data
            if (messageModel.softButtons.count === initData.softButtons.length) {
                for (var i = 0, len = messageModel.softButtons.count; i < len; i++) {
                    if (!qtest_compareInternal(messageModel.softButtons.get(i).softButtonID, initData.softButtons[i].softButtonID))
                        fail("wrong softButtonID in button")
                    if (!qtest_compareInternal(messageModel.softButtons.get(i).isHighlighted,initData.softButtons[i].isHighlighted))
                        fail("wrong isHighlighted in button")
                    if (!qtest_compareInternal(messageModel.softButtons.get(i).systemAction, initData.softButtons[i].systemAction))
                        fail("wrong systemAction in button")
                    if (!qtest_compareInternal(messageModel.softButtons.get(i).text,         initData.softButtons[i].text))
                        fail("wrong text in button")
                }
            } else {
                fail("wrong buttons count created")
            }
            //check model data equals to init data
            compare(messageModel.running, true)
            compare(messageModel.longMessageText, initData.messageText.fieldText)
            compare(messageModel.appId, initData.appID)
            compare(messageModel.timeout, initData.timeout)
            verify(messageModel.async !== undefined, "async in undefined")
            destroyView()
            console.debug("exit")
        }

        //Create ScrollableMessageView with empty data
        function test_02_emptyInit() {
            console.debug("enter")
            var initData  = {appID:1, timeout:0, messageText:{fieldText:""},   softButtons:[]}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            if(result.__errno !== undefined)
                fail("ScrollableMessage return error state")
            //NOTE: don't check timer - it has been triggered immediately (timeout is 0)
            //MessageView call @complete immediately
            compare(messageModel.running, false)
            compare(messageModel.softButtons.count, initData.softButtons.length)
            compare(messageModel.longMessageText, initData.messageText.fieldText)
            compare(messageModel.timeout, initData.timeout)
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage twice (after first view has closed)
        function test_03_doubleCreate() {
            console.debug("enter")
            var initData  = {appID:1, timeout:0,    messageText:{fieldText:"Simple text"},   softButtons:softButtonsListExample}
            var initData2 = {appID:1, timeout:10000,messageText:{fieldText:"Simple text 2"}, softButtons:[]}
            createMessageView(initData.appID)

            //create view
            var actualResult = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            //create new view
            var actualResult2 = sdlUIProxy.scrollableMessage(initData2)
            getMessageViewModel()

            if(actualResult.__errno !== undefined)
                fail("ScrollableMessage return error state")
            if(actualResult2.__errno !== undefined)
                fail("ScrollableMessage return error state")
            compare(messageModel.running, true)
            compare(messageModel.longMessageText, initData2.messageText.fieldText)
            compare(messageModel.timeout,initData2.timeout)
            compare(messageModel.appId, initData2.appID)
            verify(messageModel.async !== undefined, "async in undefined")
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage twice (before first view is closed)
        function test_04_doubleCreateError() {
            console.debug("enter")
            var initData  = {appID:1, timeout:20000, messageText:{fieldText:"Simple text"},   softButtons:softButtonsListExample}
            var initData2 = {appID:1, timeout:10000, messageText:{fieldText:"Simple text 2"}, softButtons:[]}
            createMessageView(initData.appID)

            //create view
            var actualResult = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()
            var firstView = messageView;

            //create new view
            var actualResult2 = sdlUIProxy.scrollableMessage(initData2)
            getMessageViewModel()
            if(actualResult.__errno !== undefined)
                fail("ScrollableMessage return error state")
            if(actualResult2.__errno === undefined)
                fail("ScrollableMessage don't return error state")
            compare(messageModel.running, true)
            compare(messageView, firstView, "creating new view insteed stay first")
            compare(messageModel.longMessageText, initData.messageText.fieldText)
            compare(messageModel.appId, initData.appID)
            compare(messageModel.timeout, initData.timeout)
            verify(messageModel.async !== undefined, "async in undefined")
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage and check close
        function test_05_ClickDefaultAction() {
            console.debug("enter")
            var initData  = {appID:1, timeout:20000, messageText:{fieldText:"Simple ScrollableMessage text"},   softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            compare(messageModel.running, true)
            if(result.__errno !== undefined)
                fail("ScrollableMessage return error state")
            //look for DEFAULT_ACTION button
            var defaultActionButton  = findButtonByAction(Common.SystemAction.DEFAULT_ACTION)
            verify(defaultActionButton !== undefined, "Not created button with DEFAULT_ACTION")
            //wait rendering for correct click position
            waitForRendering(mainWindowLoader)
            //Press default button
            mouseClick(defaultActionButton, defaultActionButton.width/2, defaultActionButton.height/2, Qt.LeftButton, Qt.NoModifier, 0)
            //check that MessageView (in contentLoader) is unloaded and deleted
            verify(contentLoader.source.toString().indexOf("ScrollableMessageView.qml") < 0, "MessageView should be unloaded")
            //wait for delete messageView by GC
            wait(0);
            verify(messageView === null, "MessageView should be deleted")
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage and check restart timer
        function test_06_ClickStealFocus() {
            console.debug("enter")
            var initData  = {appID:1, timeout:20000, messageText:{fieldText:"Simple ScrollableMessage text"},   softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            //check
            compare(messageModel.running, true)
            if(result.__errno !== undefined)
                fail("ScrollableMessage return error state")
            //look for STEAL_FOCUS button
            var stealFocusButton  = findButtonByAction(Common.SystemAction.STEAL_FOCUS)
            verify(stealFocusButton !== undefined, "Not created button with STEAL_FOCUS")
            //wait rendering for correct buttons size for correct click position
            waitForRendering(mainWindowLoader)
            mouseClick(stealFocusButton, stealFocusButton.width/2, stealFocusButton.height/2, Qt.LeftButton, Qt.NoModifier, 0)
            //check that MediaView loaded in loade
            var viewQMlFileName = dataContainer.currentApplication.isMediaApplication ? "SDLPlayerView.qml" : "SDLNonMediaView.qml"
            verify(contentLoader.source.toString().indexOf(viewQMlFileName) > 0, "SDLPlayerView should be loaded")
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage and check restart timer
        function test_07_ClickKeepContex() {
            console.debug("enter")
            var initData  = {appID:1, timeout:20000, messageText:{fieldText:"Simple ScrollableMessage text"},   softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            //check
            compare(messageModel.running, true)
            if(result.__errno !== undefined)
                fail("ScrollableMessage return error state")
            //look for KEEP_CONTEXT button
            var keepContexButton  = findButtonByAction(Common.SystemAction.KEEP_CONTEXT)
            verify(keepContexButton !== undefined, "Not created button with KEEP_CONTEXT")
            //wait rendering for correct click position
            waitForRendering(mainWindowLoader)
            //Press button
            mouseClick(keepContexButton, keepContexButton.width/2, keepContexButton.height/2, Qt.LeftButton, Qt.NoModifier, 0)
            //check that MessageView is still unloaded loaded
            verify(contentLoader.source.toString().indexOf("ScrollableMessageView.qml") > 0, "MessageView should be loaded")
            //verify restarted timer
            timer = messageView.getTimer()
            verify(timer.running === true, "Timer is not restarted by KEEP_CONTEXT button")
            destroyView()
            console.debug("exit")
        }
    }
}
