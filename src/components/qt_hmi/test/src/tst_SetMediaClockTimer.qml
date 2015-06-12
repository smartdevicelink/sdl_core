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
    id:item123
    TestCase {
        name: "time"
        property var mainWindowComponent : Qt.createComponent("../qml_model_qt5/MainWindow.qml")
        property var mainObject
        property var sdlUI
        property var dataContainer

        function createTime() {
            mainObject      = mainWindowComponent.createObject(parent)
            sdlUI = mainObject.getUIProxy()
            dataContainer   = mainObject.getDataStorage()
        }

        function destroyTime() {
            mainObject.destroy()
        }

        //No end time, works like a timer
        function test_01_noEndTimeInCountUp() {
            console.debug("enter")

            var initData = {startTime:{hours: 4, minutes: 0, seconds: 0}, endTime:undefined , updateMode: 0, appID: 1}

            var expectedResult = {endTimeResult:-1, resultCode : Common.Result.SUCCESS}

            createTime()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

            var actualResult = sdlUI.setMediaClockTimer(initData)

            try {
                compare(dataContainer.getApplication(initData.appID).mediaClock.endTime, expectedResult.endTimeResult, "time position")
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}

            destroyTime()
            console.debug("exit")
        }

        //No start time. Function process error
        function test_02_noStartTime() {
            console.debug("enter")

            var initData = {startTime:undefined, endTime:{hours: 4, minutes: 0, seconds: 0} , updateMode: 1, appID: 1}
            var expectedResult = {resultCode : Common.Result.INVALID_DATA}

            createTime()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

            var actualResult = sdlUI.setMediaClockTimer(initData)

            try {
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}

            destroyTime()
            console.debug("exit")
        }

        //Receive "pause", when onready paused
        function test_03_pauseAfterPause() {
            console.debug("enter")

            var initData = {startTime:{hours: 4, minutes: 0, seconds: 0}, endTime:{hours: 12, minutes: 00, seconds: 00}, updateMode: 0, appID: 1}
            var expectedResult = {resultCode : Common.Result.IGNORED}

            createTime()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

            sdlUI.setMediaClockTimer(initData)

            initData.updateMode=2
            sdlUI.setMediaClockTimer(initData)

            var actualResult = sdlUI.setMediaClockTimer(initData)

            try {
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}

            destroyTime()
            console.debug("exit")
        }

        //Receive "RESUME" after "CLEAR"
        function test_04_resumeAfterClear() {
            console.debug("enter")

            var initData = {startTime:{hours: 10, minutes: 0, seconds: 0},endTime:{ }, updateMode:0, appID:1}

            var expectedResult = {resultCode : Common.Result.IGNORED}

            createTime()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

            sdlUI.setMediaClockTimer(initData)

            initData.updateMode=4
            sdlUI.setMediaClockTimer(initData)

            initData.updateMode=3
            sdlUI.setMediaClockTimer(initData)

            var actualResult = sdlUI.setMediaClockTimer(initData)
            try {
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}

            destroyTime()
            console.debug("exit")
        }

        function test_05_resumeAfterEnd() {
            console.debug("enter")

            var initData = {startTime:{hours: 0, minutes: 12, seconds: 33},endTime:{hours: 0, minutes: 12, seconds: 33}, updateMode:0, appID:1}

            var expectedResult = {resultCode : Common.Result.IGNORED}

            createTime()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

            sdlUI.setMediaClockTimer(initData)

            dataContainer.setApplicationProperties(initData.appID, {
                "mediaClock": {
                    "updateMode": 0,
                    "runningMode": 1,
                    "startTime": initData.StartTime,
                    "endTime": initData.EndTime,
                    "startTimeForProgress": 753
                }
            })
            initData.updateMode=3
            var actualResult = sdlUI.setMediaClockTimer(initData)

            try {
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}

            destroyTime()
            console.debug("exit")
        }

        //Launch "COUNTUP" twice
        function test_06_twoCountUp() {
            console.debug("enter")

            var initData1 = {startTime:{hours: 4, minutes: 0, seconds: 0}, endTime:{hours: 12, minutes: 00, seconds: 00}, updateMode: 0, appID: 1}
            var initData2 = {startTime:{hours: 5, minutes: 0, seconds: 0}, endTime:{hours: 18, minutes: 00, seconds: 00}, updateMode: 0, appID: 1}
            var expectedResult = {startTime:18000, endTime: 64800, updateMode: 0, resultCode : Common.Result.SUCCESS}

            createTime()
            dataContainer.addApplication({appName:"TestAppName", appId:initData1.appID})

            sdlUI.setMediaClockTimer(initData1)

            var actualResult = sdlUI.setMediaClockTimer(initData2)

            try {
                compare(dataContainer.getApplication(initData1.appID).mediaClock.startTime, expectedResult.startTime, "startTime")
                compare(dataContainer.getApplication(initData1.appID).mediaClock.endTime, expectedResult.endTime, "endTime")
                compare(dataContainer.getApplication(initData1.appID).mediaClock.updateMode, expectedResult.updateMode, "updateMode")
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}

            destroyTime()
            console.debug("exit")
        }

        //Get "RESUME", when clock is paused
        function test_07_resumeAfterResume() {
            console.debug("enter")

            var initData = {startTime:{hours: 4, minutes: 0, seconds: 0}, endTime:{hours: 12, minutes: 00, seconds: 00}, updateMode: 0, appID: 1}
            var expectedResult = {resultCode : Common.Result.IGNORED}

            createTime()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

            sdlUI.setMediaClockTimer(initData)

            initData.updateMode=3
            var actualResult = sdlUI.setMediaClockTimer(initData)

            try {
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}

            destroyTime()
            console.debug("exit")
        }


        function test_08_countDownAfterCountUp() {
            console.debug("enter")

            var initData1 = {startTime:{hours: 4, minutes: 0, seconds: 0}, endTime:{hours: 12, minutes: 00, seconds: 00}, updateMode: 0, appID: 1}
            var initData2 = {startTime:{hours: 18, minutes: 0, seconds: 0}, endTime:{hours: 5, minutes: 00, seconds: 00}, updateMode: 1, appID: 1}
            var expectedResult = {startTime:64800, endTime: 18000, updateMode: 1, resultCode : Common.Result.SUCCESS}

            createTime()
            dataContainer.addApplication({appName:"TestAppName", appId:initData1.appID})

            sdlUI.setMediaClockTimer(initData1)

            var actualResult = sdlUI.setMediaClockTimer(initData2)

            try {
                compare(dataContainer.getApplication(initData1.appID).mediaClock.startTime, expectedResult.startTime, "startTime")
                compare(dataContainer.getApplication(initData1.appID).mediaClock.endTime, expectedResult.endTime, "endTime")
                compare(dataContainer.getApplication(initData1.appID).mediaClock.updateMode, expectedResult.updateMode, "updateMode")
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}

            destroyTime()
            console.debug("exit")
        }

        function test_09_noEndTimeInCountDown() {
            console.debug("enter")

            var initData = {startTime:{hours: 4, minutes: 0, seconds: 0}, endTime:undefined, updateMode: 0, appID: 1}

            var expectedResult = {startTimeForProgress:14400, resultCode : Common.Result.SUCCESS}

            createTime()
            dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

            var actualResult = sdlUI.setMediaClockTimer(initData)

            try {
                compare(dataContainer.getApplication(initData.appID).mediaClock.startTimeForProgress, expectedResult.startTimeForProgress, "time position")
                compare(actualResult.__retCode, expectedResult.resultCode, "result code")
            } catch (e) {}

            destroyTime()
            console.debug("exit")
        }
    }
}
