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
 * @name SDL.DeviceConfigView
 * @desc Info Apps visual representation
 * @category View
 * @filesource app/view/settings/policies/deviceConfigView.js
 * @version 1.0
 */

SDL.AppPermissionsView = Em.ContainerView.create( {

    elementId: 'policies_settings_appPermissions',

    classNames: 'in_settings_separate_view',

    classNameBindings: [
        'SDL.States.settings.policies.appPermissions.active:active_state:inactive_state'
    ],

    childViews: [
        'backButton',
        'appList',
        'label'
    ],

    currentAppId: null,

    /**
     * Label in title
     */
    label: SDL.Label.extend( {

        classNames: 'label',

        content: 'Choose devices to be allowed for SDL functionality:'
    }),

    backButton: SDL.Button.extend( {
        classNames:
            [
                'backButton'
            ],
        action: function(element){

            SDL.SettingsController.onState(element);

            var permissions = [];

            for (var i = 0; i < SDL.AppPermissionsView.appList.list._childViews.length; i++) {
                permissions.push({
                    "name": SDL.AppPermissionsView.appList.list._childViews[i].name,
                    "id": SDL.AppPermissionsView.appList.list._childViews[i].id,
                    "allowed": SDL.AppPermissionsView.appList.list._childViews[i].allowed
                });
            }

            FFW.BasicCommunication.OnAppPermissionConsent(permissions, "GUI", SDL.AppPermissionsView.currentAppId);

            SDL.AppPermissionsView.currentAppId = null;
        },
        goToState: 'policies',
        icon: 'images/media/ico_back.png',
        onDown: false
    } ),

    /**
     * Function to add application to application list
     */
    update: function(message, appID) {

        SDL.AppPermissionsView.currentAppId = appID;

        this.appList.items = [];

        for (var i = 0; i < message.length; i++) {

            var text = " - Undefined";

            if (message[i].allowed === true) {
                text = " - Allowed";
            } else if (message[i].allowed === false) {
                text = " - Not allowed";
            }

            this.appList.items.push({
                type: SDL.Button,
                params: {
                    action: 'changeAppPermission',
                    target: 'SDL.SettingsController',
                    text: message[i].name + text,
                    name: message[i].name,
                    allowed: message[i].allowed,
                    id: message[i].id,
                    appID: appID
                }
            });
        }

        this.appList.list.refresh();

    },

    appList: SDL.List.extend( {

        elementId: 'polocies_app_permissions_list',

        itemsOnPage: 5,

        /** Items */
        items: new Array()
    })
});