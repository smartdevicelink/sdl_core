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

        for (var i = 0; i < SDL.SDLModel.conectedDevices.length; ++i) {
            SDL.SDLModel.conectedDevices[i].allowed = allowedValue;
        }

        SDL.DeviceConfigView.showDeviceList();

        FFW.BasicCommunication.OnAllowSDLFunctionality(allowedValue, "GUI");
    },

    changeDeviceAccess: function(event) {
        for (var i = 0; i < SDL.SDLModel.conectedDevices.length; ++i) {

            if (SDL.SDLModel.conectedDevices[i].id == event.id) {

                if (SDL.SDLModel.conectedDevices[i].allowed) {

                    SDL.SDLModel.conectedDevices[i].allowed = false;
                    event.set('text', event.name + " - Not allowed");
                } else {

                    SDL.SDLModel.conectedDevices[i].allowed = true;
                    event.set('text', event.name + " - Allowed");
                }

                var device = {
                    "name": SDL.SDLModel.conectedDevices[i].name,
                    "id": SDL.SDLModel.conectedDevices[i].id
                };

                SDL.DeviceConfigView.set('globalConfigurationValue', null);

                FFW.BasicCommunication.OnAllowSDLFunctionality(SDL.SDLModel.conectedDevices[i].allowed, "GUI", device);
                break;
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

            SDL.States.goToStates('settings.policies.appPermissionsList');
    },

    updateSDL: function() {
        FFW.BasicCommunication.UpdateSDL();
    },

    getURLS: function() {
        FFW.BasicCommunication.GetURLS();
    },

    AllowSDLFunctionality: function(device) {
        this.currentDeviceAllowance = device;
        SDL.PopUp.popupActivate("Would you like to allow SDL functionality for device '" + device.name + "'?", SDL.SettingsController.OnAllowSDLFunctionality);
    },

    onSDLConsentNeededHandler: function(params) {
        this.currentDeviceAllowance = params.device;

        FFW.BasicCommunication.GetUserFriendlyMessage(function(message){SDL.PopUp.popupActivate(message, this.OnAllowSDLFunctionality)});
    },

    userFriendlyMessagePopUp: function(appId, messageCode) {

        FFW.BasicCommunication.GetUserFriendlyMessage(function(message){SDL.PopUp.popupActivate(message)}, appId, messageCode);
    },

    OnAllowSDLFunctionality: function(result) {

        for (var i = 0; i < SDL.SDLModel.conectedDevices.length; ++i) {
            if (SDL.SDLModel.conectedDevices[i].id == SDL.SettingsController.currentDeviceAllowance.id) {
                SDL.SDLModel.conectedDevices[i].allowed = result;
            }
        }

        SDL.DeviceConfigView.set('globalConfigurationValue', null);

        SDL.DeviceConfigView.showDeviceList();

        FFW.BasicCommunication.OnAllowSDLFunctionality(result, "GUI", SDL.SettingsController.currentDeviceAllowance);

        SDL.SettingsController.currentDeviceAllowance = null;
    }
});
