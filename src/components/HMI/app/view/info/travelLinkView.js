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
 * @name SDL.InfoTravelLinkView
 * @desc Info Travel Link Help module visual representation
 * @category View
 * @filesource app/view/settings/InfoTravelLinkView.js
 * @version 1.0
 */

SDL.InfoTravelLinkView = Em.ContainerView
    .create( {

        classNameBindings: [
            'SDL.States.info.travelLink.active:active_state:inactive_state'
        ],

        elementId: 'info_travelLink',

        childViews: [
            'imageLogo', 'listLeft', 'listRight',
        ],

        imageLogo: SDL.Label.extend( {
            elementId: 'info_travelLink_logo',
            icon: 'images/info/info_travelLink_sirius_logo.png',
            templateName: 'icon'
        }),

        listLeft: SDL.List
            .extend( {
                elementId: 'info_travelLink_listLeft',

                disableScrollbar: true,

                itemsOnPage: 5,

                /** Items */
                items: [
                    {

                        type: SDL.Button,

                        params: {
                            textBinding: 'SDL.locale.label.view_info_travelLink_trafficOnRoute',
                            templateName: 'arrow',
                            disabled: true
                        }

                    },
                    {

                        type: SDL.Button,

                        params: {
                            textBinding: 'SDL.locale.label.view_info_travelLink_trafficNearby',
                            templateName: 'arrow',
                            disabled: true
                        }

                    },
                    {

                        type: SDL.Button,

                        params: {
                            textBinding: 'SDL.locale.label.view_info_travelLink_fuelPrices',
                            templateName: 'arrow',
                            disabled: true
                        }

                    },
                    {

                        type: SDL.Button,

                        params: {
                            textBinding: 'SDL.locale.label.view_info_travelLink_movieListings',
                            templateName: 'arrow',
                            disabled: true
                        }

                    },
                ]
            }),

        listRight: SDL.List
            .extend( {
                elementId: 'info_travelLink_listRight',

                disableScrollbar: true,

                /** Items */
                items: [
                    {

                        type: SDL.Button,

                        params: {

                            textBinding: 'SDL.locale.label.view_info_travelLink_subscriptionInfo',
                            disabled: true
                        }

                    },
                    {

                        type: SDL.Button,

                        params: {
                            textBinding: 'SDL.locale.label.view_info_travelLink_weather',
                            templateName: 'arrow',
                            disabled: true
                        }

                    },
                    {

                        type: SDL.Button,

                        params: {
                            textBinding: 'SDL.locale.label.view_info_travelLink_sportsInfo',
                            templateName: 'arrow',
                            disabled: true
                        }

                    },
                    {

                        type: SDL.Button,

                        params: {
                            textBinding: 'SDL.locale.label.view_info_travelLink_skiConditions',
                            templateName: 'arrow',
                            disabled: true
                        }

                    }
                ]
            })
    });