/**
 * @name MFT.SDLController
 * @desc Main SDL Controller
 * @category Controller
 * @filesource app/controller/sdl/SDLController.js
 * @version 1.0
 * @author Artem Petrosyan
 */

MFT.SDLController = Em.Object.create( {

    /**
     * Driver Distraction State
     * 
     * @type bool
     */
    driverDistractionState: false,

    /**
     * Protocol Version 2 State
     * 
     * @type bool
     */
    protocolVersion2State: false,

    /**
     * Current system context
     * 
     * @type {String}
     */
    sysContext: function() {

        if (MFT.VRPopUp.VRActive) {
            return 'VRSESSION';
        }

        if (MFT.AlertPopUp.active) {
            return 'ALERT';
        }

        if (MFT.TBTClientStateView.active || MFT.VehicleInfo.active || MFT.DriverDistraction.active) {
            return 'HMI_OBSCURED';
        }

        if (MFT.OptionsView.active) {
            return 'MENU';
        }

        if (MFT.States.info.nonMedia.active || MFT.States.media.sdlmedia.active) {
            return 'MAIN';
        } else {
            return 'MENU';
        }

    }.property('MFT.DriverDistraction.active', 'MFT.OptionsView.active', 'MFT.VRPopUp.VRActive', 'MFT.AlertPopUp.active', 'MFT.TBTClientStateView.active',
                    'MFT.VehicleInfo.active', 'MFT.States.info.nonMedia.active', 'MFT.States.media.sdlmedia.active'),

    /**
     * List of SDL application models
     * 
     * @type object
     */
    applicationModels: {
        0: MFT.SDLMediaModel,
        1: MFT.SDLNonMediaModel
    },

    /**
     * Handler for SoftButtons default action
     * 
     * @param {Object}
     */
    defaultActionSoftButton: function(element) {
        switch (element.groupName) {

            case "AlertPopUp": {
                MFT.AlertPopUp.deactivate(true);
                break;
            }
            case "ScrollableMessage": {
                MFT.ScrollableMessage.deactivate();
                break;
            }
            case "TurnByTurnView": {
                MFT.TurnByTurnView.deactivate();
                break;
            }
            case "info_nonMedia": {
                MFT.SDLController.getApplicationModel(element.appId).clearAppOverLay();
                break;
            }
            case "sdl_view_container": {
                MFT.SDLController.getApplicationModel(element.appId).clearAppOverLay();
                break;
            }

        }
    },

    /**
     * Handler for SoftButtons stealFocus action
     * 
     * @param {Object}
     */
    stealFocusSoftButton: function(element) {
        switch (element.groupName) {

            case "AlertPopUp": {
                MFT.AlertPopUp.deactivate();
                MFT.SDLController.getApplicationModel(element.appId).turnOnSDL();
                break;
            }
            case "ScrollableMessage": {
                MFT.ScrollableMessage.deactivate();
                MFT.SDLController.getApplicationModel(element.appId).turnOnSDL();
                break;
            }

        }
    },

    /**
     * Handler for SoftButtons keepContext action
     * 
     * @param {Object}
     */
    keepContextSoftButton: function(element) {
        switch (element.groupName) {

            case "AlertPopUp": {
                MFT.AlertPopUp.set('timer', 0);
                break;
            }
            case "ScrollableMessage": {
                MFT.ScrollableMessage.set('timer', 0);
                break;
            }

        }
    },

    /**
     * Method to close AlertMeneuverPopUp view
     */
    closeAlertMeneuverPopUp: function() {
        MFT.AlertManeuverPopUp.set('activate', false);
    },

    /**
     * Method to open Turn List view from TBT
     * 
     * @param {Number} appId AppId of activated sdl application
     */
    tbtTurnList: function(appId) {
        MFT.TBTTurnList.activate(appId);
    },

    /**
     * Method to sent notification with selected state of TBT Client State
     * 
     * @param {String}
     */
    tbtClientStateSeleced: function(state) {
        FFW.UI.onTBTClientState(state, MFT.SDLAppController.model.appId);
    },

    /**
     * Method to sent notification ABORTED for PerformInteractionChoise
     */
    interactionChoiseCloseResponse: function(result, performInteractionRequestId) {
        FFW.UI.interactionResponse(result, performInteractionRequestId);
    },

    /**
     * Method to sent notification for Alert
     * 
     * @param {String} result
     * @param {Number} alertRequestId
     */
    alertResponse: function(result, alertRequestId) {
        FFW.UI.sendUIResult(result, alertRequestId, 'UI.Alert');
    },

    /**
     * Method to sent notification for Scrollable Message
     * 
     * @param {String} result
     * @param {Number} messageRequestId
     */
    scrollableMessageResponse: function(result, messageRequestId) {
        FFW.UI.sendUIResult(result, messageRequestId, 'UI.ScrollableMessage');
    },

    /**
     * Method to sent notification for Slider
     * 
     * @param {String} result
     * @param {Number} sliderRequestId
     */
    sliderResponse: function(result, sliderRequestId) {
        FFW.UI.sendUIResult(result, sliderRequestId, 'UI.Slider');
    },

    /**
     * Method to call performAudioPassThruResponse with Result code parameters
     * 
     * @param {Object} element Button object
     */
    callPerformAudioPassThruPopUpResponse: function(element) {
        this.performAudioPassThruResponse(element.responseResult);
    },

    /**
     * Method close PerformAudioPassThruPopUp and call response from UIRPC back
     * to SDLCore
     * 
     * @param {String} result Result code
     */
    performAudioPassThruResponse: function(result) {
        MFT.SDLModel.set('AudioPassThruState', false);
        FFW.UI.sendUIResult(result, FFW.UI.performAudioPassThruRequestId, "UI.PerformAudioPassThru");
    },

    /**
     * Method to set language for UI component with parameters sent from
     * SDLCore to UIRPC
     */
    onLanguageChangeUI: function() {
        FFW.UI.OnLanguageChange(MFT.SDLModel.hmiUILanguage);
    }.observes('MFT.SDLModel.hmiUILanguage'),

    /**
     * Method to set language for TTS and VR components with parameters sent
     * from SDLCore to UIRPC
     */
    onLanguageChangeTTSVR: function() {
        FFW.TTS.OnLanguageChange(MFT.SDLModel.hmiTTSVRLanguage);
        FFW.VR.OnLanguageChange(MFT.SDLModel.hmiTTSVRLanguage);
    }.observes('MFT.SDLModel.hmiTTSVRLanguage'),

    /**
     * Register application
     * 
     * @param {Object} params
     * @param {Number} applicationType
     */
    registerApplication: function(params, applicationType) {
        if (MFT.SDLModel.registeredApps[params.appId]) {
            return;
        }

        MFT.SDLModel.registeredApps[params.appId] = this.applicationModels[applicationType].create( {
            appId: params.appId,
            appName: params.appName,
            deviceName: params.deviceName
        });

        MFT.SDLModel.get('applicationsList').pushObject(params.appId);

        MFT.VRPopUp.AddActivateApp(params.appId, params.appName);
    },

    /**
     * SDL Driver Distraction ON/OFF switcher
     * 
     * @param {Boolean}
     */
    selectDriverDistraction: function(checked) {
        if (checked) {
            FFW.UI.onDriverDistraction("DD_ON");
            this.set('driverDistractionState', true);
        } else {
            FFW.UI.onDriverDistraction("DD_OFF");
            this.set('driverDistractionState', false);
        }
    },

    /**
     * SDL Send Data ON/OFF extended param switcher
     * 
     * @param {Boolean}
     */
    selectSendData: function(checked) {
        MFT.SDLModel.set('sendDataExtend', checked);
    },

    /**
     * SDL Protocol Version 2 ON/OFF switcher
     * 
     * @param {Boolean}
     */
    selectProtocolVersion: function(checked) {
        if (checked) {
            FFW.BasicCommunication.OnVersionChanged(2);
        } else {
            FFW.BasicCommunication.OnVersionChanged(1);
        }
    },

    /**
     * Get application model
     * 
     * @param {Number}
     */
    getApplicationModel: function(applicationId) {
        return MFT.SDLModel.registeredApps[applicationId];
    },

    /**
     * Function returns ChangeDeviceView back to previous state
     */
    turnChangeDeviceViewBack: function() {
        MFT.States.goToState('info.apps');
    },

    /**
     * Enter screen vith list of devices application model
     */
    onGetDeviceList: function() {
        MFT.States.goToState('info.devicelist');
        FFW.BasicCommunication.getDeviceList();
    },

    /**
     * Send notification if device was choosed
     * 
     * @param element: MFT.Button
     */
    onDeviceChoosed: function(element) {
        FFW.UI.OnDeviceChosen(element.deviceName);
        this.turnChangeDeviceViewBack();
    },

    /**
     * Method creates list of Application ID's Then call HMI method for display
     * a list of Applications
     * 
     * @param {Object}
     */
    onGetAppList: function(appList) {
        MFT.SDLModel.onGetAppList(appList);
    },

    /**
     * Method call's request to get list of applications
     */
    findNewApps: function() {
        FFW.BasicCommunication.getAppList();
    },

    /**
     * Method activates selected registered application
     * 
     * @param {Object}
     */
    onActivateSDLApp: function(element) {
        this.getApplicationModel(element.appId).turnOnSDL();
    },

    /**
     * Method sent custom softButtons pressed and event status to RPC
     * 
     * @param {Object}
     */
    onSoftButtonActionUpCustom: function(element) {
        if (element.time > 0) {
            FFW.Buttons.buttonEventCustom("CUSTOM_BUTTON", "BUTTONUP", element.softButtonID);
        } else {
            FFW.Buttons.buttonEventCustom("CUSTOM_BUTTON", "BUTTONUP", element.softButtonID);
            FFW.Buttons.buttonPressedCustom("CUSTOM_BUTTON", "SHORT", element.softButtonID);
        }
        clearTimeout(element.timer);
        element.time = 0;
    },

    /**
     * Method sent custom softButtons pressed and event status to RPC
     * 
     * @param {Object}
     */
    onSoftButtonActionDownCustom: function(element) {
        FFW.Buttons.buttonEventCustom("CUSTOM_BUTTON", "BUTTONDOWN", element.softButtonID);
        element.time = 0;
        element.timer = setTimeout(function() {
            FFW.Buttons.buttonPressedCustom("CUSTOM_BUTTON", "LONG", element.softButtonID);
            element.time++;
        }, 2000);
    },

    /**
     * Method sent softButtons pressed and event status to RPC
     * 
     * @param {String}
     * @param {Object}
     */
    onSoftButtonActionUp: function(name, element) {
        if (element.time > 0) {
            FFW.Buttons.buttonEvent(name, "BUTTONUP");
        } else {
            FFW.Buttons.buttonEvent(name, "BUTTONUP");
            FFW.Buttons.buttonPressed(name, "SHORT");
        }
        clearTimeout(element.timer);
        element.time = 0;
    },

    /**
     * Method sent softButtons Ok pressed and event status to RPC
     * 
     * @param {String}
     */
    onSoftButtonOkActionDown: function(name) {
        FFW.Buttons.buttonEvent(name, "BUTTONDOWN");
    },

    /**
     * Method sent softButton OK pressed and event status to RPC
     * 
     * @param {String}
     */
    onSoftButtonOkActionUp: function(name) {
        FFW.Buttons.buttonEvent(name, "BUTTONUP");
        FFW.Buttons.buttonPressed(name, "SHORT");
        MFT.SDLAppController.model.set('isPlaying', !MFT.SDLAppController.model.isPlaying);
    },

    /**
     * Method sent softButtons pressed and event status to RPC
     * 
     * @param {String}
     * @param {Object}
     */
    onSoftButtonActionDown: function(name, element) {
        FFW.Buttons.buttonEvent(name, "BUTTONDOWN");
        element.time = 0;
        element.timer = setTimeout(function() {
            FFW.Buttons.buttonPressed(name, "LONG");
            element.time++;
        }, 2000);
    },

    /**
     * Send system context
     */
    onSystemContextChange: function() {
        FFW.UI.OnSystemContext(this.get('sysContext'));
    }
});
