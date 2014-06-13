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
 * @name SDL.SettingsController
 * @desc Info Controller logic
 * @category Controller
 * @filesource app/controller/SettingsController.js
 * @version 1.0
 */

SDL.SettingsController = Em.Object.create( {

    activeState: 'settings.policies',

    hiddenLeftMenu: false,

    /**
     * File name for SDL.OnSystemRequest
     * Came in SDL.PolicyUpdate request
     */
    policyUpdateFile: null,

    /**
     * Data of current requested devices which access will be allowed or disallowed.
     */
    currentDeviceAllowance: null,

    onState: function(event) {

        SDL.States.goToStates('settings.' + event.goToState);
    },

    onChildState: function(event) {

        SDL.States.goToStates(SDL.States.currentState.get('path') + '.'
            + event.goToState);
    },

    getStatusUpdate: function(){
        FFW.BasicCommunication.GetStatusUpdate();
    },

    allDeviceAccess: function(){

        var allowedValue, allowedText;

        if (SDL.DeviceConfigView.globalConfigurationValue) {
            SDL.DeviceConfigView.set('globalConfigurationValue', false);
            allowedValue = false;
            allowedText = " - Not allowed";
        } else {
            SDL.DeviceConfigView.set('globalConfigurationValue', true);
            allowedValue = true;
            allowedText = " - Allowed";
        }

        var dev = SDL.SDLModel.connectedDevices;
        for (var key in dev) {
            if (dev.hasOwnProperty(key)) {
                dev[key].allowed = allowedValue;
            }
        }

        SDL.DeviceConfigView.showDeviceList();

        FFW.BasicCommunication.OnAllowSDLFunctionality(allowedValue, "GUI");
    },

    changeDeviceAccess: function(event) {

        var dev = SDL.SDLModel.connectedDevices;
        for (var key in dev) {

            if (dev.hasOwnProperty(key)) {

                if (dev[key].id == event.id) {

                    if (dev[key].allowed) {

                        dev[key].allowed = false;
                        event.set('text', event.name + " - Not allowed");
                    } else {

                        dev[key].allowed = true;
                        event.set('text', event.name + " - Allowed");
                    }

                    var device = {
                        "name": dev[key].name,
                        "id": dev[key].id
                    };

                    SDL.DeviceConfigView.set('globalConfigurationValue', null);

                    FFW.BasicCommunication.OnAllowSDLFunctionality(dev[key].allowed, "GUI", device);
                    break;
                }
            }
        }
    },

    changeAppPermission: function(event) {

        var allowance = SDL.SDLController.getApplicationModel(event.appID).allowedFunctions;

        for (var i = 0; i < allowance.length; ++i) {

            if (allowance[i].name == event.name) {

                if (allowance[i].allowed) {

                    allowance[i].allowed = false;
                    event.set('text', event.name + " - Not allowed");
                } else {

                    allowance[i].allowed = true;
                    event.set('text', event.name + " - Allowed");
                }
                break;
            }
        }
    },

    /**
     * Method to update array with app permissions which came from SDL
     *
     * @param {Object} params
     *
     */
    onAppPermissionChanged: function(params) {

        if (params.isAppPermissionsRevoked) {
            FFW.BasicCommunication.GetUserFriendlyMessage(function(message){SDL.PopUp.popupActivate(message)});

            //deleted array
            SDL.SDLModel.setAppPermissions(params.appRevokedPermissions);
        }

        if (params.isAppRevoked) {

            //to do use GetUserFriendlyMessage
            SDL.PopUp.popupActivate("Current version of app is no longer supported!");

            SDL.SDLModel.onAppUnregistered({
                "appID": params.appID
            });
        }

        if (params.isPermissionsConsentNeeded) {

            FFW.BasicCommunication.GetListOfPermissions(params.appID);

            //returns allowedFunctions
            FFW.BasicCommunication.GetUserFriendlyMessage(function(allowedFunctions, appID){FFW.BasicCommunication.OnAppPermissionConsent(allowedFunctions, "GUI", appID)}, params.appID);
        }

        if (params.appUnauthorized) {

            //to do use GetUserFriendlyMessage
            SDL.PopUp.popupActivate("Something went wrong!");

            SDL.SDLModel.onAppUnregistered({
                "appID": params.appID
            });
        }
    },

    /**
     * Method to send request to update array with app permissions
     *
     * @param {Object} element
     *
     */
    GetListOfPermissions: function(element) {
        FFW.BasicCommunication.GetListOfPermissions(element.appID);
    },

    /**
     * Method to update array with app permissions which came from SDL
     *
     * @param {Object} message
     *
     */
    GetListOfPermissionsResponse: function(message) {

        if (message.id in SDL.SDLModel.getListOfPermissionsPull) {

            var appID = SDL.SDLModel.getListOfPermissionsPull[message.id],
                messageCodes = [];

            SDL.SDLController.getApplicationModel(appID).allowedFunctions = message.result.allowedFunctions;

            for (var i = 0; i < message.result.allowedFunctions.length; i++) {
                messageCodes.push(message.result.allowedFunctions[i].name);
            }

            FFW.BasicCommunication.GetUserFriendlyMessage(SDL.SettingsController.permissionsFriendlyMessageUpdate, appID, messageCodes);

            SDL.SettingsController.userFriendlyMessagePopUp();

            delete SDL.SDLModel.getListOfPermissionsPull[message.id];
        }
    },

    /**
     * Method to update array with app permissions with UserFriendlyMessage from SDL
     *
     * @param {Object} message
     *
     */
    permissionsFriendlyMessageUpdate: function(message, appID) {
            var len = SDL.SDLController.getApplicationModel(appID).allowedFunctions.length;

            for (var i = 0; i < len; i++) {

                if (message && message.label) {
                    SDL.SDLController.getApplicationModel(appID).allowedFunctions[i].text = message.label;
                }
            }

            SDL.AppPermissionsView.update(appID);

            SDL.States.goToStates('settings.policies.appPermissions');
    },

    updateSDL: function() {
        FFW.BasicCommunication.UpdateSDL();
    },

    getURLS: function() {
        FFW.BasicCommunication.GetURLS();
    },

    AllowSDLFunctionality: function(device) {
        this.currentDeviceAllowance = device;
        SDL.SDLModel.connectedDevices[device.id].sdlFunctionality.popUpId = SDL.PopUp.popupActivate("Would you like to allow SDL functionality for device '" + device.name + "'?", SDL.SettingsController.OnAllowSDLFunctionality);
    },

    onSDLConsentNeededHandler: function(params) {
        this.currentDeviceAllowance = params.device;

        FFW.BasicCommunication.GetUserFriendlyMessage(function(message){SDL.PopUp.popupActivate(message, this.OnAllowSDLFunctionality)});
    },

    userFriendlyMessagePopUp: function(appId, messageCode) {

        FFW.BasicCommunication.GetUserFriendlyMessage(function(message){SDL.PopUp.popupActivate(message)}, appId, messageCode);
    },

    OnAllowSDLFunctionality: function(result) {

        var dev = SDL.SDLModel.connectedDevices;
        for (var key in dev) {
            if (dev.hasOwnProperty(key)) {
                if (dev[key].id == SDL.SettingsController.currentDeviceAllowance.id) {
                    dev[key].allowed = result;
                }
            }
        }

        SDL.DeviceConfigView.set('globalConfigurationValue', null);

        SDL.DeviceConfigView.showDeviceList();

        FFW.BasicCommunication.OnAllowSDLFunctionality(result, "GUI", SDL.SettingsController.currentDeviceAllowance);

        SDL.SDLModel.connectedDevices[SDL.SettingsController.currentDeviceAllowance.id].sdlFunctionality.allowed = result;

        SDL.SDLModel.connectedDevices[SDL.SettingsController.currentDeviceAllowance.id].sdlFunctionality.popUpId = null;

        SDL.SettingsController.currentDeviceAllowance = null;
    },

    /**
     * Method responsible for PolicyUpdate retry sequence
     * abort parameter if set to true means that retry sequence if finished
     *
     * @param {Boolean} abort
     */
    policyUpdateRetry: function(abort) {

        clearTimeout(SDL.SDLModel.policyUpdateRetry.timer);
        SDL.SDLModel.policyUpdateRetry.timer = null;

        if (!abort || (SDL.SDLModel.policyUpdateRetry.try < SDL.SDLModel.policyUpdateRetry.retry.length)) {

            SDL.SDLModel.policyUpdateRetry.oldTimer = SDL.SDLModel.policyUpdateRetry.oldTimer +
                SDL.SDLModel.policyUpdateRetry.timeout * 1000 +
                SDL.SDLModel.policyUpdateRetry.retry[SDL.SDLModel.policyUpdateRetry.try] * 1000;

            SDL.SDLModel.policyUpdateRetry.timer = setTimeout(
                function(){
                    if (SDL.SDLModel.policyURLs.length > SDL.SDLModel.policyUpdateRetry.try) {
                        FFW.BasicCommunication.OnSystemRequest(
                            "PROPRIETARY",
                            SDL.SDLModel.policyURLs[SDL.SDLModel.policyUpdateRetry.try].policyAppId,
                            SDL.SettingsController.policyUpdateFile,
                            SDL.SDLModel.policyURLs[SDL.SDLModel.policyUpdateRetry.try].url
                        );
                    } else {
                        FFW.BasicCommunication.OnSystemRequest("PROPRIETARY");
                    }
                    SDL.SettingsController.policyUpdateRetry();
                }, SDL.SDLModel.policyUpdateRetry.oldTimer
            );

            SDL.SDLModel.policyUpdateRetry.try++;
        } else {
            SDL.SDLModel.policyUpdateRetry = {
                timeout: null,
                retry: [],
                try: null,
                timer: null,
                oldTimer: 0
            };
        }
    }
});
