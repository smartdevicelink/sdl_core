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

SDL.StatisticsInfoView = Em.ContainerView.create( {

    elementId: 'policies_settings_status_info',

    classNames: 'in_settings_separate_view',

    classNameBindings: [
        'SDL.States.settings.policies.statisticsInfo.active:active_state:inactive_state'
    ],

    childViews: [
        'backButton',
        'label',
        'listSelect'
    ],

    /**
     * HMI element Select with parameters of transmission state from VehicleInfo
     * Model
     */
    listSelect: Em.Select.extend( {

        elementId: 'listSelect',

        classNames: 'listSelect',

        contentBinding: 'SDL.SDLModel.settingsInfoList',

        valueBinding: 'SDL.SDLModel.settingsInfoListState'
    } ),

    /**
     * Label in title
     */
    label: SDL.Label.extend( {

        elementId: 'label',

        classNames: 'label',

        content: 'Choose devices to be allowed for SDL functionality:'
    }),

    backButton: SDL.Button.extend( {
        classNames:
            [
                'backButton'
            ],
        action: 'onState',
        target: 'SDL.SettingsController',
        goToState: 'policies',
        icon: 'images/media/ico_back.png',
        onDown: false
    } )
});