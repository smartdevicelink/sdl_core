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
    property var mainWindowComponent : Qt.createComponent("./MainWindow.qml")
    property var mainObject          : mainWindowComponent.createObject(this)
    property var sdlUIProxy          : mainObject.test_getUIProxy()
    property var dataContainer       : mainObject.test_getDataStorage()
    property var silderPopup         : mainObject.test_getSlider("aaa")

    Timer {
        id: testTimer
        property var callBack
        property var expectedResult
        interval: 0
        onTriggered: {
            callBack()
        }

    }

    function test_01_SliderTimeoutZero() {
        console.debug("enter")
        var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:0, appID:1}
        dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})
        var actualResult = sdlUIProxy.slider(initData)
        compare(actualResult.sliderPosition, initData.position, "slider position")
        console.debug("exit")
    }

    function sliderChangePositionFinish(){
        compare(dataContainer.uiSlider.position, testTimer.expectedResult, "slider position")
    }

    function test_02_SliderChangePosition() {
        console.debug("enter")
        var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:10, appID:1}
        dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})
        testTimer.interval = initData.timeout * 2
        testTimer.callBack = sliderChangePositionFinish
        testTimer.expectedResult = silderPopup.position

        sdlUIProxy.slider(initData)
        silderPopup.position = 2

        testTimer.start()
        console.debug("exit")
    }

    function test_03_SliderUnChangePosition() {
        console.debug("enter")
        var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:10, appID:1}
        dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})
        testTimer.interval = initData.timeout * 2
        testTimer.callBack = sliderChangePositionFinish
        testTimer.expectedResult = initData.position

        sdlUIProxy.slider(initData)

        testTimer.start()
        console.debug("exit")
    }


    function test_04_SliderChangePositionBackPressed() {
        console.debug("enter")
        var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:10, appID:1}
        dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})

        sdlUIProxy.slider(initData)
        silderPopup.position = 2
        silderPopup.complete(Common.Result.ABORTED)

        compare(dataContainer.uiSlider.position, initData.position, "slider position")
        console.debug("exit")
    }

    function sliderSecondCallFinish1() {
        var initData = {numTicks:10, position:5, sliderHeader:"header1", sliderFooter:["footer1"], timeout:5, appID:1}
        try {
            sdlUIProxy.slider(initData)
        } catch (e) {
            console.debug("exception")
        }
    }

    function test_05_SliderSecondCall() {
        console.debug("enter")
        var initData = {numTicks:9, position:4, sliderHeader:"header", sliderFooter:["footer"], timeout:100, appID:1}
        dataContainer.addApplication({appName:"TestAppName", appId:initData.appID})
        testTimer.interval = initData.timeout / 2
        testTimer.callBack = sliderSecondCallFinish1

        sdlUIProxy.slider(initData)

        testTimer.start()
        console.debug("exit")
    }

    //test footers
    //test different  app id

}
