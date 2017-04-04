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
     * Structure specified for PoliceUpdate retry sequence
     * contains timeout seconds param, array of retry seconds and counter of number of retries
     *
     * @type {Objetc}
     */
    policyUpdateRetry:{
        timeout: null,
        retry: [],
        try: null,
        timer: null,
        oldTimer: 0
    },

    /**
     * Application's container for current processed requests on VR component of HMI
     *
     * @type {Object}
     */
    vrActiveRequests: {
        vrPerformInteraction: null
    },

    /**
     * List of callback functions for request SDL.GetUserFriendlyMessage
     * where key is requestId
     * and parameter is a function that will handle data came in respone from SDL
     *
     * @type {Object}
     */
    userFriendlyMessagePull: {},

    /**
     * List of appID functions for request SDL.GetListOfPermissions
     * where key is requestId
     * and parameter is a appID that will handle data came in respone from SDL
     *
     * @type {Object}
     */
    getListOfPermissionsPull: {},

    /**
     * List of application id's for request SDL.ActivateApp
     * where key is requestId
     * and parameter is a id of application to be activated
     *
     * @type {Object}
     */
    activateAppRequestsList: {},

    /**
     * ID of app in LIMITED HMI state
     */
    stateLimited: null,

    /**
     * Active state of phone call on HMI for Deactivate app to handle event
     */
    phoneCallActive: false,

    /**
     * FLAG of any app in limited level exists
     */
    limitedExist: false,

    applicationStatusBar: function () {

        if (this.limitedExist && SDL.SDLController.getApplicationModel(this.stateLimited)) {
            return SDL.SDLController.getApplicationModel(this.stateLimited).statusText;
        } else {
            return '';
        }
    }.property("this.limitedExist"),

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
    naviVideo: {},

/**
     * Array of strings came in SDL.GetURLS response
     *
     * @type {Object}
     */
    policyURLs: [],

    /**
     * Policy Settings Info state value
     *
     * @type {String}
     */
    settingsInfoListState: 'iAPP_BUFFER_FULL',

    /**
     * Policy Settings Info list
     *
     * @type {Object}
     */
    settingsInfoList: [
        'iAPP_BUFFER_FULL',
        'blah'
    ],

    /**
     * Policy Settings Info state value
     *
     * @type {String}
     */
    systemErrorListState: 'SYNC_REBOOTED',

    /**
     * Policy Settings Info list
     *
     * @type {Object}
     */
    systemErrorList: [
        'SYNC_REBOOTED',
        'SYNC_OUT_OF_MEMMORY'
    ],

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
        },
        {
            name: "SUSPEND",
            id  : 3
        }
    ],

    /**
     * List of states for OnSystemRequest notification
     */
    systemRequestState: [
        {
            name: "HTTP",
            id  : 0
        },
        {
            name: "FILE_RESUME",
            id  : 1
        },
        {
            name: "AUTH_REQUEST",
            id  : 2
        },
        {
            name: "AUTH_CHALLENGE",
            id  : 3
        },
        {
            name: "AUTH_ACK",
            id  : 4
        },
        {
            name: "PROPRIETARY",
            id  : 5
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
     * List of unregistered applications, to verify which app is reestablished connection
     *
     * @type object
     */
    unRegisteredApps: [],

    /**
     * List of applications to show on appList view came in BC.UpdateAppList
     *
     * @type object
     */
    updatedAppsList: [],

    /**
     * List of objects with params for connected devices
     *
     * @type object
     */
    connectedDevices: {},

    /**
     * List of devices with registered applications
     *
     * @type object
     */
    conectedDevices: [],

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
    defaultListOfIcons: {
        // appID: syncFileName
        //0: "images/media/ico_li.png"
        'app': 'images/info/info_leftMenu_apps_ico.png',
        'command': 'images/common/defaultButtonImage.png',
        'trackIcon': 'images/sdl/audio_icon.jpg'
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
    performInteractionSession: [],

/**
     * Array with app permissions
     * used for policies
     *
     * @type {Object}
     */
    appPermissions: [],

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
     * Function make diff between two arrays of permissions
     * remove argument array from existed array of permissions
     */
    setAppPermissions: function(appID, permissions){

        var messageCodes = [];

        permissions.forEach(function (x) {
            messageCodes.push(x.name);
        });

        messageCodes.push("AppPermissionsRevoked");

        FFW.BasicCommunication.GetUserFriendlyMessage(SDL.SettingsController.simpleParseUserFriendlyMessageData, appID, messageCodes);

    },

    /**
     * Method to set selected state of settings Info List
     */
    settingsInfoListStateChange: function() {

        FFW.BasicCommunication.AddStatisticsInfo(this.settingsInfoListState);
    }.observes('this.settingsInfoListState'),

    /**
     * Method to set selected state of settings Info List
     */
    systemErrorListStateChange: function() {

        FFW.BasicCommunication.OnSystemError(this.systemErrorListState);
    }.observes('this.systemErrorListState'),

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
     * Method to remove deleted by SDL Core images used in HMI
     * check images came in request from SDLCore like UI.Show, UI.AddCommand, UI.SetGlobalProperties,
     * UI.SeAppIcon, Navigation.ShowConstantTBT, Navigation.UpdateTurnList, UI.ShowNotification
     *
     * @param {Object}
     */
    onFileRemoved: function(params) {

        var result = false;

        if ((params.fileType === "GRAPHIC_PNG" || params.fileType === "GRAPHIC_BMP" || params.fileType === "GRAPHIC_JPEG") && SDL.SDLController.getApplicationModel(params.appID)) {
            result = SDL.SDLController.getApplicationModel(params.appID).onImageRemoved(params.fileName);

            if (SDL.SDLController.getApplicationModel(params.appID).appIcon.indexOf(params.fileName) != -1
                && params.fileName.length == SDL.SDLController.getApplicationModel(params.appID).appIcon.length) {
                SDL.SDLController.getApplicationModel(params.appID).set('appIcon', SDL.SDLModel.defaultListOfIcons.app);
            }

            if (SDL.SDLController.getApplicationModel(params.appID).constantTBTParams) {

                if (SDL.SDLController.getApplicationModel(params.appID).constantTBTParams.turnIcon
                    && SDL.SDLController.getApplicationModel(params.appID).constantTBTParams.turnIcon.value.indexOf(params.fileName) != -1
                    && params.fileName.length == SDL.SDLController.getApplicationModel(params.appID).constantTBTParams.turnIcon.value.length) {
                    SDL.SDLController.getApplicationModel(params.appID).constantTBTParams.turnIcon.value = SDL.SDLModel.defaultListOfIcons.command;
                    SDL.TurnByTurnView.activate(params.appID);
                }

                if (SDL.SDLController.getApplicationModel(params.appID).constantTBTParams.nextTurnIcon
                    && SDL.SDLController.getApplicationModel(params.appID).constantTBTParams.nextTurnIcon.value.indexOf(params.fileName) != -1
                    && params.fileName.length == SDL.SDLController.getApplicationModel(params.appID).constantTBTParams.nextTurnIcon.value.length) {
                    SDL.SDLController.getApplicationModel(params.appID).constantTBTParams.nextTurnIcon.value = SDL.SDLModel.defaultListOfIcons.command;
                    SDL.TurnByTurnView.activate(params.appID);
                }
            }

            if (SDL.SDLAppController.model.appInfo.trackIcon && SDL.SDLAppController.model.appInfo.trackIcon.indexOf(params.fileName) != -1
                && params.fileName.length == SDL.SDLAppController.model.appInfo.trackIcon.length) {
                SDL.SDLAppController.model.appInfo.set('trackIcon', SDL.SDLModel.defaultListOfIcons.trackIcon);
            }

            if (SDL.SDLAppController.model.appInfo.mainImage && SDL.SDLAppController.model.appInfo.mainImage.indexOf(params.fileName) != -1
                && params.fileName.length == SDL.SDLAppController.model.appInfo.mainImage.length) {
                SDL.SDLAppController.model.appInfo.set('mainImage', SDL.SDLModel.defaultListOfIcons.trackIcon);
            }

            var len = SDL.SDLController.getApplicationModel(params.appID).turnList.length;
            for (var i = 0; i < len; i++) {
                if (!SDL.SDLController.getApplicationModel(params.appID).turnList[i].turnIcon) {
                    continue;
                }
                if (SDL.SDLController.getApplicationModel(params.appID).turnList[i].turnIcon.value.indexOf(params.fileName) != -1
                    && params.fileName.length == SDL.SDLController.getApplicationModel(params.appID).turnList[i].turnIcon.value.length) {
                    SDL.SDLController.getApplicationModel(params.appID).turnList[i].turnIcon.value = SDL.SDLModel.defaultListOfIcons.command;
                }
            }

            SDL.TBTTurnList.updateList(params.appID);

            if (SDL.SDLController.getApplicationModel(params.appID).softButtons) {
                var len = SDL.SDLController.getApplicationModel(params.appID).softButtons.length;
                for (var i = 0; i < len; i++) {
                    if (!SDL.SDLController.getApplicationModel(params.appID).softButtons[i].image) {
                        continue;
                    }
                    if (SDL.SDLController.getApplicationModel(params.appID).softButtons[i].image.value.indexOf(params.fileName) != -1
                        && params.fileName.length == SDL.SDLController.getApplicationModel(params.appID).softButtons[i].image.value.length) {
                        SDL.SDLController.getApplicationModel(params.appID).softButtons[i].image.value = SDL.SDLModel.defaultListOfIcons.command;
                    }
                }

                if (params.appID == SDL.SDLAppController.model.appID) {
                    SDL.sdlView.innerMenu.refreshItems();
                }
            }

            var len = SDL.VRHelpListView.helpList.items.length;
            for (var i = 0; i < len; i++) {
                if (!SDL.VRHelpListView.helpList.items[i].params.icon) {
                    continue;
                }
                if (SDL.VRHelpListView.helpList.items[i].params.icon.indexOf(params.fileName) != -1
                    && params.fileName.length == SDL.VRHelpListView.helpList.items[i].params.icon.length) {
                    SDL.VRHelpListView.helpList.items[i].params.icon = SDL.SDLModel.defaultListOfIcons.command;
                }
            }

            SDL.VRHelpListView.helpList.list.refresh();

            var len = SDL.InteractionChoicesView.listOfChoices.items.length;
            for (var i = 0; i < len; i++) {
                if (!SDL.InteractionChoicesView.listOfChoices.items[i].params.icon) {
                    continue;
                }
                if (SDL.InteractionChoicesView.listOfChoices.items[i].params.icon.indexOf(params.fileName) != -1
                    && params.fileName.length == SDL.InteractionChoicesView.listOfChoices.items[i].params.icon.length) {
                    SDL.InteractionChoicesView.listOfChoices.items[i].params.icon = SDL.SDLModel.defaultListOfIcons.command;
                }
            }

            SDL.InteractionChoicesView.listOfChoices.list.refresh();

            var len = SDL.InteractionChoicesView.listWrapper.naviChoises._childViews.length;
            for (var i = 0; i < len; i++) {
                if (!SDL.InteractionChoicesView.listWrapper.naviChoises._childViews[i].icon) {
                    continue;
                }
                if (SDL.InteractionChoicesView.listWrapper.naviChoises._childViews[i].icon.indexOf(params.fileName) != -1
                    && params.fileName.length == SDL.InteractionChoicesView.listWrapper.naviChoises._childViews[i].icon.length) {
                    SDL.InteractionChoicesView.listWrapper.naviChoises._childViews[i].icon = SDL.SDLModel.defaultListOfIcons.command;
                }
            }

            SDL.InteractionChoicesView.listWrapper.naviChoises.rerender();

        }


        if (result && SDL.OptionsView.active && SDL.SDLAppController.model.appID == params.appID) {
            SDL.OptionsView.commands.refreshItems();
        }
    },

    /**
     * Method to start playing video from streaming video source
     * provided by SDLCore
     *
     * @param {Object}
     */
    startStream: function(request) {

        var text = "Would you like to start Video stream?";

        SDL.PopUp.create().appendTo('body').popupActivate(text, function(result){
            if (result) {

                SDL.SDLController.getApplicationModel(request.params.appID).set('navigationStream', request.params.url);
                SDL.SDLModel.playVideo(request.params.appID);

                FFW.Navigation.sendNavigationResult(
                    SDL.SDLModel.resultCode["SUCCESS"],
                    request.id,
                    request.method
                );

            } else if (result === false) {

                FFW.Navigation.sendError(
                    SDL.SDLModel.resultCode["REJECTED"],
                    request.id,
                    request.method,
                    "Ignored by USER!"
                );
            }
        });

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

        SDL.NavigationAppView.videoView.remove();
        SDL.NavigationAppView.videoView.destroy();

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

        var text = "Would you like to start Audio stream?";

        SDL.PopUp.create().appendTo('body').popupActivate(text, function(result){
            if (result) {

                SDL.SDLController.getApplicationModel(params.appID).set('navigationAudioStream', params.url);
                SDL.StreamAudio.play(params.url);

                FFW.Navigation.sendNavigationResult(
                    SDL.SDLModel.resultCode["SUCCESS"],
                    request.id,
                    request.method
                );

            } else if (result === false) {

                FFW.Navigation.sendError(
                    SDL.SDLModel.resultCode["REJECTED"],
                    request.id,
                    request.method,
                    "Ignored by USER!"
                );
            }
        });
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
    playVideo: function(appID){
        if (SDL.SDLController.getApplicationModel(appID).navigationStream !== null) {

            SDL.SDLModel.naviVideo = document.getElementById("html5Player");
            SDL.SDLModel.naviVideo.src = SDL.SDLController.getApplicationModel(appID).navigationStream;
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

        SDL.SDLController.getApplicationModel(params.appID).turnList = params.turnList ? params.turnList : [];
        SDL.SDLController.getApplicationModel(params.appID).turnListSoftButtons = params.softButtons ? params.softButtons : [];
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
    changeRegistrationUI: function (lang, appID, appName) {

        if (SDL.SDLController.getApplicationModel(appID)) {
            SDL.SDLController.getApplicationModel(appID).set('UILanguage', lang);
        }

        if (appName) {
            SDL.SDLMediaController.currentAppId = 0;
            SDL.SDLController.getApplicationModel(appID).appName = appName;
            SDL.SDLMediaController.set('currentAppId', appID);
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

        var applicationType = null,//Default value - NonMediaModel see SDL.SDLController.applicationModels
            app = SDL.SDLController.getApplicationModel(params.appID);

        if (app != undefined && app.initialized == false) {

            if (app.isMedia != params.isMediaApplication) { // If current not initialized model doe not matches the registered application type
                this.convertModel(params);                   // then model should be changed
            } else {
                app.disabledToActivate = params.disabled;
            }
            return;
        } else if (app != undefined && app.initialized == true) {
            console.error("Application with appID " + params.appID + " already registered!");
            return; // if application already registered and correctly initialized and BC.UpdateAppList came from SDL than nothing shoul happend
        }

        if (params.vrSynonyms) {

            var message = {"cmdID": 0, "vrCommands": params.vrSynonyms, "appID": params.appID, "type": "Application"};
            this.addCommandVR(message);
        }

        if (params.isMediaApplication === true) {

            applicationType = 0;
        } else if (params.isMediaApplication === false) {

            applicationType = 1;
        }

        SDL.SDLController.registerApplication(params, applicationType);

        if (SDL.SDLModel.unRegisteredApps.indexOf(params.appID) >= 0) {
            setTimeout(function(){ SDL.PopUp.create().appendTo('body').popupActivate("Connection with " + params.appName + "  is re-established.")}, 1000);
            this.unRegisteredApps.pop(params.appID);
        }
    },

    /**
     * Method to convert existed model to registered type
     */
    convertModel: function(params) {

        SDL.SDLModel.get('registeredApps').removeObjects(SDL.SDLModel.get('registeredApps').filterProperty('appID', params.appID));

        this.onAppRegistered(params);
    },

    /**
     * Method to delete activation button from VR commands and delete device
     * parameters from model
     *
     * @param {Object}
     */
    onAppUnregistered: function (params) {

        if (SDL.SDLController.getApplicationModel(params.appID)) {

            if (params.unexpectedDisconnect) {
                SDL.PopUp.create().appendTo('body').popupActivate("The connection with the " + SDL.SDLController.getApplicationModel(params.appID).appName + " was unexpectedly lost.");
                this.unRegisteredApps.push(params.appID);
            }

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
            return true;
        } else {
            FFW.UI.sendError(SDL.SDLModel.resultCode["REJECTED"], request.id, request.method, 'Higher priority request is being processed on HMI!');
            return false;
        }
    },

    /**
     * setGlobalProperties
     *
     * @param {Object}
     *            message Object with parameters come from SDLCore.
     */
    setProperties: function(params) {

        if (SDL.SDLController.getApplicationModel(params.appID)) {
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
                    } else {
                        SDL.SDLController.getApplicationModel(params.appID).set('globalProperties.keyboardProperties.autoCompleteText', '');
                    }
                } else {
                    SDL.SDLController.getApplicationModel(params.appID).set('globalProperties.' + i, params[i]);
                }
            }
        } else {
            console.error("CriticalError! No app registered with current appID!");
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

        //SDL.SDLModel.set('connectedDevices', params.deviceList);
        var exist = false;

        for (var i = 0; i < params.deviceList.length; i++) {

            if (params.deviceList[i].id in SDL.SDLModel.connectedDevices) {
                exist = true;
            }

            if (!exist) {
                SDL.SDLModel.connectedDevices[params.deviceList[i].id] = {
                    "name": params.deviceList[i].name,
                    "id": params.deviceList[i].id,
                    "sdlFunctionality": {
                        "popUpId": null,
                        "allowed": false
                    }
                }
            } else {

                exist = false;
            }
        }

        if (SDL.States.info.devicelist.active) {
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
            return true;
        } else {
            SDL.SDLController.alertResponse(this.resultCode['REJECTED'], alertRequestId);
            return false;
        }
    },

    /**
     * SDL UI PerformInteraction response handler show popup window
     *
     * @param {Object}
     *            message Object with parameters come from SDLCore
     */
    uiPerformInteraction: function (message) {

        if (!SDL.SDLController.getApplicationModel(message.params.appID).activeRequests.uiPerformInteraction) {
            SDL.SDLController.getApplicationModel(message.params.appID).activeRequests.uiPerformInteraction = message.id;

            if (message.params && message.params.vrHelpTitle && message.params.vrHelp) {

                SDL.SDLModel.set('interactionData.vrHelpTitle', message.params.vrHelpTitle);
                SDL.SDLModel.set('interactionData.vrHelp', message.params.vrHelp);
            }

            SDL.InteractionChoicesView.activate(message);
            SDL.SDLController.VRMove();

            return true;
        } else {

            FFW.UI.sendError(SDL.SDLModel.resultCode['REJECTED'], message.id, message.method, "UI PerformInterection REJECTED on HMI");
            return false;
        }
    },

    /**
     * SDL VR PerformInteraction response handler
     *
     * @param {Object}
     *            message Object with parameters come from SDLCore
     */
    vrPerformInteraction: function (message) {

        if (!SDL.SDLModel.vrActiveRequests.vrPerformInteraction) {
            SDL.SDLModel.vrActiveRequests.vrPerformInteraction = message.id;
        } else {
            FFW.VR.sendError(SDL.SDLModel.resultCode['REJECTED'], message.id, message.method, "VR PerformInterection REJECTED on HMI")
            return;
        }

        setTimeout(function(){
            if (SDL.SDLModel.vrActiveRequests.vrPerformInteraction) {
                SDL.SDLModel.onPrompt(message.params.timeoutPrompt);
            }
        }, message.params.timeout - 2000); //Magic numer is a platform depended HMI behavior: -2 seconds for timeout prompt

        SDL.SDLModel.onPrompt(message.params.initialPrompt);

        SDL.SDLModel.interactionData.helpPrompt = message.params.helpPrompt;

        if (message.params.grammarID) {

            this.set('performInteractionSession', message.params.grammarID);
            SDL.SDLModel.set('VRActive', true);

            setTimeout(function(){
                if (SDL.SDLModel.VRActive) {
                    if (SDL.SDLAppController.model && SDL.SDLModel.vrActiveRequests.vrPerformInteraction) {
                        SDL.SDLController.vrInteractionResponse(SDL.SDLModel.resultCode['TIMED_OUT']);
                    }

                    SDL.SDLModel.set('VRActive', false);
                }
            }, message.params.timeout);

            SDL.InteractionChoicesView.timerUpdate();
        } else {

            SDL.SDLController.vrInteractionResponse(SDL.SDLModel.resultCode['SUCCESS']);
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
            return true;
        } else {
            FFW.UI.sendSliderResult(this.resultCode["REJECTED"], message.id);
            return false;
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
            FFW.UI.sendError(this.resultCode["REJECTED"], FFW.UI.endAudioPassThruRequestID, "UI.EndAudioPassThru", "UI.PerformAudioPassThru are not processed at the moment!");
        }
    },

    /**
     * Prompt activation
     *
     * @param {Object}
     * @param {Number}
     */
    onPrompt: function (ttsChunks, appID) {

        var message = '';
        if (ttsChunks) {
            for (var i = 0; i < ttsChunks.length; i++) {
                message += ttsChunks[i].text + '\n';
            }
            SDL.TTSPopUp.ActivateTTS(message, appID);
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
        FFW.TTS.set('aborted', true);
        SDL.TTSPopUp.DeactivateTTS();
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

        if (message.type == "Application") {

            SDL.SDLModel.VRCommands.push(message);
            SDL.VRPopUp.AddCommand(message.cmdID, message.vrCommands, message.appID, message.type);
        } else if ("appID" in message) {

            SDL.SDLController.getApplicationModel(message.appID).VRCommands.push(message);

            if (SDL.SDLAppController.model && SDL.SDLAppController.model.appID == message.appID) {

                SDL.VRPopUp.AddCommand(message.cmdID, message.vrCommands, message.appID, message.type, message.grammarID);
            }
        } else {

            SDL.SDLModel.VRCommands.push(message);
            SDL.VRPopUp.AddCommand(message.cmdID, message.vrCommands, 0, message.type, message.grammarID);
        }
    },

    /**
     * SDL VR DeleteCommand response handler delete command from voice
     * recognition window
     *
     * @param {Number}
     */
    deleteCommandVR: function (request) {

        var appModel = SDL.SDLController.getApplicationModel(request.params.appID);

        if (appModel.currentSubMenuId != 'top') {
            for (var i in appModel.commandsList) {
                if (appModel.commandsList[i].filterProperty('commandID', request.params.cmdID).length) {
                    if (i == appModel.currentSubMenuId) {
                        
                        FFW.VR.sendError(SDL.SDLModel.resultCode["IN_USE"], request.id, request.method, "SubMenu is currently opened on UI!");
                        return;
                    }
                }
            }
        }

        SDL.VRPopUp.DeleteCommand(request.params.cmdID, request.params.appID);
        var len = appModel.VRCommands.length;

        for (var i = len - 1; i >= 0 ; i--) {
            if (appModel.VRCommands[i].appID == request.params.appID &&
                appModel.VRCommands[i].cmdID == request.params.cmdID) {
                appModel.VRCommands.splice(i, 1);
            }
        }

        FFW.VR.sendVRResult(SDL.SDLModel.resultCode["SUCCESS"],
            request.id,
            request.method);


    },

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
                case "call":
                {
                    reason = 'PHONECALL';
                    break;
                }
                default:
                {
                    reason = 'GENERAL';
                    break;
                }
            }

            if (SDL.SDLModel.stateLimited && reason === 'AUDIO') {

                SDL.SDLModel.stateLimited = null;
                SDL.SDLModel.set('limitedExist', false);
            }

            SDL.TurnByTurnView.deactivate();

            if (!SDL.SDLModel.phoneCallActive) {
                FFW.BasicCommunication.OnAppDeactivated(reason, appID);
            }
        }
    }
});
