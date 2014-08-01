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
 * @name SDL.PoliciesSettingsView
 * @desc Info Apps visual representation
 * @category View
 * @filesource app/view/settings/AppsView.js
 * @version 1.0
 */

SDL.PoliciesView = Em.ContainerView.create( {

    elementId: 'policies_settings',

    classNames: 'in_settings_view',

    classNameBindings: [
        'SDL.States.settings.policies.active:active_state:inactive_state'
    ],

    childViews: [
        'SettingsList'
    ],

    SettingsList: SDL.List.extend( {

        elementId: 'policies_settings_list',

        classNames: 'policies_settings_list',

        itemsOnPage: 5,

        /** Items */
        items: [
            {
                type: SDL.Button,
                params: {
                    goToState: 'policies.deviceConfig',
                    text: 'Allow SDL Functionality',
                    action: 'onState',
                    target: 'SDL.SettingsController',
                    templateName: 'arrow',
                    onDown: false
                }
            },
            {
                type: SDL.Button,
                params: {
                    text: 'Update SDL',
                    action: 'updateSDL',
                    target: 'SDL.SettingsController',
                    onDown: false
                }
            },
            {
                type: SDL.Button,
                params: {
                    text: 'Policy table update status',
                    action: 'getStatusUpdate',
                    target: 'SDL.SettingsController',
                    onDown: false
                }
            },
            {
                type: SDL.Button,
                params: {
                    text: 'Send request GetURLS',
                    action: 'getURLS',
                    target: 'SDL.SettingsController',
                    onDown: false
                }
            },
            {
                type: SDL.Button,
                params: {
                    text: 'Statistics Info settings',
                    action: 'onState',
                    target: 'SDL.SettingsController',
                    templateName: 'arrow',
                    goToState: 'policies.statisticsInfo',
                    onDown: false
                }
            },
            {
                type: SDL.Button,
                params: {
                    text: 'App permissions',
                    action: 'onState',
                    target: 'SDL.SettingsController',
                    templateName: 'arrow',
                    goToState: 'policies.appPermissionsList',
                    onDown: false
                }
            },
            {
                type: SDL.Button,
                params: {
                    text: 'Get list of permissions',
                    click: function () {
                        FFW.BasicCommunication.GetListOfPermissions();
                    },
                    onDown: false
                }
            },
            {
                type: SDL.Button,
                params: {
                    text: 'Device state change',
                    action: 'onState',
                    target: 'SDL.SettingsController',
                    templateName: 'arrow',
                    goToState: 'policies.deviceStateChange',
                    onDown: false
                }
            },
            {
                type: SDL.Button,
                params: {
                    text: 'System Error',
                    action: 'onState',
                    target: 'SDL.SettingsController',
                    templateName: 'arrow',
                    goToState: 'policies.systemError',
                    onDown: false
                }
            }
        ]
    })
});