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

SDL.SDLModel = Em.Object
    .create( {

        /**
         * Flag to indicate AudioPassThruPopUp activity
         * 
         * @param {Boolean}
         */
        AudioPassThruState: false,

        /**
         * Current device information
         * 
         * @param {Object}
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
         * @param {Boolean}
         */
        sendDataExtend: false,

        /**
         * VR active status
         * 
         * @param {Boolean}
         */
        VRActive: false,

        /**
         * Flag to be set true when phone call is initialised
         * 
         * @param {Boolean}
         */
        phoneCall: false,

        /**
         * Default values for global properties
         */
        globalPropertiesDefault: {
            helpPrompt: [
                {
                    "text": "Some text for help prompt",
                    "type": "TEXT"
                }
            ],

            timeoutPrompt: [
                {
                    "text": "Some text for timeout prompt",
                    "type": "TEXT"
                }
            ]
        },

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
                id: 0
            }, {
                name: "MASTER_RESET",
                id: 1
            }, {
                name: "FACTORY_DEFAULTS",
                id: 2
            }
        ],

        /**
         * Enum with result codes for RPC
         */
        resultCode: {
            "SUCCESS": 0,
            "UNSUPPORTED_REQUEST": 1,
            "UNSUPPORTED_RESOURCE": 2,
            "DISALLOWED": 3,
            "REJECTED": 4,
            "ABORTED": 5,
            "IGNORED": 6,
            "RETRY": 7,
            "IN_USE": 8,
            "DATA_NOT_AVAILABLE": 9,
            "TIMED_OUT": 10,
            "INVALID_DATA": 11,
            "CHAR_LIMIT_EXCEEDED": 12,
            "INVALID_ID": 13,
            "DUPLICATE_NAME": 14,
            "APPLICATION_NOT_REGISTERED": 15,
            "WRONG_LANGUAGE": 16,
            "OUT_OF_MEMORY": 17,
            "TOO_MANY_PENDING_REQUESTS": 18,
            "NO_APPS_REGISTERED": 19,
            "NO_DEVICES_CONNECTED": 20,
            "WARNINGS": 21,
            "GENERIC_ERROR": 22,
            "USER_DISALLOWED": 23
        },

        /**
         * Data for AudioPassThruPopUp that contains params for visualisation
         * 
         * @type {Object}
         */
        AudioPassThruData: {},

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
         * Global properties
         * 
         * @type {Object}
         */
        globalProperties: {
            helpPrompt: [],
            timeoutPrompt: []
        },

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
         * Method to open Turn By Turn view
         * 
         * @param {Object}
         */
        tbtActivate: function(params) {

            SDL.TurnByTurnView.activate(params);
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
         * 
         * @param {Object}
         */
        ShowVrHelp: function(params) {

            SDL.VRHelpListView.showVRHelp(params);
        },

        /**
         * Method to set language for UI component with parameters sent from
         * SDLCore to UIRPC
         * 
         * @type {String} lang
         */
        changeRegistrationUI: function(lang) {

            SDL.SDLAppController.model.set('UILanguage', lang);
        },

        /**
         * Method to set language for TTS and VR components with parameters sent
         * from SDLCore to UIRPC
         * 
         * @type {String} lang
         */
        changeRegistrationTTSVR: function(lang) {

            SDL.SDLAppController.model.set('TTSVRLanguage', lang);
        },

        /**
         * Method to add activation button to VR commands and set device
         * parameters to model
         * 
         * @param {Object}
         */
        onAppRegistered: function(params) {

            var applicationType = 1;

            if (SDL.SDLController.getApplicationModel(params.appID)) { return; }

            if (params.isMediaApplication) {
                applicationType = 0;
            }

            SDL.SDLController.registerApplication(params, applicationType);
        },

        /**
         * Method to delete activation button from VR commands and delete device
         * parameters from model
         * 
         * @param {Object}
         */
        onAppUnregistered: function(params) {

            if (SDL.SDLController.getApplicationModel(params.appID)) {

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
        onSDLScrolableMessage: function(request, messageRequestId) {

            if (!SDL.ScrollableMessage.active) {
                if (SDL.SDLModel.driverDistractionState) {
                    SDL.DriverDistraction.activate();
                    FFW.UI.sendError(SDL.SDLModel.resultCode["REJECTED"],
                        request.id,
                        request.method,
                        'DD mode is active!');
                } else {
                    SDL.ScrollableMessage.activate(SDL.SDLController
                        .getApplicationModel(request.params.appID).appName,
                        request.params,
                        messageRequestId);
                }
            } else {
                FFW.UI.sendError(SDL.SDLModel.resultCode["REJECTED"],
                    request.id,
                    request.method,
                    'Higher priority request is being processed on HMI!');
            }

        },

        /**
         * Handler for reset globalProperties
         * 
         * @param {Object}
         */
        resetProperties: function(params) {

            var i, len = params.properties.length;
            for (i = 0; i < len; i++) {
                if (params.properties[i] == "HELPPROMPT") {
                    this.set('globalProperties.helpPrompt',
                        this.globalPropertiesDefault.helpPrompt);
                }

                if (params.properties[i] == "TIMEOUTPROMPT") {
                    this.set('globalProperties.timeoutPrompt',
                        this.globalPropertiesDefault.timeoutPrompt);
                }
            }
        },

        /**
         * setGlobalProperties
         * 
         * @param {Object}
         *            message Object with parameters come from SDLCore.
         */
        setProperties: function(message) {

            this.set('globalProperties.helpPrompt', message.helpPrompt);
            this.set('globalProperties.timeoutPrompt', message.timeoutPrompt);

        },

        /**
         * Method to call handler from model to show list of avaliable
         * applications
         * 
         * @param {Object}
         *            appList
         */
        onGetAppList: function(appList) {

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
        onGetDeviceList: function(params) {

            if (SDL.States.info.devicelist.active && params.deviceList
                && params.deviceList.length) {
                SDL.DeviceListView.ShowDeviceList(params);
            }
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
        onSDLSetAppIcon: function(message, id, method) {

            var img = new Image();
            img.onload = function() {

                // code to set the src on success
                SDL.SDLController.getApplicationModel(message.appID)
                    .set('appIcon', message.syncFileName.value);
                FFW.UI.sendUIResult(SDL.SDLModel.resultCode["SUCCESS"],
                    id,
                    method);
            };
            img.onerror = function(event) {

                // doesn't exist or error loading
                FFW.UI.sendError(SDL.SDLModel.resultCode["INVALID_DATA"],
                    id,
                    method,
                    'Image does not exist!');
                return false;
            };

            img.src = message.syncFileName.value;
        },

        /**
         * SDL UI Alert Maneuver response handler show popup window
         * 
         * @param {Object}
         *            message Object with parameters come from SDLCore
         */
        onNavigationAlertManeuver: function(message) {

            SDL.AlertManeuverPopUp.AlertManeuverActive(message);
        },

        /**
         * SDL UI Alert response handler show popup window
         * 
         * @param {Object}
         *            message Object with parameters come from SDLCore
         * @param {Number}
         *            alertRequestId Id of current handled request
         */
        onUIAlert: function(message, alertRequestId) {

            if (!SDL.AlertPopUp.active) {
                SDL.AlertPopUp.AlertActive(message, alertRequestId);
            } else {
                SDL.SDLController.alertResponse(this.resultCode['REJECTED'],
                    alertRequestId);
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
        uiPerformInteraction: function(message, performInteractionRequestId) {

            if (!SDL.InteractionChoicesView.active) {
                SDL.SDLController.getApplicationModel(message.appID)
                    .onPreformInteraction(message, performInteractionRequestId);
            } else {
                SDL.SDLController.interactionChoiseCloseResponse('ABORTED',
                    performInteractionRequestId);
            }
        },

        /**
         * SDL UI Slider response handler show popup window
         * 
         * @param {Object}
         *            message Object with parameters come from SDLCore
         */
        uiSlider: function(message) {

            if (!SDL.SliderView.active) {
                SDL.SDLController.getApplicationModel(message.params.appID)
                    .onSlider(message);
            } else {
                FFW.UI.sendSliderResult('ABORTED', message.id);
            }
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
        UIEndAudioPassThru: function() {

            if (this.AudioPassThruState) {
                FFW.UI.sendUIResult(this.resultCode["SUCCESS"],
                    FFW.UI.endAudioPassThruRequestID,
                    "UI.EndAudioPassThru");
                SDL.SDLController
                    .performAudioPassThruResponse(this.resultCode["SUCCESS"]);
            } else {
                FFW.UI.sendUIResult(this.resultCode["GENERIC_ERROR"],
                    FFW.UI.endAudioPassThruRequestID,
                    "UI.EndAudioPassThru");
            }
        },

        /**
         * Prompt activation
         * 
         * @param {Object}
         * @param {Number}
         */
        onPrompt: function(ttsChunks) {

            var message = '';
            if (ttsChunks) {
                for ( var i = 0; i < ttsChunks.length; i++) {
                    message += ttsChunks[i].text + '\n';
                }
                SDL.TTSPopUp.ActivateTTS(message);
            }
        },

        /**
         * Play audio file on PlayTone notification
         */
        onPlayTone: function() {

            SDL.Audio.play('audio/initial.wav');
        },

        /**
         * Prompt deactivation
         */
        TTSStopSpeaking: function() {

            SDL.TTSPopUp.DeactivateTTS();
        },

        /**
         * SDL VR AddCommand response handler add command to voice recognition
         * window
         * 
         * @param {Object}
         */
        addCommandVR: function(message) {

            var appId = 0;
            if ("appID" in message) {
                appId = message.appID;
            }

            SDL.VRPopUp.AddCommand(message.cmdID, message.vrCommands, appId);
        },

        /**
         * SDL VR DeleteCommand response handler delete command from voice
         * recognition window
         * 
         * @param {Number}
         */
        deleteCommandVR: function(commandID) {

            SDL.VRPopUp.DeleteCommand(commandID);
        },

        onDeactivateApp: function(target, appID) {

            if (SDL.SDLController.getApplicationModel(appID)) {
                /**
                 * Close Options menu if opened
                 */
                if (SDL.OptionsView.active) {
                    SDL.OptionsView.set('active', false);
                }

                var dest = target.split('.'), reason;

                switch (dest[0]) {
                case 'media': {
                    reason = 'AUDIO';
                    break;
                }
                case "phone": {
                    reason = 'PHONEMENU';
                    break;
                }
                case "navigation": {
                    reason = 'NAVIGATIONMAP';
                    break;
                }
                case "settings": {
                    reason = 'SYNCSETTINGS';
                    break;
                }
                default: {
                    reason = 'GENERAL';
                    break;
                }
                }

                FFW.BasicCommunication.OnAppDeactivated(reason, appID);
            }
        }
    });
