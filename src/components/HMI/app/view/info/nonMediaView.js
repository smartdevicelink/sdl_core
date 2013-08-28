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
 * @name SDL.InfoNonMedia
 * @desc NonMedia application visual representation
 * @category View
 * @filesource app/view/info/nonMedia.js
 * @version 1.0
 */

SDL.InfoNonMedia = Em.ContainerView
    .create( {

        elementId: 'info_nonMedia',

        controller: SDL.SDLAppController,

        childViews: [
            'content'
        ],

        classNameBindings: [
            'SDL.States.info.nonMedia.active:active_state:inactive_state'
        ],

        content: Em.ContainerView
            .extend( {
                classNames: [
                    'inner_content'
                ],

                childViews: [
                    'deviceName', 'image', 'fields', 'innerMenu', 'presets'
                ],

                /**
                 * Deactivate View
                 */
                deactivate: function() {

                    SDL.States.goToStates('info.apps');
                },

                deviceName: SDL.Label.extend( {

                    classNames: [
                        'app_title'
                    ],

                    contentBinding: 'SDL.SDLAppController.model.deviceName'
                }),

                image: Em.View
                    .extend( {

                        classNames: [
                            'app_image'
                        ],

                        template: Ember.Handlebars
                            .compile('<img width="150px" {{bindAttr src="SDL.SDLAppController.model.appInfo.mainImage"}}>')
                    }),

                fields: Em.ContainerView.extend( {

                    classNames: [
                        'app_fields'
                    ],

                    childViews: [
                        'field1', 'field2', 'field3', 'field4'
                    ],

                    field1: SDL.Label.extend( {
                        contentBinding: 'controller.model.appInfo.field1',
                        attributeBindings: ["style"],
                        style: function(){
                            if (SDL.SDLAppController.model) {
                                return SDL.SDLAppController.model.appInfo.alignment;
                            } else {
                                return null;
                            }
                        }.property("SDL.SDLAppController.model.appInfo.alignment")
                    }),

                    field2: SDL.Label.extend( {
                        contentBinding: 'controller.model.appInfo.field2',
                        attributeBindings: ["style"],
                        style: function(){
                            if (SDL.SDLAppController.model) {
                                return SDL.SDLAppController.model.appInfo.alignment;
                            } else {
                                return null;
                            }
                        }.property("SDL.SDLAppController.model.appInfo.alignment")
                    }),

                    field3: SDL.Label.extend( {
                        contentBinding: 'controller.model.appInfo.field3'
                    }),

                    field4: SDL.Label.extend( {
                        contentBinding: 'controller.model.appInfo.field4'
                    })

                }),

                innerMenu: SDL.MenuList
                    .extend( {

                        refreshItems: function() {

                            if (SDL.SDLAppController.model.appID == SDL.NonMediaController.currentAppId) {
                                this
                                    .addItems(SDL.SDLAppController.model.softButtons,
                                        SDL.SDLAppController.model.appID);
                            }
                        }
                            .observes('SDL.SDLAppController.model.softButtons.@each'),

                        groupName: "NonMediaView",

                        content: Em.ContainerView.extend( {

                            classNames: [
                                'content'
                            ],

                            attributeBindings: [
                                'parentView.contentPositon:style'
                            ],

                            childViews: [
                                'optionsButton'
                            ],

                            optionsButton: SDL.Button.extend( {
                                text: 'Options',

                                templateName: 'arrow',

                                action: 'openCommandsList',
                                target: 'SDL.SDLAppController'
                            })
                        })
                    }),

                presets: Em.ContainerView
                    .extend( {
                        classNames: [
                            'presets'
                        ],

                        classNameBindings: [
                            'hidden'
                        ],

                        hidden: function() {

                            if (SDL.SDLAppController.model) { return !Boolean(SDL.SDLAppController.model
                                .get('appInfo.customPresets').length); }

                        }
                            .property('SDL.SDLAppController.model.appInfo.customPresets.@each'),

                        childViews: [
                            'perset1',
                            'perset2',
                            'perset3',
                            'perset4',
                            'perset5',
                            'perset6'
                        ],

                        perset1: SDL.SDLPresetButton
                            .extend( {
                                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.0',
                                presetName: 'PRESET_1'
                            }),
                        perset2: SDL.SDLPresetButton
                            .extend( {
                                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.1',
                                presetName: 'PRESET_2'
                            }),
                        perset3: SDL.SDLPresetButton
                            .extend( {
                                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.2',
                                presetName: 'PRESET_3'
                            }),
                        perset4: SDL.SDLPresetButton
                            .extend( {
                                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.3',
                                presetName: 'PRESET_4'
                            }),
                        perset5: SDL.SDLPresetButton
                            .extend( {
                                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.4',
                                presetName: 'PRESET_5'
                            }),
                        perset6: SDL.SDLPresetButton
                            .extend( {
                                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.5',
                                presetName: 'PRESET_6'
                            })
                    })
            })
    });