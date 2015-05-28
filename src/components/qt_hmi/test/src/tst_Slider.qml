/**
 * @file tst_Slider.qml
 * @brief Test Case for Slider.
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
import "../qml_model_qt5/controls"
import "../qml_model_qt5/views"
import "../qml_model_qt5/hmi_api" as HmiApi
import "../qml_model_qt5/models"
import "../qml_model_qt5/popups"
import "../qml_model_qt5/hmi_api/Common.js" as Common
import "../qml_model_qt5/models/Constants.js" as Constants
import "../qml_model_qt5/models/Internal.js" as Internal

Item {
    TestCase {
        name: "Slider"
        when: windowShown
        property var mainWindowComponent : Qt.createComponent("../qml_model_qt5/MainWindow.qml")
        property var mainObject
        property var sldUI
        property var dataContainer
        property var sliderPopup
        property var timer

        function createSlider() {
            mainObject      = mainWindowComponent.createObject(parent)
            var mainScreen = mainObject.getMainScreen()
            mainScreen.visible = true
            sldUI = mainObject.getUIProxy()
            dataContainer   = mainObject.getDataStorage()
            sliderPopup     = mainObject.getSlider()
            timer           = sliderPopup.getTimer()
        }


        function destroySlider() {
            mainObject.destroy()
        }

        function test_01_timeoutZero() {
            console.debug("enter")
            var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:0, appID:1}
            createSlider()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})
            var actualResult = sldUI.slider(initData)
            try {
                compare(actualResult.sliderPosition, initData.position, "slider position")
            } catch(e){}
            destroySlider()
            console.debug("exit")
        }

        function test_02_changePosition() {
            console.debug("enter")
            var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:1, appID:1}
            var expectedResult = {position : 2, resultCode : Common.Result.SUCCESS}
            createSlider()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})
            sliderPopup.onReady.connect(function simulateUserAction(){sliderPopup.position = expectedResult.position})

            sldUI.slider(initData)

            timer.onTriggered()
            try {
                compare(dataContainer.uiSlider.position, expectedResult.position, "slider position")
                compare(sliderPopup.resultCode, expectedResult.resultCode, "result code")
            } catch (e) {}
            destroySlider()
            console.debug("exit")
        }

        function test_03_unChangedPosition() {
            console.debug("enter")
            var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:1, appID:1}
            var expectedResult = {position : 4, resultCode : Common.Result.SUCCESS}
            createSlider()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

            sldUI.slider(initData)

            timer.onTriggered()
            try {
                compare(dataContainer.uiSlider.position, expectedResult.position, "slider position")
                compare(sliderPopup.resultCode, expectedResult.resultCode, "result code")
            } catch (e) {}
            destroySlider()
            console.debug("exit")
        }


        function test_04_changePositionBackPressed() {
            console.debug("enter")
            var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:1, appID:1}
            var expectedResult = {position : 4, resultCode : Common.Result.ABORTED}
            createSlider()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})
            sliderPopup.onReady.connect(function simulateUserAction(){sliderPopup.position = expectedResult.position})

            sldUI.slider(initData)
            sliderPopup.getBackButton().clicked()

            try {
                compare(dataContainer.uiSlider.position, expectedResult.position, "slider position")
                compare(sliderPopup.resultCode, expectedResult.resultCode, "result code")
            } catch (e) {}
            destroySlider()
            console.debug("exit")
        }

        function test_05_secondCall() {
            console.debug("enter")
            var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:1, appID:1}
            var expectedResult = {position : 5, resultCode : Common.Result.ABORTED}
            createSlider()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})
            sliderPopup.onReady.connect(function simulateUserAction(){sliderPopup.position = expectedResult.position})

            sldUI.slider(initData)
            initData.position = 5
            var actualResult = sldUI.slider(initData)

            try {
                compare(actualResult.sliderPosition, expectedResult.position, "slider position")
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}
            destroySlider()
            console.debug("exit")
        }

        function test_06_footerValueNoChange() {
            console.debug("enter")
            var initData = {numTicks:4, position:3, sliderHeader:"header", sliderFooter:["footer1", "footer2", "footer3", "footer4"], timeout:1, appID:1}
            var expectedResult = {position: 3, footers:["footer1", "footer2", "footer3", "footer4"]}
            createSlider()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

            sldUI.slider(initData)

            try {
                compare(sliderPopup.getFooterText().text, expectedResult.footers[expectedResult.position - 1], "current footer")
            } catch (e) {}
            destroySlider()
            console.debug("exit")
        }

        function test_07_footerValueChange() {
            console.debug("enter")
            var initData = {numTicks:4, position:2, sliderHeader:"header", sliderFooter:["footer1", "footer2", "footer3", "footer4"], timeout:1, appID:1}
            var expectedResult = {position: 3, footers:["footer1", "footer2", "footer3", "footer4"]}
            createSlider()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})
            sliderPopup.onReady.connect(function simulateUserAction(){
                var rect = sliderPopup.getBorderRectangle()
                console.debug("visible", sliderPopup.visible)
                mouseClick(rect, rect.width / initData.numTicks * expectedResult.position, 1)
            })

            sldUI.slider(initData)

            try {
                compare(sliderPopup.getFooterText().text, expectedResult.footers[expectedResult.position - 1], "current footer")
            } catch (e) {}
            destroySlider()
            console.debug("exit")
        }

    }
}
