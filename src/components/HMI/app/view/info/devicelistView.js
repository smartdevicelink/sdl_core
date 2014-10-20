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
 * @name SDL.DeviceListView
 * @desc Media App Options module visual representation
 * @category View
 * @filesource app/view/media/devicelistView.js
 * @version 1.0
 */

SDL.DeviceListView = Em.ContainerView.create( {

    classNames: [
        'info_apps_deviceList_view'
    ],

    classNameBindings: [
        'SDL.States.info.devicelist.active:active_state:inactive_state'
    ],

    /**
     * View Id
     */
    elementId: 'info_apps_deviceList_view',

    /**
     * View Components
     */
    childViews: [
        'backButton',
        'listOfDevices',
        'deviceListLabel',
        'progress'
    ],

    /**
     * Animation of search devices progress
     */
    progress: Em.View.extend( {
        classNames: [
            'progress'
        ],
        classNameBindings: ['SDL.SDLModel.deviceSearchProgress:progress']
    }),


    /**
     * Button to return to previous view
     */
    backButton: SDL.Button.extend( {
        classNames: [
            'backButton', 'button'
        ],
        action: 'turnChangeDeviceViewBack',
        target: 'SDL.SDLController',
        icon: 'images/media/ico_back.png'
    }),

    /**
     * Label in title
     */
    deviceListLabel: SDL.Label.extend( {

        elementId: 'deviceListLabel',

        classNames: 'deviceListLabel',

        content: 'Change Devices'
    }),

    /**
     * Function calls when notification from RPC comes and creates buttons to
     * choose devices
     */
    ShowDeviceList: function(params) {

        this.clearDeviceList();

        var i, len = params.deviceList.length;
        for (i = 0; i < len; i++) {
            this.get('listOfDevices.list.childViews').pushObject(SDL.Button
                .create( {
                    deviceName: params.deviceList[i].name,
                    icon: params.deviceList[i].icon,
                    text: params.deviceList[i].name,
                    classNames: 'ffw-button notpressed list-item',
                    templateName: params.deviceList[i].icon ? 'rightIcon'
                        : 'text',
                    action: 'onDeviceChoosed',
                    target: 'SDL.SDLController',
                    onDown: false,
                    id: params.deviceList[i].id
                }));
        }
    },

    /**
     * Function calls each time when user enters Change Device menu and clear
     * all old data about devices
     */
    clearDeviceList: function() {

        this.get('listOfDevices.list').removeAllChildren();
        this.listOfDevices.rerender();
    },

    /**
     * List for option on DeviceListView screen
     */
    listOfDevices: SDL.List.extend( {

        elementId: 'info_apps_deviceList_list',

        itemsOnPage: 5,

        /** Items array */
        items: []
    })
});
