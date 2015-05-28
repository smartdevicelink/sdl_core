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
        property var dataContainer
        property var sdlUIProxy
        property var contentLoader
        property var messageModel
        property var messageView
        property var timer

        property var softButtonsListExample : [
            {softButtonID:0, isHighlighted:true, systemAction:Common.SystemAction.DEFAULT_ACTION,
                type:Common.SoftButtonType.SBT_TEXT, text:"Default Action"},
            {softButtonID:1, isHighlighted:false, systemAction:Common.SystemAction.STEAL_FOCUS,
                type:Common.SoftButtonType.SBT_IMAGE, text:"Steal Focus"},
            {softButtonID:2, isHighlighted:false, systemAction:Common.SystemAction.KEEP_CONTEXT,
                type:Common.SoftButtonType.SBT_BOTH, text:"Keep context"}]

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

        //get messageModel and messageView after call @scrollableMessage
        function getMessageViewModel() {
            messageModel    = dataContainer.scrollableMessageModel
            if(!messageModel)
                qtest_fail("messageModel is undefined");
            messageView = contentLoader.item
            if(!messageView)
                qtest_fail("messageView is undefined");
        }

        //cleanup for each test
        function destroyView() {
            //Clear Loader with MainWindow, which delete own models and views
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
            var initData  = {appID:1, timeout:2000,
                messageText:{fieldText:"Simple ScrollableMessage text"},
                softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            verify(result.__errno === undefined, "ScrollableMessage return error state")
            timer = messageView.getTimer()
            if(!timer)
                qtest_fail("timer is undefined");
            //check button equals to init data
            compare(messageModel.softButtons.count, initData.softButtons.length, "wrong buttons count created")
            for (var i = 0, len = messageModel.softButtons.count; i < len; i++) {
                var act = messageModel.softButtons.get(i)
                var exp = initData.softButtons[i]
                compare(act.softButtonID, exp.softButtonID, "wrong softButtonID in button")
                compare(act.isHighlighted, exp.isHighlighted, "wrong isHighlighted in button")
                compare(act.systemAction, exp.systemAction, "wrong systemAction in button")
                compare(act.text, exp.text, "wrong text in button")
            }
            //check model data equals to init data
            compare(messageModel.running, true, "ScrollableMessage didn't start")
            compare(messageModel.longMessageText, initData.messageText.fieldText, "wrong messageText")
            compare(messageModel.appId, initData.appID, "wrong application ID")
            compare(messageModel.timeout, initData.timeout, "wrong timeout")
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

            verify(result.__errno === undefined, "ScrollableMessage return error state")
            //NOTE: don't check timer - it has been triggered immediately (timeout is 0)
            //MessageView call @complete immediately
            compare(messageModel.running, false, "ScrollableMessage didn't stop")
            compare(messageModel.softButtons.count, initData.softButtons.length, "wrong buttons count")
            compare(messageModel.longMessageText, initData.messageText.fieldText, "wrong messageText")
            compare(messageModel.timeout, initData.timeout, "wrong timeout")
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage twice (after first view has closed)
        function test_03_doubleCreate() {
            console.debug("enter")
            var initData  = {appID:1, timeout:0,    messageText:{fieldText:"Simple text"},
                softButtons:softButtonsListExample}
            var initData2 = {appID:1, timeout:10000, messageText:{fieldText:"Simple text 2"},
                softButtons:[]}
            createMessageView(initData.appID)

            //create view
            var actualResult = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            //create new view
            var actualResult2 = sdlUIProxy.scrollableMessage(initData2)
            getMessageViewModel()

            verify(actualResult.__errno === undefined, "ScrollableMessage return error state")
            verify(actualResult2.__errno === undefined, "ScrollableMessage return error state")
            compare(messageModel.running, true, "ScrollableMessage didn't start")
            compare(messageModel.longMessageText, initData2.messageText.fieldText, "wrong messageText")
            compare(messageModel.timeout, initData2.timeout, "wrong timeout")
            compare(messageModel.appId, initData2.appID, "wrong application ID")
            verify(messageModel.async !== undefined, "async in undefined")
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage twice (before first view is closed)
        function test_04_doubleCreateError() {
            console.debug("enter")
            var initData  = {appID:1, timeout:20000, messageText:{fieldText:"Simple text"},
                softButtons:softButtonsListExample}
            var initData2 = {appID:1, timeout:10000, messageText:{fieldText:"Simple text 2"},
                softButtons:[]}
            createMessageView(initData.appID)

            //create view
            var actualResult = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()
            var firstView = messageView;

            //create new view
            var actualResult2 = sdlUIProxy.scrollableMessage(initData2)
            getMessageViewModel()
            verify(actualResult.__errno === undefined, "ScrollableMessage return error state")
            verify(actualResult2.__errno !== undefined, "ScrollableMessage don't return error state")
            compare(messageModel.running, true, "ScrollableMessage didn't start")
            compare(messageView, firstView, "creating new view insteed stay first")
            compare(messageModel.longMessageText, initData.messageText.fieldText, "wrong messageText")
            compare(messageModel.timeout, initData.timeout, "wrong timeout")
            compare(messageModel.appId, initData.appID, "wrong application ID")
            verify(messageModel.async !== undefined, "async in undefined")
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage and check close
        function test_05_ClickBackButton() {
            console.debug("enter")
            var initData  = {appID:1, timeout:20000,
                messageText:{fieldText:"Simple ScrollableMessage text"},
                softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            compare(messageModel.running, true, "ScrollableMessage didn't start")
            verify(result.__errno === undefined, "ScrollableMessage return error state")
            //look for back button
            var backButton  = messageView.getBackButton()
            verify(backButton !== undefined, "Not created back button")
            //wait rendering for correct click position
            waitForRendering(mainWindowLoader)
            //Press back button
            mouseClick(backButton, backButton.width/2, backButton.height/2,
                       Qt.LeftButton, Qt.NoModifier, 0)
            //check that MessageView (in contentLoader) is unloaded and deleted
            var isLoaded = (contentLoader.source.toString().indexOf("ScrollableMessageView.qml") > 0)
            verify(!isLoaded, "MessageView should be unloaded")
            //wait for delete messageView by GC
            wait(0);
            verify(messageView === null, "MessageView should be deleted")
            destroyView()
            console.debug("exit")
        }
        //call @scrollableMessage and check close
        function test_06_ClickDefaultAction() {
            console.debug("enter")
            var initData  = {appID:1, timeout:20000,
                messageText:{fieldText:"Simple ScrollableMessage text"},
                softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            compare(messageModel.running, true, "ScrollableMessage didn't start")
            verify(result.__errno === undefined, "ScrollableMessage return error state")
            //look for DEFAULT_ACTION button
            var defaultActionButton  = findButtonByAction(Common.SystemAction.DEFAULT_ACTION)
            verify(defaultActionButton !== undefined, "Not created button with DEFAULT_ACTION")
            //wait rendering for correct click position
            waitForRendering(mainWindowLoader)
            //Press default button
            mouseClick(defaultActionButton, defaultActionButton.width/2, defaultActionButton.height/2,
                       Qt.LeftButton, Qt.NoModifier, 0)
            //check that MessageView (in contentLoader) is unloaded and deleted
            var isLoaded = (contentLoader.source.toString().indexOf("ScrollableMessageView.qml") > 0)
            verify(!isLoaded, "MessageView should be unloaded")
            //wait for delete messageView by GC
            wait(0);
            verify(messageView === null, "MessageView should be deleted")
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage and check restart timer
        function test_07_ClickStealFocus() {
            console.debug("enter")
            var initData  = {appID:1, timeout:20000, messageText:{fieldText:"Simple ScrollableMessage text"},
                softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            //check
            compare(messageModel.running, true, "ScrollableMessage didn't start")
            verify(result.__errno === undefined, "ScrollableMessage return error state")
            //look for STEAL_FOCUS button
            var stealFocusButton  = findButtonByAction(Common.SystemAction.STEAL_FOCUS)
            verify(stealFocusButton !== undefined, "Not created button with STEAL_FOCUS")
            //wait rendering for correct buttons size for correct click position
            waitForRendering(mainWindowLoader)
            mouseClick(stealFocusButton, stealFocusButton.width/2, stealFocusButton.height/2,
                       Qt.LeftButton, Qt.NoModifier, 0)
            //check that MediaView loaded in loade
            var viewQMlFileName = dataContainer.currentApplication.isMediaApplication ?
                        "SDLPlayerView.qml" : "SDLNonMediaView.qml"
            var isLoaded = (contentLoader.source.toString().indexOf(viewQMlFileName) > 0)
            verify(isLoaded, "SDLPlayerView/SDLNonMediaView should be loaded")
            destroyView()
            console.debug("exit")
        }

        //call @scrollableMessage and check restart timer
        function test_08_ClickKeepContex() {
            console.debug("enter")
            var initData  = {appID:1, timeout:20000, messageText:{fieldText:"Simple ScrollableMessage text"},
                softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            //check
            compare(messageModel.running, true, "ScrollableMessage didn't start")
            verify(result.__errno === undefined, "ScrollableMessage return error state")
            //look for KEEP_CONTEXT button
            var keepContexButton  = findButtonByAction(Common.SystemAction.KEEP_CONTEXT)
            verify(keepContexButton !== undefined, "Not created button with KEEP_CONTEXT")
            //wait rendering for correct click position
            waitForRendering(mainWindowLoader)
            //Press button
            mouseClick(keepContexButton, keepContexButton.width/2, keepContexButton.height/2,
                       Qt.LeftButton, Qt.NoModifier, 0)
            //check that MessageView is still unloaded loaded
            var isLoaded = (contentLoader.source.toString().indexOf("ScrollableMessageView.qml") > 0)
            verify(isLoaded, "MessageView should be loaded")
            //verify restarted timer
            timer = messageView.getTimer()
            verify(timer.running === true, "Timer is not restarted by KEEP_CONTEXT button")
            destroyView()
            console.debug("exit")
        }

        //add long text to model and check scrollBar visibility
        function test_09_ScrollBarShown() {
            console.debug("enter")
            //generate string with (10!) lines
            var longText = "It is very long text!\n"
            for(var i = 1; i < 10; ++i)
                longText += longText
            var initData = {appID:1, timeout:20000, messageText:{fieldText:longText}, softButtons:[]}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            //check
            compare(messageModel.running, true, "ScrollableMessage didn't start")
            verify(result.__errno === undefined, "ScrollableMessage return error state")

            //wait rendering
            waitForRendering(mainWindowLoader)
            var scrollBar = messageView.getScrollbar()
            verify(scrollBar.visible === true, "ScrollBar is not shown for long text")

            destroyView()
            console.debug("exit")
        }

        //compare text width with and without buttons at top of SrollableMessage
        function test_10_MessageTextHeight() {
            console.debug("enter")
            var initData = {appID:1, timeout:500, messageText:{fieldText:"Simple text"}, softButtons:[]}
            createMessageView(initData.appID)

            var result = sdlUIProxy.scrollableMessage(initData)
            getMessageViewModel()

            //check
            compare(messageModel.running, true, "ScrollableMessage didn't start")
            verify(result.__errno === undefined, "ScrollableMessage return error state")

            var textAreaHeight = messageView.getTextArea().height
            wait(initData.timeout)

            var initData2 = {appID:1, timeout:20000, messageText:{fieldText:"Simple text"},
                softButtons:softButtonsListExample}
            createMessageView(initData.appID)

            var result2 = sdlUIProxy.scrollableMessage(initData2)
            getMessageViewModel()

            //wait rendering
            waitForRendering(mainWindowLoader)
            var textAreaHeight2 = messageView.getTextArea().height
            console.debug("messageTextH", textAreaHeight, textAreaHeight2)
            verify(textAreaHeight2  <= textAreaHeight, "Height of text area shoud be less with buttons")

            destroyView()
            console.debug("exit")
        }
    }
}
