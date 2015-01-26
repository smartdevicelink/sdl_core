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
 * @name SDL.SDLMediaControlls
 * @desc SDLMediaControlls visual representation
 * @category View
 * @filesource app/view/media/sdl/controllsView.js
 * @version 1.0
 */

SDL.SDLMediaControlls = Em.ContainerView.create( {

    childViews:
        [
            'info',
            'Controls',
            'tuneButtons' // Uncomment to see preset buttons on sdl screen
        ],

    /**
     * Verification paramiter
     */
    sdlMediaControlls: "V2",

    info: Em.View.extend( {

        elementId: 'app_view_info',

        template: Em.Handlebars
            .compile('{{#with view}}'
                + '<div class="track-info">'
                + '<div class="device">{{SDL.SDLAppController.model.deviceName}}</div>'
                + '<div class="divider_o"></div>'
                + '<div class="title textLimit" {{bindAttr style="SDL.SDLAppController.model.appInfo.alignment"}}>{{SDL.SDLAppController.model.appInfo.field1}}</div>'
                + '<div class="album textLimit" {{bindAttr style="SDL.SDLAppController.model.appInfo.alignment"}}>{{SDL.SDLAppController.model.appInfo.field2}}</div>'
                + '<div class="artist textLimit"{{bindAttr class="SDL.SDLAppController.model.mediaPreset:hidden"}}>{{SDL.SDLAppController.model.appInfo.field3}}</div>'
                + '<div class="time"{{bindAttr class="SDL.SDLAppController.model.mediaPreset:timeV2"}}>{{SDL.SDLAppController.model.appInfo.mediaClock}}</div>'
                + '<img class="cd_logo" {{bindAttr src="SDL.SDLAppController.model.appInfo.trackIcon" class="SDL.SDLAppController.model.mediaPreset:cd_logoV2"}} />'
                + '</div>' + '{{/with}}')
    }),

    Controls: Em.ContainerView.create( {

        elementId: 'app_view_controlls',

        /** View components */
        childViews:
            [
                'PrevTrackButton',
                'PlayButton',
                'NextTrackButton'
            ],

        classNames: 'player_controlls',

        classNameBindings: 'SDL.SDLAppController.model.mediaPreset:player_controllsV2',

        PrevTrackButton: SDL.Button.extend( SDL.PresetEvents, {
            elementId: 'app_view_controlls_prev_track_button_v2',
            classNames:
                [
                    'bc-item-big',
                    'prevcd'
                ],
            icon: 'images/media/ico_prew.png',
            presetName: 'SEEKLEFT'
        } ),

        PlayButton: SDL.Button.extend( {
            elementId: 'app_view_controlls_play_button_v2',
            classNames:
                [
                    'bc-item-big',
                    'playcd'
                ],
            presetName: 'OK',
            actionUp: function() {
                SDL.SDLController.onSoftButtonOkActionUp( this.presetName );
            },
            actionDown: function() {
                SDL.SDLController.onSoftButtonOkActionDown( this.presetName );
            },
            /**
             * Define button
             * template
             */
            template: Ember.Handlebars
                            .compile( '<img class="playIcon" {{bindAttr class="SDL.SDLAppController.model.isPlaying:visible:not-visible"}} src="images/media/ico_pause.png" />'
                                            + '<img class="playIcon not-visible" {{bindAttr class="SDL.SDLAppController.model.isPlaying:not-visible:visible"}} src="images/media/ico-play.png" />' )
        } ),
        NextTrackButton: SDL.Button.extend( SDL.PresetEvents, {
            elementId: 'app_view_controlls_next_track_button_v2',
            classNames:
                [
                    'bc-item-big',
                    'nextcd'
                ],
            icon: 'images/media/ico_next.png',
            presetName: 'SEEKRIGHT'
        } )
    } ),

    tuneButtons: Em.ContainerView.create( {

        elementId: 'sdl_media_presetButtons',

        classNameBindings:
            [
                'SDL.SDLAppController.model.mediaPreset::hidden'
            ],

        classNames:
            [
                'main-preset-buttons-wraper'
            ],

        childViews:
            [
                'wrapper'
            ],

        wrapper: Em.ContainerView.extend( {

            elementId: 'sdl_media_presetButtons_wrapper',

            classNames: 'preset-items',

            childViews:
                [
                    '1',
                    '2',
                    '3',
                    '4',
                    '5',
                    '6',
                    '7',
                    '8',
                    '9',
                    '10'
                ],

            1: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button1',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.0',
                templateName: 'text',
                presetName: 'PRESET_0'
            } ),
            2: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button2',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.1',
                templateName: 'text',
                presetName: 'PRESET_1'
            } ),
            3: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button3',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.2',
                templateName: 'text',
                presetName: 'PRESET_2'
            } ),
            4: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button4',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.3',
                templateName: 'text',
                presetName: 'PRESET_3'
            } ),
            5: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button5',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.4',
                templateName: 'text',
                presetName: 'PRESET_4'
            } ),
            6: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button6',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.5',
                templateName: 'text',
                presetName: 'PRESET_5'
            } ),
            7: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button7',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.6',
                templateName: 'text',
                presetName: 'PRESET_6'
            } ),
            8: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button8',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.7',
                templateName: 'text',
                presetName: 'PRESET_7'
            } ),
            9: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button9',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.8',
                templateName: 'text',
                presetName: 'PRESET_8'
            } ),
            10: SDL.Button.extend( SDL.PresetEvents, {
                elementId: 'sdl_media_preset_button10',
                classNames: 'preset-item',
                textBinding: 'SDL.SDLAppController.model.appInfo.customPresets.9',
                templateName: 'text',
                presetName: 'PRESET_9'
            } )
        } )
    } )
} );
