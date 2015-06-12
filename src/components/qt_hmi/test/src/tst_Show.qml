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
    width : mainWindowLoader.width
    height: mainWindowLoader.height
    Loader {
        id: mainWindowLoader
        asynchronous : false
    }

    TestCase {
        name: "Show"
        when: windowShown
        property var mainWindowComponent : Qt.createComponent("../qml_model_qt5/MainWindow.qml")
        property var mainObject
        property var sldUI
        property var dataContainer
        property var contentLoader

        property var softButtonsListExample : [
            {softButtonID:0, isHighlighted:true, systemAction:Common.SystemAction.DEFAULT_ACTION,
                type:Common.SoftButtonType.SBT_TEXT, text:"Default Action"},
            {softButtonID:1, isHighlighted:false, systemAction:Common.SystemAction.STEAL_FOCUS,
                type:Common.SoftButtonType.SBT_IMAGE, text:"Steal Focus"},
            {softButtonID:2, isHighlighted:false, systemAction:Common.SystemAction.KEEP_CONTEXT,
                type:Common.SoftButtonType.SBT_BOTH, text:"Keep context"}]


        //initialization for each test
        function createShow(appID) {
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
            sldUI = mainWindowObject.getUIProxy()
            dataContainer   = mainWindowObject.getDataStorage()
            dataContainer.addApplication({appName:"TestAppName", appId:appID})
        }



        //cleanup for each test
        function destroyShow() {
            //Clear Loader with MainWindow, which delete own models and views
            mainWindowLoader.source = ""
        }


        //Fill up all fields, check save process to dataContainer
        function test_01_create() {
            console.debug("enter")
            var initData  = {
                appID:1,
                showStrings:[
                    {fieldName:Common.TextFieldName.mainField1 ,fieldText:"showString1"},
                    {fieldName:Common.TextFieldName.mainField2,fieldText:"showString2"},
                    {fieldName:Common.TextFieldName.mainField3,fieldText:"showString3"},
                    {fieldName:Common.TextFieldName.mainField4,fieldText:"showString4"},
                    {fieldName:Common.TextFieldName.mediaTrack,fieldText:"mediaTrack"},
                    {fieldName:Common.TextFieldName.statusBar,fieldText:"StatusBar"},
                  {fieldName:Common.TextFieldName.mediaClock,fieldText:"12:34"}
                            ],
                softButtons:softButtonsListExample,
                alignment:2,
                graphic:{value:"/home/user/applink/bin/SPTAlice9675308/action.png",imageType: "DYNAMIC"},
                secondaryGraphic:{value:"/home/user/applink/bin/SPTAlice9675308/action.png",imageType: "DYNAMIC"},
                customPresets:["GEN0","GEN1","GEN2","GEN3"]
                            }

            createShow(1)
            var app = dataContainer.getApplication(initData.appID)
            sldUI.show (initData)

            try {

                compare(app.hmiUIText.mainField1 , initData.showStrings[0].fieldText, "mainField1")
                compare(app.hmiUIText.mainField2 , initData.showStrings[1].fieldText, "mainField2")
                compare(app.hmiUIText.mainField3 , initData.showStrings[2].fieldText, "mainField3")
                compare(app.hmiUIText.mainField4 , initData.showStrings[3].fieldText, "mainField4")
                compare(app.hmiUIText.mediaTrack , initData.showStrings[4].fieldText, "mediaTrack")
                compare(app.hmiUIText.statusBar  , initData.showStrings[5].fieldText, "StatusBar" )
                compare(app.hmiUIText.mediaClock , initData.showStrings[6].fieldText, "mediaClock")
                var a;
                if(app.hmiUITextAlignment === 1) a=0;
                if(app.hmiUITextAlignment === 2) a=1;
                if(app.hmiUITextAlignment === 4) a=2;
                compare(initData.alignment, a, "Alignment")
                compare(app.hmiUIText.image, initData.graphic.value, "image")
                compare(app.hmiUIText.secondaryImage, initData.secondaryGraphic.value, "secdondImage")
                compare(app.softButtons.count, initData.softButtons.length, "wrong buttons count created")
                for (var i = 0, len = app.softButtons.count; i < len; i++)
                {
                    var act = app.softButtons.get(i)
                    var exp = initData.softButtons[i]
                    compare(act.softButtonID, exp.softButtonID, "wrong softButtonID in button")
                    compare(act.isHighlighted, exp.isHighlighted, "wrong isHighlighted in button")
                    compare(act.systemAction, exp.systemAction, "wrong systemAction in button")
                    compare(act.text, exp.text, "wrong text in button")
                }
                compare(app.customPresets.count, initData.customPresets.length, "wrong buttons count created")
                for (var j = 0, len2 = app.customPresets.count; j < len; j++)
                {
                    var act2 = app.customPresets.get(j)
                    var exp2 = initData.customPresets[j]
                    compare(act2.text, exp2, "wrong text in button")
                }
            }
            catch (e) {}
            destroyShow()
            console.debug("exit")
                        }
          //All fields are empty
          function test_02_emptyInit()
          {
              console.debug("enter")
              var initData  = {
                  appID:1,
                  showStrings:[
                      {fieldName:Common.TextFieldName.mainField1 ,fieldText:""},
                      {fieldName:Common.TextFieldName.mainField2,fieldText:""},
                      {fieldName:Common.TextFieldName.mainField3,fieldText:""},
                      {fieldName:Common.TextFieldName.mainField4,fieldText:""},
                      {fieldName:Common.TextFieldName.mediaTrack,fieldText:""},
                      {fieldName:Common.TextFieldName.statusBar,fieldText:""},
                      {fieldName:Common.TextFieldName.mediaClock,fieldText:""}
                              ],
                 softButtons:[],
                  alignment:0,
                  graphic:{value:"",imageType: "DYNAMIC"},
                  secondaryGraphic:{value:"",imageType: "DYNAMIC"},
                  customPresets:[]
                              }

              createShow(1)
              var app = dataContainer.getApplication(initData.appID)
              sldUI.show (initData)

              try {

                  compare(app.hmiUIText.mainField1 , initData.showStrings[0].fieldText, "mainField1")
                  compare(app.hmiUIText.mainField2 , initData.showStrings[1].fieldText, "mainField2")
                  compare(app.hmiUIText.mainField3 , initData.showStrings[2].fieldText, "mainField3")
                  compare(app.hmiUIText.mainField4 , initData.showStrings[3].fieldText, "mainField4")
                  compare(app.hmiUIText.mediaTrack , initData.showStrings[4].fieldText, "mediaTrack")
                  compare(app.hmiUIText.statusBar , initData.showStrings[5].fieldText,  "StatusBar" )
                  compare(app.hmiUIText.mediaClock , initData.showStrings[6].fieldText, "mediaClock")
                  var a;
                  if(app.hmiUITextAlignment === 1) a=0;
                  if(app.hmiUITextAlignment === 2) a=1;
                  if(app.hmiUITextAlignment === 4) a=2;
                  compare(initData.alignment, a, "Alignment")
                  compare(app.hmiUIText.image, initData.graphic.value, "image")
                  compare(app.hmiUIText.secondaryImage, initData.secondaryGraphic.value, "secdondImage")
                  compare(app.softButtons.count, initData.softButtons.length, "wrong buttons count created")
                  for (var i = 0, len = app.softButtons.count; i < len; i++)
                  {
                      var act = app.softButtons.get(i)
                      var exp = initData.softButtons[i]
                      compare(act.softButtonID, exp.softButtonID, "wrong softButtonID in button")
                      compare(act.isHighlighted, exp.isHighlighted, "wrong isHighlighted in button")
                      compare(act.systemAction, exp.systemAction, "wrong systemAction in button")
                      compare(act.text, exp.text, "wrong text in button")
                  }
                  compare(app.customPresets.count, initData.customPresets.length, "wrong buttons count created")
                  for (var j = 0, len2 = app.customPresets.count; j < len; j++)
                  {
                      var act2 = app.customPresets.get(j)
                      var exp2 = initData.customPresets[j]
                      compare(act2.text, exp2, "wrong text in button")
                  }
              }
              catch (e) {}
              destroyShow()
              console.debug("exit")
          }

    //Mandatory fields only
    function test_03_create_mandatory_only() {
        console.debug("enter")
        var initData  = {
            appID:1,
            showStrings:[
                {fieldName:Common.TextFieldName.mainField1 ,fieldText:"showString1"},
                {fieldName:Common.TextFieldName.mainField2,fieldText:"showString2"},
                {fieldName:Common.TextFieldName.mainField3,fieldText:"showString3"},
                {fieldName:Common.TextFieldName.mainField4,fieldText:"showString4"},
                {fieldName:Common.TextFieldName.mediaTrack,fieldText:"mediaTrack"},
                {fieldName:Common.TextFieldName.statusBar,fieldText:"StatusBar"},
              {fieldName:Common.TextFieldName.mediaClock,fieldText:"12:34"}
                        ]
                        }

        createShow(1)
        var app = dataContainer.getApplication(initData.appID)
        sldUI.show (initData)

        try {

            compare(app.hmiUIText.mainField1 , initData.showStrings[0].fieldText, "mainField1")
            compare(app.hmiUIText.mainField2 , initData.showStrings[1].fieldText, "mainField2")
            compare(app.hmiUIText.mainField3 , initData.showStrings[2].fieldText, "mainField3")
            compare(app.hmiUIText.mainField4 , initData.showStrings[3].fieldText, "mainField4")
            compare(app.hmiUIText.mediaTrack , initData.showStrings[4].fieldText, "mediaTrack")
            compare(app.hmiUIText.statusBar  , initData.showStrings[5].fieldText, "StatusBar" )
            compare(app.hmiUIText.mediaClock , initData.showStrings[6].fieldText, "mediaClock")

        }
        catch (e) {}
        destroyShow()
        console.debug("exit")
                    }
    //Field with extra param
    function test_04_fake_param() {
        console.debug("enter")
        var initData  = {
            appID:1,
            showStrings:[
                {fieldName:Common.TextFieldName.mainField1 ,fieldText:"showString1"},
                {fieldName:Common.TextFieldName.mainField2,fieldText:"showString2"},
                {fieldName:Common.TextFieldName.mainField3,fieldText:"showString3"},
                {fieldName:Common.TextFieldName.mainField4,fieldText:"showString4"},
                {fieldName:Common.TextFieldName.mediaTrack,fieldText:"mediaTrack"},
                {fieldName:Common.TextFieldName.statusBar,fieldText:"StatusBar",fakeParam:"FakeParam"},
                {fieldName:Common.TextFieldName.mediaClock,fieldText:"12:34"}
                        ]
                        }

        createShow(1)
        var app = dataContainer.getApplication(initData.appID)
        sldUI.show (initData)

        try {

            compare(app.hmiUIText.mainField1 , initData.showStrings[0].fieldText, "mainField1")
            compare(app.hmiUIText.mainField2 , initData.showStrings[1].fieldText, "mainField2")
            compare(app.hmiUIText.mainField3 , initData.showStrings[2].fieldText, "mainField3")
            compare(app.hmiUIText.mainField4 , initData.showStrings[3].fieldText, "mainField4")
            compare(app.hmiUIText.mediaTrack , initData.showStrings[4].fieldText, "mediaTrack")
            compare(app.hmiUIText.statusBar  , initData.showStrings[5].fieldText, "StatusBar" )
            compare(app.hmiUIText.mediaClock , initData.showStrings[6].fieldText, "mediaClock")

        }
        catch (e) {}
        destroyShow()
        console.debug("exit")
    }
    //aligment of mainField1, mainField2
    function test_05_alignment() {
        console.debug("enter")
        var initData  = {
            appID:1,
            showStrings:[],
            alignment:0,
                        }

        createShow(1)
        var app = dataContainer.getApplication(initData.appID)
        sldUI.show (initData)

        try {
            //Magic with digits - because enum of text.Alighnment enum and sdl alignment doesn't match
            var a;
            if(app.hmiUITextAlignment === 1) a=0;
            if(app.hmiUITextAlignment === 2) a=1;
            if(app.hmiUITextAlignment === 4) a=2;
            compare(initData.alignment, a, "Alignment")
            initData.alignment=1
            sldUI.show (initData)
            var b;
            if(app.hmiUITextAlignment === 1) b=0;
            if(app.hmiUITextAlignment === 2) b=1;
            if(app.hmiUITextAlignment === 4) b=2;
            compare(initData.alignment, b, "Alignment")
            initData.alignment=2
            sldUI.show (initData)
            var c;
            if(app.hmiUITextAlignment === 1) c=0;
            if(app.hmiUITextAlignment === 2) c=1;
            if(app.hmiUITextAlignment === 4) c=2;
            compare(initData.alignment, c, "Alignment")
        }
        catch (e) {}
        destroyShow()
        console.debug("exit")
                    }
    //pole bez parametra
    function test_06_no_param() {
        console.debug("enter")
        var initData  = {
            appID:1,
            showStrings:[],
            graphic:{imageType: "DYNAMIC"},
                        }

        createShow(1)
        var app = dataContainer.getApplication(initData.appID)
        sldUI.show (initData)

        try {

            compare(app.hmiUIText.image, undefined, "image")

        }
        catch (e) {}
        destroyShow()
        console.debug("exit")
     }
}
}
