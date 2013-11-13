import QtQuick 2.0
import QtTest 1.0
import QtQuick 2.0
import QtMultimedia 5.0
import com.ford.sdl.hmi.dbus_adapter 1.0
import com.ford.sdl.hmi.hw_buttons 1.0
import com.ford.sdl.hmi.log4cxx 1.0
import "./controls"
import "./views"
import "./hmi_api" as HmiApi
import "./models"
import "./popups"
import "hmi_api/Common.js" as Common
import "./models/Constants.js" as Constants
import "models/Internal.js" as Internal


TestCase {
    name: "Slider"
    when: windowShown
    property var mainWindowComponent : Qt.createComponent("./MainWindow.qml")
    property var mainObject
    property var sdlUIProxy
    property var dataContainer
    property var silderPopup
    property var timer

    function createSlider() {
        mainObject      = mainWindowComponent.createObject(this)
        sdlUIProxy      = mainObject.test_getUIProxy()
        dataContainer   = mainObject.test_getDataStorage()
        silderPopup     = mainObject.test_getSlider()
        timer           = silderPopup.getTimer()
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
        silderPopup.onReady.connect(function simulateUserAction(){silderPopup.position = expectedResult.position})

        sdlUIProxy.slider(initData)

        timer.onTriggered()
        try {
            compare(dataContainer.uiSlider.position, expectedResult.position, "slider position")
            compare(silderPopup.resultCode, expectedResult.resultCode, "result code")
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
            compare(silderPopup.resultCode, expectedResult.resultCode, "result code")
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
        silderPopup.onReady.connect(function simulateUserAction(){silderPopup.position = expectedResult.position})

        sdlUIProxy.slider(initData)
        silderPopup.getBackButton().clicked()

        try {
            compare(dataContainer.uiSlider.position, expectedResult.position, "slider position")
            compare(silderPopup.resultCode, expectedResult.resultCode, "result code")
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
        silderPopup.onReady.connect(function simulateUserAction(){silderPopup.position = expectedResult.position})

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
            compare(silderPopup.getFooterText().text, expectedResult.footers[expectedResult.position - 1], "current footer")
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
        silderPopup.onReady.connect(function simulateUserAction(){
            //todo (ykazakov): mouse click is not going to target i dont know why.
            var ma = silderPopup.getMouseArea()
            mouseClick(ma.parent, ma.width / initData.numTicks * expectedResult.position, 1)
        })

        sdlUIProxy.slider(initData)

        try {
            compare(silderPopup.getFooterText().text, expectedResult.footers[expectedResult.position - 1], "current footer")
        } catch (e) {}
        destroySlider()
        console.debug("exit")
    }

}
