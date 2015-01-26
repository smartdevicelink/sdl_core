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
 * @name SDL.StatusMediaView
 * @desc Component for Media Status bar
 * @category view
 * @filesource app/view/media/StatusMediaView.js
 * @version 1.0
 */
SDL.StatusMediaView = Em.ContainerView
    .extend( {

        elementId: 'media_status',

        classNameBindings: [
            'SDL.States.media.active:selected'
        ],

        /** Climate components */
        childViews: [
            'statusHome', 'statusInfo'
        ],

        /** Label for Home screen */
        statusHome: SDL.Label.extend( {
            elementId: 'media_status_homestatus',
            classNameBindings: [
                'SDL.States.home.active:visible'
            ],
            contentBinding: Ember.Binding
                .oneWay('SDL.locale.label.view_mediaStatus_entertainment')
        }),

        statusInfo: Em.ContainerView
            .extend( {
                elementId: 'media_status_radio_container',

                childViews: [
                    'info'
                ],

                info: Em.View
                    .extend( {
                        elementId: 'media_status_radio_info',
                        controlerBinding: 'SDL.MediaController',
                        /** Bind class for visual representation */
                        classNameBindings: [
                            'SDL.States.home.active:hidden_display:visible_display'
                        ],
                        /** Define module layout */
                        template: Em.Handlebars
                            .compile('<div class="statusInfo">'
                                + '<div class="station">'
                                + '<span {{bindAttr class="SDL.CDModel.active:visible_display"}}>{{SDL.CDModel.statusBar}}</span>'
                                + // title
                                '<span {{bindAttr class="SDL.SDLModel.limitedExist:visible_display"}}>{{SDL.SDLModel.applicationStatusBar}}</span>'
                                + // SDL
                                '</div>'
                                + '<div class="icon cdIco"'
                                + '{{bindAttr class="SDL.CDModel.active:cdIco"}}'
                                + // CD
                                // icon
                                '{{bindAttr class="SDL.SDLModel.limitedExist:appIco"}}>'
                                + // SDL
                                '</div>' + '</div>')
                    })
            }),
        actionUp: function(event) {

            if (SDL.MediaController.activeState.indexOf('media.sdlmedia') >= 0) {
                SDL.SDLMediaController.activateCurrentApp();
            } else {
                SDL.States.goToStates(SDL.MediaController.activeState);
            }
        }
    });
