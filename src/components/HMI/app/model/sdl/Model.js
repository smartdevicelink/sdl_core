/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: ·
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. · Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution. · Neither the name of the Ford Motor Company nor the
 * names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
/**
 * @name SDL.SDLModel
 * @desc General model for SDL applications
 * @category Model
 * @filesource app/model/sdl/SDLModel.js
 * @version 1.0
 */

SDL.SDLModel = Em.Object.create({

    /**
     * Data came from UI.PerformInteractionRequest for ShowVRHelpItems popup
     *
     * @type {Object}
     */
    interactionData: {
        'vrHelpTitle': null,
        'vrHelp': null
    },

    /**
     * IScroll object to manage scroll on PerformInteraction view
     *
     * @type {Object}
     */
    interactionListWrapper: null,

    /**
     * TimeStamp of current started HMI session
     *
     * @type {Number}
     */
    timeStamp: null,

    /**
     * List of VR commands
     */
    VRCommands: [],

    /**
     * Video player object for navigationApp
     *
     * @type {Object}
     */
    naviVideo: null,

    /**
     * Flag to indicate AudioPassThruPopUp activity
     *
     * @type {Boolean}
     */
    AudioPassThruState: false,

    /**
     * Current device information
     *
     * @type {Object}
     */
    CurrDeviceInfo: {
        "name": null,
        "id": null
    },

    /**
     * Driver Distraction State
     *
     * @type bool
     */
    driverDistractionState: false,

    /**
     * Flag to sent Send Data extended params
     *
     * @type {Boolean}
     */
    sendDataExtend: false,

    /**
     * VR active status
     *
     * @type {Boolean}
     */
    VRActive: false,

    /**
     * Flag to be set true when phone call is initialised
     *
     * @type {Boolean}
     */
    phoneCall: false,

    /**
     * Device list search progress flag
     *
     * @param {Boolean}
     */
    deviceSearchProgress: false,

    /**
     * Flag to be set true when VRHelpList are activated
     *
     * @param {Boolean}
     */
    VRHelpListActivated: false,

    /**
     * Flag to be set true when VRHelpList are activated
     *
     * @type {String}
     */
    keyboardInputValue: "",

    /**
     * List of states for OnTBTClientState notification
     */
    tbtClientStates: [
        {
            name: "ROUTE_UPDATE_REQUEST",
            id: 0
        }, {
            name: "ROUTE_ACCEPTED",
            id: 1
        }, {
            name: "ROUTE_REFUSED",
            id: 2
        }, {
            name: "ROUTE_CANCELLED",
            id: 3
        }, {
            name: "ETA_REQUEST",
            id: 4
        }, {
            name: "NEXT_TURN_REQUEST",
            id: 5
        }, {
            name: "ROUTE_STATUS_REQUEST",
            id: 6
        }, {
            name: "ROUTE_SUMMARY_REQUEST",
            id: 7
        }, {
            name: "TRIP_STATUS_REQUEST",
            id: 8
        }, {
            name: "ROUTE_UPDATE_REQUEST_TIMEOUT",
            id: 9
        }
    ],
    /**
     * List of states for ExitApplication notification
     */
    exitAppState: [
        {
            name: "IGNITION_OFF",
            id  : 0
        },
        {
            name: "MASTER_RESET",
            id  : 1
        },
        {
            name: "FACTORY_DEFAULTS",
            id  : 2
        }
    ],

    /**
     * Data for AudioPassThruPopUp that contains params for visualisation
     *
     * @type {Object}
     */
    AudioPassThruData: {},

    /**
     * Enum with result codes for RPC
     */
    resultCode: {
        "SUCCESS"                   : 0,
        "UNSUPPORTED_REQUEST"       : 1,
        "UNSUPPORTED_RESOURCE"      : 2,
        "DISALLOWED"                : 3,
        "REJECTED"                  : 4,
        "ABORTED"                   : 5,
        "IGNORED"                   : 6,
        "RETRY"                     : 7,
        "IN_USE"                    : 8,
        "DATA_NOT_AVAILABLE"        : 9,
        "TIMED_OUT"                 : 10,
        "INVALID_DATA"              : 11,
        "CHAR_LIMIT_EXCEEDED"       : 12,
        "INVALID_ID"                : 13,
        "DUPLICATE_NAME"            : 14,
        "APPLICATION_NOT_REGISTERED": 15,
        "WRONG_LANGUAGE"            : 16,
        "OUT_OF_MEMORY"             : 17,
        "TOO_MANY_PENDING_REQUESTS" : 18,
        "NO_APPS_REGISTERED"        : 19,
        "NO_DEVICES_CONNECTED"      : 20,
        "WARNINGS"                  : 21,
        "GENERIC_ERROR"             : 22,
        "USER_DISALLOWED"           : 23
    },



    /**
     * Info navigationApp data for ShowConstantTBT request
     *
     * @type: {Object}
     */
    constantTBTParams: {
        "navigationTexts":[
            {
                "fieldName": "navigationText1",
                "fieldText": "mainField1"
            },
            {
                "fieldName": "navigationText2",
                "fieldText": "mainField2"
            },
            {
                "fieldName": "ETA",
                "fieldText": "mainField3"
            },
            {
                "fieldName": "totalDistance",
                "fieldText": "mainField4"
            },
            {
                "fieldName": "navigationText",
                "fieldText": "mainField5"
            },
            {
                "fieldName": "timeToDestination",
                "fieldText": "mainField6"
            }
        ],
        "softButtons": [
            {
                "text" : "Menu",
                "isHighlighted" : true,
                "softButtonID" : 1
            },
            {
                "text" : "Custom button",
                "isHighlighted" : false,
                "softButtonID" : 2
            },
            {
                "text" : "+",
                "isHighlighted" : true,
                "softButtonID" : 3
            },
            {
                "text" : "-",
                "isHighlighted" : false,
                "softButtonID" : 4
            }
        ]
    },

    /**
     * List of registered applications, To prevent errors without registered
     * application "-1" used as test appID
     *
     * @type object
     */
    registeredApps: [],

    /**
     * List of registered components
     *
     * @type object
     */
    registeredComponents: [
        {
            type: "UI",
            state: false
        }, {
            type: "TTS",
            state: false
        }, {
            type: "VR",
            state: false
        }, {
            type: "BasicCommunication",
            state: false
        }, {
            type: "VehicleInfo",
            state: false
        }, {
            type: "Buttons",
            state: false
        }, {
            type: "Navigation",
            state: false
        }
    ],

    /**
     * List of icons
     *
     * @type {Object}
     */
    listOfIcons: {
        // appID: syncFileName
        0: "images/media/ico_li.png"
    },

    /**
     * Array of active applications
     *
     * @type {Array}
     */
    applicationsList: [],

    /**
     * Array of connected devices
     *
     * @type {Array}
     */
    devicesList: [],

    /**
     * TTS + VR language
     *
     * @type {String}
     */
    hmiTTSVRLanguage: 'EN-US',

    /**
     * UI language
     *
     * @type {String}
     */
    hmiUILanguage: 'EN-US',

    /**
     * Parameter describes if performInteraction session was started on HMI
     * this flag set to true when UI.PerformInteraction request came on HMI
     * and set to false when HMI send response to SDL Core on UI.PerformInteraction request
     *
     * @type {Boolean}
     */
    performInteractionSession: false,

    /**
     * List of supported languages
     *
     * @type {Array}
     */
    sdlLanguagesList: [
        'EN-US',
        'ES-MX',
        'FR-CA',
        'DE-DE',
        'ES-ES',
        'EN-GB',
        'RU-RU',
        'TR-TR',
        'PL-PL',
        'FR-FR',
        'IT-IT',
        'SV-SE',
        'PT-PT',
        'NL-NL',
        'ZH-TW',
        'JA-JP',
        'AR-SA',
        'KO-KR',
        'PT-BR',
        'CS-CZ',
        'DA-DK',
        'NO-NO'
    ],

    /**
     * Method to open Phone view and dial phone number
     *
     * @param {Object}
     */
    dialNumber: function(params) {

        this.set('phoneCall', true);
        SDL.States.goToStates('phone.dialpad');
        SDL.PhoneModel.set('dialpadNumber', params.number);
        SDL.PhoneController.onDialCall();
    },

    /**
     * Notification method to send touch event data to SDLCore
     *
     * @param {Object}
     */
    onTouchEvent: function(event){

        if (event.target.parentElement.className.indexOf("navButton") >= 0 || event.target.className.indexOf("navButton") >= 0) {
            return;
        }

        var type = "",
            changedTouches = event.originalEvent.changedTouches ? event.originalEvent.changedTouches.length : 1;

        switch (event.originalEvent.type) {
            case "touchstart": {
                FLAGS.TOUCH_EVENT_STARTED = true;
                type = "BEGIN";
                break;
            }
            case "touchmove": {
                type = "MOVE";
                break;
            }
            case "touchend": {
                type = "END";
                break;
            }
            case "mousedown": {
                FLAGS.TOUCH_EVENT_STARTED = true;
                type = "BEGIN";
                break;
            }
            case "mousemove": {
                type = "MOVE";
                break;
            }
            case "mouseup": {
                type = "END";
                break;
            }
        }

        if (FLAGS.TOUCH_EVENT_STARTED ) {

            var events = [];
            for(var i = 0; i < changedTouches; i++){

                if (event.originalEvent.changedTouches && (event.originalEvent.changedTouches[i].pageX > SDL.SDLVehicleInfoModel.vehicleData.displayResolution.width || event.originalEvent.changedTouches[i].pageY > SDL.SDLVehicleInfoModel.vehicleData.displayResolution.height)) {
                    return;
                }

                events[i] = {};
                events[i].c = [{}];

                events[i].id  = event.originalEvent.changedTouches ? event.originalEvent.changedTouches[i].identifier : 0;
                events[i].c[0].x = event.originalEvent.changedTouches ? event.originalEvent.changedTouches[i].pageX : event.originalEvent.pageX;
                events[i].c[0].y = event.originalEvent.changedTouches ? event.originalEvent.changedTouches[i].pageY : event.originalEvent.pageY;
                events[i].ts  = [event.timeStamp - SDL.SDLModel.timeStamp];



            }
            FFW.UI.onTouchEvent(type, events);
        }

        if (event.originalEvent.type == "mouseup") {
            FLAGS.TOUCH_EVENT_STARTED = false;
        }
    },

    /**
     * Method to start playing video from streaming video source
     * provided by SDLCore
     *
     * @param {Object}
     */
    startStream: function(params) {

        SDL.SDLController.getApplicationModel(params.appID).set('navigationStream', params.url);
        SDL.SDLModel.playVideo();
    },

    /**
     * Method to stop playing video streaming
     *
     * @param {Number}
     */
    stopStream: function(appID) {

        var createVideoView =  Ember.View.create({
                templateName: "video",
                template: Ember.Handlebars.compile('<video id="html5Player"></video>')
            }),
            videoChild = null;

        SDL.NavigationAppView.removeChild(SDL.NavigationAppView.get('videoView'));
        SDL.NavigationAppView.rerender();

        SDL.SDLController.getApplicationModel(appID).set('navigationStream', null);

        //this.pauseVideo();

        videoChild = SDL.NavigationAppView.createChildView(createVideoView);

        SDL.NavigationAppView.get('childViews').pushObject(videoChild);
        SDL.NavigationAppView.set('videoView', videoChild);
    },

    /**
     * Method to start playing audio from streaming audio source
     * provided by SDLCore
     *
     * @param {Object}
     */
    startAudioStream: function(params) {

        SDL.SDLController.getApplicationModel(params.appID).set('navigationAudioStream', params.url);
        SDL.StreamAudio.play(params.url);
    },

    /**
     * Method to set navigationApp streaming url to current app model
     *
     * @param {Number}
     */
    stoptAudioStream: function(appID) {

        SDL.SDLController.getApplicationModel(appID).set('navigationAudioStream', null);
        SDL.StreamAudio.stop();
    },

    /**
     * Method to reset navigationApp streaming url from current app model
     */
    playVideo: function(){
        if (SDL.SDLAppController.model.navigationStream !== null) {

            SDL.SDLModel.naviVideo = document.getElementById("html5Player");
            SDL.SDLModel.naviVideo.src = SDL.SDLAppController.model.navigationStream;
            SDL.SDLModel.naviVideo.play();
        }
    },

    /**
     * Video player trigger to stop playing video
     */
    pauseVideo: function(){
        if (SDL.SDLModel.naviVideo != null) {

            SDL.SDLModel.naviVideo.pause();
        }
    },

    /**
     * Method to open Turn By Turn view
     *
     * @param {Object}
     */
    tbtActivate: function(params) {

        SDL.SDLController.getApplicationModel(params.appID).set('constantTBTParams', params);
        SDL.SDLController.getApplicationModel(params.appID).set('tbtActivate', true);

        if (SDL.SDLAppController.model) {
            SDL.SDLController.activateTBT();
        }
    },

    /**
     * Method to set data for Turn List in applications model
     *
     * @param {Object}
     */
    tbtTurnListUpdate: function(params) {

        SDL.SDLController.getApplicationModel(params.appID).turnList = params.turnList;
        SDL.SDLController.getApplicationModel(params.appID).turnListSoftButtons = params.softButtons;
        SDL.TBTTurnList.updateList(params.appID);
    },

    /**
     * Method to VRHelpList on UI with request parameters
     * It opens VrHelpList PopUp with current list of readable VR commands
     *
     * @param {Object}
     */
    ShowVrHelp: function(vrHelpTitle, vrHelp) {

        SDL.VRHelpListView.showVRHelp(vrHelpTitle, vrHelp);
    },

    /**
     * Method to set language for UI component with parameters sent from
     * SDLCore to UIRPC
     *
     * @type {String} lang
     */
    changeRegistrationUI: function (lang, appID) {

        if (SDL.SDLController.getApplicationModel(appID)) {
            SDL.SDLController.getApplicationModel(appID).set('UILanguage', lang);
        }
    },

    /**
     * Method to set language for TTS and VR components with parameters sent
     * from SDLCore to UIRPC
     *
     * @type {String} lang
     */
    changeRegistrationTTSVR: function (lang, appID) {

        if (SDL.SDLController.getApplicationModel(appID)) {
            SDL.SDLController.getApplicationModel(appID).set('TTSVRLanguage', lang);
        }
    },

    /**
     * Method to add activation button to VR commands and set device
     * parameters to model
     *
     * @param {Object}
     */
    onAppRegistered: function (params) {

        var applicationType = 1;

        if (SDL.SDLController.getApplicationModel(params.application.appID)) {
            return;
        }

        if (params.ttsName) {
            var message = {"cmdID": 0, "vrCommands": [params.ttsName.text], "appID": params.application.appID, "type": "Application"};
            this.addCommandVR(message);
        }

        if (params.vrSynonyms) {

            var message = {"cmdID": 0, "vrCommands": params.vrSynonyms, "appID": params.application.appID, "type": "Application"};
            this.addCommandVR(message);
        }

        if (params.application.isMediaApplication) {
            applicationType = 0;
        }

        SDL.SDLController.registerApplication(params.application, applicationType);
    },

    /**
     * Method to delete activation button from VR commands and delete device
     * parameters from model
     *
     * @param {Object}
     */
    onAppUnregistered: function (params) {

        if (SDL.SDLController.getApplicationModel(params.appID)) {

            if (SDL.SDLController.getApplicationModel(params.appID).activeRequests.uiPerformInteraction) {
                SDL.InteractionChoicesView.deactivate("ABORTED");
            }
            SDL.SDLController.unregisterApplication(params.appID);
        }
    },

    /**
     * SDL UI ScrolableMessage activation function dependent of Driver
     * Distraction toggle state
     *
     * @param {Object}
     *            params Object with parameters come from SDLCore.
     * @param {Number}
     *            messageRequestId Identification of unique request
     */
    onSDLScrolableMessage: function (request, messageRequestId) {

        if (!SDL.ScrollableMessage.active) {
            if (SDL.SDLModel.driverDistractionState) {
                SDL.DriverDistraction.activate();
                FFW.UI.sendError(SDL.SDLModel.resultCode["REJECTED"], request.id, request.method, 'DD mode is active!');
            } else {
                SDL.ScrollableMessage.activate(SDL.SDLController.getApplicationModel(request.params.appID).appName, request.params, messageRequestId);
            }
        } else {
            FFW.UI.sendError(SDL.SDLModel.resultCode["REJECTED"], request.id, request.method, 'Higher priority request is being processed on HMI!');
        }

    },

    /**
     * setGlobalProperties
     *
     * @param {Object}
     *            message Object with parameters come from SDLCore.
     */
    setProperties: function(params) {

        for (var i in params) {
            if (i === "keyboardProperties") {
                if (params[i].language) {
                    SDL.SDLController.getApplicationModel(params.appID).set('globalProperties.keyboardProperties.language', params[i].language);
                }
                if (params[i].keyboardLayout) {
                    SDL.SDLController.getApplicationModel(params.appID).set('globalProperties.keyboardProperties.keyboardLayout', params[i].keyboardLayout);
                }
                if (params[i].keypressMode) {
                    SDL.SDLController.getApplicationModel(params.appID).set('globalProperties.keyboardProperties.keypressMode', params[i].keypressMode);
                }
                if (params[i].limitedCharacterList) {
                    SDL.SDLController.getApplicationModel(params.appID).set('globalProperties.keyboardProperties.limitedCharacterList', params[i].limitedCharacterList);
                } else {
                    SDL.SDLController.getApplicationModel(params.appID).set('globalProperties.keyboardProperties.limitedCharacterList', []);
                }
                if (params[i].autoCompleteText) {
                    SDL.SDLController.getApplicationModel(params.appID).set('globalProperties.keyboardProperties.autoCompleteText', params[i].autoCompleteText);
                }
            } else {
                SDL.SDLController.getApplicationModel(params.appID).set('globalProperties.' + i, params[i]);
            }
        }

    },

    /**
     * Method to call handler from model to show list of avaliable
     * applications
     *
     * @param {Object}
     *            appList
     */
    onGetAppList: function (appList) {

        var i = 0, len = appList.length;
        for (i = 0; i < len; i++) {
            if (appList[i]) {
                SDL.SDLModel.onAppRegistered(appList[i]);
            }
        }

    },

    /**
     * Method to call function from DeviceListView to show list of connected
     * devices
     *
     * @param {Object}
     *            params
     */
    onGetDeviceList: function (params) {

        if (SDL.States.info.devicelist.active && params.deviceList && params.deviceList.length) {
            SDL.DeviceListView.ShowDeviceList(params);
        }

        SDL.SDLModel.set('deviceSearchProgress', false);
    },

    /**
     * SDL UI SetAppIcon handler
     *
     * @param {Object}
     *            message
     * @param {Number}
     *            id
     * @param {String}
     *            method
     */
    onSDLSetAppIcon: function (message, id, method) {

        if (!SDL.SDLController.getApplicationModel(message.appID)){
            FFW.UI.sendUIResult(SDL.SDLModel.resultCode["APPLICATION_NOT_REGISTERED"], id, method);
        } else {

            var img = new Image();
            img.onload = function () {

                // code to set the src on success
                SDL.SDLController.getApplicationModel(message.appID).set('appIcon', message.syncFileName.value);
                FFW.UI.sendUIResult(SDL.SDLModel.resultCode["SUCCESS"], id, method);
            };
            img.onerror = function (event) {

                // doesn't exist or error loading
                FFW.UI.sendError(SDL.SDLModel.resultCode["INVALID_DATA"], id, method, 'Image does not exist!');
                return false;
            };

            img.src = message.syncFileName.value;
        }
    },

    /**
     * SDL UI Alert response handler show popup window
     *
     * @param {Object}
     *            message Object with parameters come from SDLCore
     * @param {Number}
     *            alertRequestId Id of current handled request
     */
    onUIAlert: function (message, alertRequestId) {

        if (!SDL.AlertPopUp.active) {
            SDL.AlertPopUp.AlertActive(message, alertRequestId);
        } else {
            SDL.SDLController.alertResponse(this.resultCode['REJECTED'], alertRequestId);
        }
    },

    /**
     * SDL UI PerformInteraction response handler show popup window
     *
     * @param {Object}
     *            message Object with parameters come from SDLCore
     * @param {Number}
     *            performInteractionRequestId Id of current handled request
     */
    uiPerformInteraction: function (message) {

        this.set('performInteractionSession', true);

        if (!SDL.SDLController.getApplicationModel(message.params.appID).activeRequests.uiPerformInteraction) {
            SDL.SDLController.getApplicationModel(message.params.appID).activeRequests.uiPerformInteraction = message.id;
        } else {
            SDL.SDLController.interactionChoiseCloseResponse(message.appID, SDL.SDLModel.resultCode['REJECTED']);
            return;
        }

        if (message.params && message.params.vrHelpTitle && message.params.vrHelp) {

            SDL.SDLModel.set('interactionData.vrHelpTitle', message.params.vrHelpTitle);
            SDL.SDLModel.set('interactionData.vrHelp', message.params.vrHelp);
        }

        SDL.InteractionChoicesView.activate(message);

        SDL.SDLController.VRMove();

        if (message.choiceSet) {
            SDL.VRPopUp.hideCommands();
        }
    },

    /**
     * SDL UI Slider response handler show popup window
     *
     * @param {Object}
     *            message Object with parameters come from SDLCore
     */
    uiSlider: function (message) {

        if (!SDL.SliderView.active) {
            SDL.SDLController.getApplicationModel(message.params.appID).onSlider(message);
        } else {
            FFW.UI.sendSliderResult(this.resultCode["ABORTED"], message.id);
        }
    },

    /**
     * SDL ShowKeyboard show method with incoming parameters of layout, language etc...
     *
     * @param {Object}
     *            message Object with parameters come from SDLCore
     */
    uiShowKeyboard: function(element){
        SDL.Keyboard.activate(element);
    },

    /**
     * SDL UI AudioPassThru response handler show popup window
     *
     * @param {Object}
     *            message Object with parameters come from SDLCore.
     */
    UIPerformAudioPassThru: function(message) {

        this.set('AudioPassThruData', message);
        this.set('AudioPassThruState', true);
    },

    /**
     * Method ends processing of AudioPassThru and call AudioPassThru UI
     * response handler
     */
    UIEndAudioPassThru: function () {

        if (this.AudioPassThruState) {
            FFW.UI.sendUIResult(this.resultCode["SUCCESS"], FFW.UI.endAudioPassThruRequestID, "UI.EndAudioPassThru");
            SDL.SDLController.performAudioPassThruResponse(this.resultCode["SUCCESS"]);
        } else {
            FFW.UI.sendError(this.resultCode["GENERIC_ERROR"], FFW.UI.endAudioPassThruRequestID, "UI.EndAudioPassThru", "UI.PerformAudioPassThru are not processed at the moment!");
        }
    },

    /**
     * Prompt activation
     *
     * @param {Object}
     * @param {Number}
     */
    onPrompt: function (ttsChunks) {

        var message = '';
        if (ttsChunks) {
            for (var i = 0; i < ttsChunks.length; i++) {
                message += ttsChunks[i].text + '\n';
            }
            SDL.TTSPopUp.ActivateTTS(message);
        }
    },

    /**
     * Play audio file on PlayTone notification
     */
    onPlayTone: function () {

        SDL.Audio.play('audio/initial.wav');
    },

    /**
     * Prompt deactivation
     */
    TTSStopSpeaking: function () {
        //true parameter makes send error response ABORTED
        SDL.TTSPopUp.DeactivateTTS();
        FFW.TTS.set('aborted', true);
    },

    /**
     * SDL DeleteCommand response handler to sent delete command error or normal result
     *
     * @param {Number}
     * @param {Number}
     */
    deleteCommandResponse: function (resultCode, requestID) {

        if (resultCode === SDL.SDLModel.resultCode["SUCCESS"]) {
            FFW.UI.sendUIResult(resultCode, requestID, "UI.DeleteCommand");
        } else {
            FFW.UI.sendError(resultCode, requestID, "UI.DeleteCommand", "SubMenu is opened")
        }
    },

    /**
     * SDL VR AddCommand response handler add command to voice recognition
     * window
     *
     * @param {Object}
     */
    addCommandVR: function (message) {

        var appID = 0;
        if (message.type == "Application") {

            SDL.SDLModel.VRCommands.push(message);
            SDL.VRPopUp.AddCommand(message.cmdID, message.vrCommands, message.appID, message.type);
        } else if ("appID" in message) {

            appID = message.appID;

            SDL.SDLController.getApplicationModel(appID).VRCommands.push(message);

            if (SDL.SDLAppController.model && SDL.SDLAppController.model.appID) {

                SDL.VRPopUp.AddCommand(message.cmdID, message.vrCommands, appID, message.type);
            }
        } else {

            SDL.SDLModel.VRCommands.push(message);
            SDL.VRPopUp.AddCommand(message.cmdID, message.vrCommands, appID, message.type);
        }
    },

    /**
     * SDL VR DeleteCommand response handler delete command from voice
     * recognition window
     *
     * @param {Number}
     */
    deleteCommandVR: function (params) {

        SDL.VRPopUp.DeleteCommand(params.cmdID, params.appID);

        var len = SDL.SDLController.getApplicationModel(params.appID).VRCommands.length;

        for (var i = len - 1; i >= 0 ; i--) {
            if (SDL.SDLController.getApplicationModel(params.appID).VRCommands[i].appID == params.appID &&
                SDL.SDLController.getApplicationModel(params.appID).VRCommands[i].cmdID == params.cmdID) {
                SDL.SDLController.getApplicationModel(params.appID).VRCommands.splice(i, 1);
            }
        }
    },
//
//    /**
//     * SDL VR DeleteCommand response handler delete command from voice
//     * recognition window
//     *
//     * @param {Number}
//     */
//    deleteCommandVR: function (commandID) {
//
//        SDL.VRPopUp.DeleteCommand(commandID);
//    },

    onDeactivateApp: function (target, appID) {

        if (SDL.SDLController.getApplicationModel(appID)) {
            /**
             * Close Options menu if opened
             */
            if (SDL.OptionsView.active) {
                SDL.OptionsView.set('active', false);
            }

            var dest = target.split('.'), reason;

            switch (dest[0]) {
                case 'media':
                {
                    reason = 'AUDIO';
                    break;
                }
                case "phone":
                {
                    reason = 'PHONEMENU';
                    break;
                }
                case "navigation":
                {
                    reason = 'NAVIGATIONMAP';
                    break;
                }
                case "settings":
                {
                    reason = 'SYNCSETTINGS';
                    break;
                }
                default:
                {
                    reason = 'GENERAL';
                    break;
                }
            }

            SDL.TurnByTurnView.deactivate();

            //if (!SDL.SDLController.getApplicationModel(appID).unregistered) {
                FFW.BasicCommunication.OnAppDeactivated(reason, appID);
            //}
        }
    }
});
