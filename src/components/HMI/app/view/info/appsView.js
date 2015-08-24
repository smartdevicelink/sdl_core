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
 * @name SDL.InfoAppsview
 * @desc Info Apps visual representation
 * @category View
 * @filesource app/view/info/AppsView.js
 * @version 1.0
 */

SDL.InfoAppsView = Em.ContainerView
    .create( {

        elementId: 'info_apps',

        classNameBindings: [
            'SDL.States.info.apps.active:active_state:inactive_state'
        ],

        childViews: [
            'vehicleHealthReport',
            'Asist911',
            'installButton',
            'findNewApps',
            'getDeviceList',
            'listOfApplications'
        ],

        /**
         * Function to add application to application list
         */
        showAppList: function() {

            this.get('listOfApplications.list').removeAllChildren();

            this.listOfApplications.list.refresh();

            var i, apps = SDL.SDLModel.updatedAppsList, btn, appIndex;

            for (i = 0; i < apps.length; i++) {

                btn = {
                    action: 'onActivateSDLApp',
                    target: 'SDL.SDLController',
                    text: apps[i].appName + " - " + apps[i].deviceName,
                    appName: apps[i].appName,
                    appID: apps[i].appID,
                    classNames: 'list-item button',
                    disabled: apps[i].greyOut
                };

                if (SDL.SDLController.getApplicationModel(apps[i].appID)) {
                    var index = SDL.SDLModel.registeredApps.indexOf(
                        SDL.SDLController.getApplicationModel(apps[i].appID)
                    );
                    btn.iconBinding = 'SDL.SDLModel.registeredApps.' + index + '.appIcon';
                } else {
                    btn.icon = apps[i].icon ?  apps[i].icon : SDL.SDLModel.defaultListOfIcons.app;
                }

                this.get('listOfApplications.list.childViews').pushObject(SDL.Button.create(btn));
            }

        },

        vehicleHealthReport: SDL.Button
            .extend( {
                goToState: 'vehicle.healthReport',
                classNames: 'button vehicleHealthReport leftButtons',
                icon: 'images/info/ico_vehicle.png',
                textBinding: 'SDL.locale.label.view_info_apps_vehicle_VehicleHealthReport',
                elementId: 'infoAppsVehicleHealthReport',
                arrow: true,
                onDown: false
            }),

        Asist911: SDL.Button.extend( {
            goToState: 'help.helpAssist',
            classNames: 'button Asist911 leftButtons',
            icon: 'images/info/ico_assist.png',
            textBinding: 'SDL.locale.label.view_info_apps_911Assist',
            elementId: 'infoAppsAsist911',
            arrow: true,
            onDown: false
        }),

        installButton: SDL.Button
            .extend( {
                goToState: 'settings.system.installApplications',
                icon: 'images/info/ico_info_install.png',
                textBinding: 'SDL.locale.label.view_info_apps_vehicle_InstallApplicationsUp',
                elementId: 'infoAppsInstallButton',
                classNames: 'button installButton leftButtons',
                arrow: true,
                onDown: false
            }),

        findNewApps: SDL.Button
            .extend( {
                goToState: 'settings.system.installApplications',
                icon: 'images/sdl/new_apps.png',
                textBinding: 'SDL.locale.label.view_info_apps_vehicle_FindNewApplications',
                elementId: 'infoAppsFindNewApps',
                classNames: 'button findNewApps leftButtons',
                arrow: true,
                action: 'findNewApps',
                target: 'SDL.SDLController',
                onDown: false
            }),

        getDeviceList: SDL.Button
            .extend( {
                icon: 'images/sdl/devices.png',
                textBinding: 'SDL.locale.label.view_info_apps_vehicle_GetDeviceList',
                elementId: 'infoAppsGetDeviceList',
                classNames: 'button getDeviceList leftButtons',
                arrow: true,
                action: 'onGetDeviceList',
                target: 'SDL.SDLController',
                onDown: false
            }),

        listOfApplications: SDL.List.extend( {

            elementId: 'info_apps_list',

            itemsOnPage: 5,

            /** Items */
            items: new Array()
        })
    });
