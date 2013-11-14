import QtTest 1.0
import QtQuick 2.0
import QtMultimedia 5.0
import com.ford.sdl.hmi.dbus_adapter 1.0
import com.ford.sdl.hmi.hw_buttons 1.0
import com.ford.sdl.hmi.log4cxx 1.0
import "./../../../../src/components/qt_hmi/qml_model/controls"
import "./../../../../src/components/qt_hmi/qml_model/views"
import "./../../../../src/components/qt_hmi/qml_model/hmi_api" as HmiApi
import "./../../../../src/components/qt_hmi/qml_model/models"
import "./../../../../src/components/qt_hmi/qml_model/popups"
import "./../../../../src/components/qt_hmi/qml_model/hmi_api/Common.js" as Common
import "./../../../../src/components/qt_hmi/qml_model/models/Constants.js" as Constants
import "./../../../../src/components/qt_hmi/qml_model/models/Internal.js" as Internal

Item {
    TestCase {
        name: "Slider"
        when: windowShown
        property var mainWindowComponent : Qt.createComponent("./../../../../src/components/qt_hmi/qml_model/MainWindow.qml")
        property var mainObject
        property var sdlUIProxy
        property var dataContainer
        property var sliderPopup
        property var timer

        function createSlider() {
            mainObject      = mainWindowComponent.createObject(parent)
            var mainScreen = mainObject.getMainScreen()
            mainScreen.visible = true
            sdlUIProxy      = mainObject.getUIProxy()
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
            var actualResult = sdlUIProxy.slider(initData)
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

            sdlUIProxy.slider(initData)

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

            sdlUIProxy.slider(initData)

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

            sdlUIProxy.slider(initData)
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

            sdlUIProxy.slider(initData)
            initData.position = 5
            var actualResult = sdlUIProxy.slider(initData)

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

            sdlUIProxy.slider(initData)

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

            sdlUIProxy.slider(initData)

            try {
                compare(sliderPopup.getFooterText().text, expectedResult.footers[expectedResult.position - 1], "current footer")
            } catch (e) {}
            destroySlider()
            console.debug("exit")
        }

    }
}
