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
 * @name SDL.InfoView
 * @desc Info module visual representation
 * @category Info
 * @filesource app/view/InfoView.js
 * @version 1.0
 */

SDL.InfoView = Em.ContainerView.create( {

    elementId: 'info_view',

    /** Bind class for visual representation */
    classNameBindings:
        [
            'SDL.States.info.active:active_state:inactive_state'
        ],

    controllerBinding: Ember.Binding.oneWay( 'SDL.InfoController' ),

    /** Settings components */
    childViews:
        [
            'leftMenu',
            SDL.InfoServicesView,
            SDL.InfoTravelLinkView,
            SDL.InfoCalendarView,
            SDL.InfoAppsView,
            SDL.InfoNonMedia,
            SDL.DeviceListView
        ],

    /** Left menu */
    leftMenu: Em.ContainerView.extend( {
        elementId: 'info_leftMenu',

        classNameBindings:
            [
                'parentView.controller.hiddenLeftMenu:hidden'
            ],

        classNames: 'menu-items',

        childViews:
            [
                'border',
                'items'
            ],

        border: Em.View.extend( {
            classNames: 'ls_border'
        } ),

        items: Em.ContainerView.extend( {
            classNames: 'ls-items',

            childViews:
                [
                    'servicesButton',
                    'travelLinkButton',
                    'calendarButton',
                    'appsButton',
                    'sdlButton',
                    'goToCD'
                ],

            servicesButton: SDL.Button.extend( {
                elementId: 'info_leftMenu_services',
                goToState: 'services',
                classNames: 'menu-item lsp1_p',
                classNameBindings:
                    [
                        'SDL.States.info.services.active:info_active'
                    ],
                textBinding: 'SDL.locale.label.view_info_leftmenu_services',
                icon: 'images/info/info_leftMenu_services_ico.png',
                action: 'onState',
                target: 'SDL.InfoController'
            } ),

            travelLinkButton: SDL.Button.extend( {
                elementId: 'info_leftMenu_travelLink',
                goToState: 'travelLink',
                classNames: 'menu-item lsp1_p',
                classNameBindings:
                    [
                        'SDL.States.info.travelLink.active:info_active'
                    ],
                textBinding: 'SDL.locale.label.view_info_leftmenu_travelLink',
                icon: 'images/info/info_leftMenu_travelLink_ico.png',
                action: 'onState',
                target: 'SDL.InfoController'
            } ),

            alertsButton: SDL.Button.extend( {
                elementId: 'info_leftMenu_alerts',
                goToState: 'alerts',
                classNames: 'menu-item lsp1_p',
                classNameBindings:
                    [
                        'SDL.States.info.alerts.active:info_active'
                    ],
                textBinding: 'SDL.locale.label.view_info_leftmenu_alerts',
                icon: 'images/info/info_leftMenu_alerts_ico.png',
                action: 'onState',
                target: 'SDL.InfoController'
            } ),

            calendarButton: SDL.Button.extend( {
                elementId: 'info_leftMenu_calendar',
                goToState: 'calendar',
                classNames: 'menu-item lsp1_p',
                classNameBindings:
                    [
                        'SDL.States.info.calendar.active:info_active'
                    ],
                textBinding: 'SDL.locale.label.view_info_leftmenu_calendar',
                icon: 'images/info/info_leftMenu_calendar_ico.png',
                action: 'onState',
                target: 'SDL.InfoController'
            } ),

            appsButton: SDL.Button.extend( {
                elementId: 'info_leftMenu_apps',
                goToState: 'apps',
                classNames: 'menu-item lsp1_p',
                classNameBindings:
                    [
                        'SDL.States.info.apps.active:info_active'
                    ],
                textBinding: 'SDL.locale.label.view_info_leftmenu_apps',
                icon: 'images/info/info_leftMenu_apps_ico.png',
                action: 'onState',
                target: 'SDL.InfoController'
            } ),

            sdlButton: SDL.Button.extend( {
                elementId: 'info_leftMenu_sdl',
                goToState: 'nonMedia',
                classNames: 'menu-item lsp1_p',
                classNameBindings:
                    [
                        'SDL.States.info.nonMedia.active:info_active'
                    ],
                hidden: function() {
                    return !SDL.NonMediaController.currentAppId;
                }.property( 'SDL.NonMediaController.currentAppId' ),
                textBinding: 'SDL.NonMediaController.currentAppName',
                iconBinding: 'SDL.NonMediaController.currentAppIcon',
                action: 'activateCurrentApp',
                target: 'SDL.NonMediaController'
            } ),

            goToCD: SDL.Button.extend( {
                elementId: 'info_leftMenu_goToCD',
                classNames: 'menu-item lsp1_p',
                text: 'Go To CD',
                action: 'turnOnCD',
                icon: 'images/media/ico_cd.png',
                target: 'SDL.MediaController'
            } )
        } )
    } )
} );