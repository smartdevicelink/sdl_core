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
        name: "DeleteSubMenu"
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


        function destroyView() {
            //Clear Loader with MainWindow, which delete own models and views
            mainWindowLoader.source = ""
        }

        //Positive case
        function test_01_deleteSubMenu()
        {
            console.debug("enter")
            var initData = {
                menuID: 1000,
                appID: 1
            }

            createMessageView(initData.appID)
            sdlUIProxy.deleteSubMenu(initData)
            var app = dataContainer.getApplication(initData.appID)
            compare ( initData.menuID, app.options.get(0).id, "SubMenu ID")

            destroyView()
            console.debug("exit")

        }


    }

}
