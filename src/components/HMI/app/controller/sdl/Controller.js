/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  · Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *  · Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *  · Neither the name of the Ford Motor Company nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
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
 * @name SDL.SDLController
 * @desc Main SDL Controller
 * @category Controller
 * @filesource app/controller/sdl/SDLController.js
 * @version 1.0
 */

SDL.SDLController = Em.Object.create({

    /**
     * Current system context
     * 
     * @type {String}
     */
    sysContext: function() {

        if(SDL.VRPopUp.VRActive){
            return 'VRSESSION';
        }

        if(SDL.AlertPopUp.active){
            return 'ALERT';
        }

        if(SDL.TBTClientStateView.active || SDL.VehicleInfo.active || SDL.DriverDistraction.active){
            return 'HMI_OBSCURED';
        }

        if(SDL.OptionsView.active){
            return 'MENU';
        }

        if(SDL.States.info.nonMedia.active || SDL.States.media.sdlmedia.active){
            return 'MAIN';
        }else{
            return 'MENU';
        }

    }.property('SDL.DriverDistraction.active', 'SDL.OptionsView.active', 'SDL.VRPopUp.VRActive', 'SDL.AlertPopUp.active', 'SDL.TBTClientStateView.active',
                    'SDL.VehicleInfo.active', 'SDL.States.info.nonMedia.active', 'SDL.States.media.sdlmedia.active'),

    /**
     * List of SDL application models
     * 
     * @type object
     */
    applicationModels: {
        0: SDL.SDLMediaModel,
        1: SDL.SDLNonMediaModel
    },

    /**
     * Default action for SoftButtons: closes window, popUp or clears
     * applications screen
     * 
     * @param {Object}
     */
    defaultActionSoftButton: function(element) {
        switch(element.groupName){

            case "AlertPopUp": {
                SDL.AlertPopUp.deactivate(true);
                break;
            }
            case "ScrollableMessage": {
                SDL.ScrollableMessage.deactivate();
                break;
            }
            case "TurnByTurnView": {
                SDL.TurnByTurnView.deactivate();
                break;
            }
            case "NonMediaView": {
                SDL.SDLController.getApplicationModel(element.appID).clearAppOverLay();
                break;
            }
            case "MediaView": {
                SDL.SDLController.getApplicationModel(element.appID).clearAppOverLay();
                break;
            }

        }
    },

    /**
     * Action to show Voice Recognition PopUp
     */
    activateVRPopUp: function() {
        SDL.VRPopUp.activateVRPopUp();
    },

    /**
     * Action for SoftButtons that closes popUp or window and opens applications
     * screen
     * 
     * @param {Object}
     */
    stealFocusSoftButton: function(element) {
        switch(element.groupName){

            case "AlertPopUp": {
                SDL.AlertPopUp.deactivate();
                SDL.SDLController.getApplicationModel(element.appID).turnOnSDL();
                break;
            }
            case "ScrollableMessage": {
                SDL.ScrollableMessage.deactivate();
                SDL.SDLController.getApplicationModel(element.appID).turnOnSDL();
                break;
            }

        }
    },

    /**
     * Action for SoftButtons that clears popUps timer and it become visible all
     * the time until user user closes it
     * 
     * @param {Object}
     */
    keepContextSoftButton: function(element) {
        switch(element.groupName){

            case "AlertPopUp": {
                SDL.AlertPopUp.set('timer', 0);
                break;
            }
            case "ScrollableMessage": {
                SDL.ScrollableMessage.set('timer', 0);
                break;
            }

        }
    },

    /**
     * Method to close AlertMeneuverPopUp view
     */
    closeAlertMeneuverPopUp: function() {
        SDL.AlertManeuverPopUp.set('activate', false);
    },

    /**
     * Method to open Turn List view from TBT
     * 
     * @param {Number} appID AppID of activated sdl application
     */
    tbtTurnList: function(appID) {
        SDL.TBTTurnList.activate(appID);
    },

    /**
     * Method to sent notification with selected state of TBT Client State
     * 
     * @param {String}
     */
    tbtClientStateSeleced: function(state) {
        FFW.Navigation.onTBTClientState(state, SDL.SDLAppController.model.appID);
    },

    /**
     * Method to sent notification ABORTED for PerformInteractionChoise
     */
    interactionChoiseCloseResponse: function(result, performInteractionRequestID) {
        FFW.UI.interactionResponse(result, performInteractionRequestID);
    },

    /**
     * Method to sent notification for Alert
     * 
     * @param {String} result
     * @param {Number} alertRequestID
     */
    alertResponse: function(result, alertRequestID) {
        FFW.UI.alertResponse(result, alertRequestID);
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
        SDL.SDLModel.set('AudioPassThruState', false);
        FFW.UI.sendUIResult(result, FFW.UI.performAudioPassThruRequestId, "UI.PerformAudioPassThru");
    },

    /**
     * Method to set language for UI component with parameters sent from SDLCore
     * to UIRPC
     */
    onLanguageChangeUI: function() {
        FFW.UI.OnLanguageChange(SDL.SDLModel.hmiUILanguage);
    }.observes('SDL.SDLModel.hmiUILanguage'),

    /**
     * Method to set language for TTS and VR components with parameters sent
     * from SDLCore to UIRPC
     */
    onLanguageChangeTTSVR: function() {
        FFW.TTS.OnLanguageChange(SDL.SDLModel.hmiTTSVRLanguage);
        FFW.VR.OnLanguageChange(SDL.SDLModel.hmiTTSVRLanguage);
    }.observes('SDL.SDLModel.hmiTTSVRLanguage'),

    /**
     * Register application
     * 
     * @param {Object} params
     * @param {Number} applicationType
     */
    registerApplication: function(params, applicationType) {

        SDL.SDLModel.get('registeredApps').pushObject(SDL.SDLController.applicationModels[applicationType].create({
            appID: params.appID,
            appName: params.appName,
            deviceName: params.deviceName
        }));

    },

    /**
     * Unregister application
     * 
     * @param {Number} appID
     */
    unregisterApplication: function(appID) {

        this.getApplicationModel(appID).onDeleteApplication(appID);

        this.getApplicationModel(appID).set('active', false);

        var index = SDL.SDLModel.registeredApps.indexOf(SDL.SDLModel.registeredApps.filterProperty('appID', appID)[0]);

        SDL.SDLModel.registeredApps.replace(index, 1);

        this.set('model', null);

    },

    /**
     * SDL Driver Distraction ON/OFF switcher
     */
    selectDriverDistraction: function() {
        if(SDL.SDLModel.driverDistractionState){
            FFW.UI.onDriverDistraction("DD_ON");
        }else{
            FFW.UI.onDriverDistraction("DD_OFF");
        }
    }.observes('SDL.SDLModel.driverDistractionState'),

    /**
     * SDL Protocol Version 2 ON/OFF switcher
     */
    selectProtocolVersion: function() {
        if(SDL.SDLModel.protocolVersion2State){
            FFW.BasicCommunication.OnVersionChanged(2);
        }else{
            FFW.BasicCommunication.OnVersionChanged(1);
        }
    }.observes('SDL.SDLModel.protocolVersion2State'),

    /**
     * Get application model
     * 
     * @param {Number}
     */
    getApplicationModel: function(applicationId) {
        return SDL.SDLModel.registeredApps.filterProperty('appID', applicationId)[0];
    },

    /**
     * Function returns ChangeDeviceView back to previous state
     */
    turnChangeDeviceViewBack: function() {
        SDL.States.goToStates('info.apps');
    },

    /**
     * Enter screen vith list of devices application model
     */
    onGetDeviceList: function() {
        SDL.States.goToStates('info.devicelist');
        //FFW.BasicCommunication.getDeviceList();
        FFW.BasicCommunication.StartDeviceDiscovery();
    },

    /**
     * Send notification if device was choosed
     * 
     * @param element: SDL.Button
     */
    onDeviceChoosed: function(element) {
        FFW.UI.OnDeviceChosen(element.deviceName, element.id);
        this.turnChangeDeviceViewBack();
    },

    /**
     * Method creates list of Application ID's Then call HMI method for display
     * a list of Applications
     * 
     * @param {Object}
     */
    onGetAppList: function(appList) {
        SDL.SDLModel.onGetAppList(appList);
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
    	//FFW.BasicCommunication.ActivateApp(element.appID);
    	SDL.SDLController.getApplicationModel(element.appID).turnOnSDL();
    },

    /**
     * Method sent custom softButtons pressed and event status to RPC
     * 
     * @param {Object}
     */
    onSoftButtonActionUpCustom: function(element) {
        if(element.time > 0){
            FFW.Buttons.buttonEventCustom("CUSTOM_BUTTON", "BUTTONUP", element.softButtonID);
        }else{
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
    onSoftButtonActionUp: function(element) {
        if(element.time > 0){
            FFW.Buttons.buttonEvent(element.presetName, "BUTTONUP");
        }else{
            FFW.Buttons.buttonEvent(element.presetName, "BUTTONUP");
            FFW.Buttons.buttonPressed(element.presetName, "SHORT");
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
        SDL.SDLAppController.model.set('isPlaying', !SDL.SDLAppController.model.isPlaying);
    },

    /**
     * Method sent softButtons pressed and event status to RPC
     * 
     * @param {String}
     * @param {Object}
     */
    onSoftButtonActionDown: function(element) {
        FFW.Buttons.buttonEvent(element.presetName, "BUTTONDOWN");
        element.time = 0;
        element.timer = setTimeout(function() {
            FFW.Buttons.buttonPressed(element.presetName, "LONG");
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
