/**
 * @name MFT.SDLMediaControlls
 * @desc SDLMediaControlls visual representation
 * @category View
 * @filesource app/view/media/sdl/controllsView.js
 * @version 2.0
 * @author Melnik Andriy
 */

MFT.SDLMediaControlls = Em.ContainerView.create( {

    childViews:
        [
            'info',
            'Controls',
            'tuneButtons'
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
                                        + '<div class="device">{{MFT.SDLAppController.model.deviceName}}</div>'
                                        + '<div class="divider_o"></div>'
                                        + '<div class="title textLimit">{{MFT.SDLAppController.model.appInfo.field1}}</div>'
                                        + '<div class="album textLimit">{{MFT.SDLAppController.model.appInfo.field2}}</div>'
                                        + '<div class="artist textLimit"{{bindAttr class="MFT.SDLController.protocolVersion2State:hidden"}}>{{MFT.SDLAppController.model.appInfo.field3}}</div>'
                                        + '<div class="time"{{bindAttr class="MFT.SDLController.protocolVersion2State:timeV2"}}>{{MFT.SDLAppController.model.appInfo.mediaClock}}</div>'
                                        + '<img class="cd_logo" {{bindAttr src="MFT.SDLAppController.model.appInfo.trackIcon" class="MFT.SDLController.protocolVersion2State:cd_logoV2"}} />'
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

        classNameBindings: 'MFT.SDLController.protocolVersion2State:player_controllsV2',

        PrevTrackButton: MFT.Button.extend(MFT.PresetEvents, {
            elementId: 'app_view_controlls_prev_track_button_v2',
            classNames:
                [
                    'bc-item-big',
                    'prevcd'
                ],
            classNames:
                [
                    'bc-item-big',
                    'prevcd'
                ],
            icon: 'images/media/ico_prew.png',
            presetName: 'SEEKLEFT'
        }),
        PlayButton: MFT.Button.extend( {
            elementId: 'app_view_controlls_play_button_v2',
            classNames:
                [
                    'bc-item-big',
                    'playcd'
                ],
            presetName: 'OK',
            actionUp: function() {
                MFT.SDLController.onSoftButtonOkActionUp(this.presetName);
            },
            actionDown: function() {
                MFT.SDLController.onSoftButtonOkActionDown(this.presetName);
            },
            /**
             * Define button
             * template
             */
            template: Ember.Handlebars
                            .compile('<img class="playIcon" {{bindAttr class="MFT.SDLAppController.model.isPlaying:visible:not-visible"}} src="images/media/ico_pause.png" />'
                                            + '<img class="playIcon not-visible" {{bindAttr class="MFT.SDLAppController.model.isPlaying:not-visible:visible"}} src="images/media/ico-play.png" />')
        }),
        NextTrackButton: MFT.Button.extend(MFT.PresetEvents, {
            elementId: 'app_view_controlls_next_track_button_v2',
            classNames:
                [
                    'bc-item-big',
                    'nextcd'
                ],
            icon: 'images/media/ico_next.png',
            presetName: 'SEEKRIGHT'
        })
    }),

    tuneButtons: Em.ContainerView.create( {

        elementId: 'sdl_media_presetButtons',

        classNameBindings:
            ['MFT.SDLController.protocolVersion2State::hidden'
            ],

        classNames:
            ['main-preset-buttons-wraper'
            ],

        childViews:
            ['wrapper'
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
                    '6'
                ],

            1: MFT.Button.extend(MFT.PresetEvents, {
                elementId: 'sdl_media_preset_button1',
                classNames: 'a0 preset-item',
                textBinding: 'MFT.SDLAppController.model.appInfo.customPresets.0',
                templateName: 'text',
                presetName: 'PRESET_1'
            }),
            2: MFT.Button.extend(MFT.PresetEvents, {
                elementId: 'sdl_media_preset_button2',
                classNames: 'a1 preset-item',
                textBinding: 'MFT.SDLAppController.model.appInfo.customPresets.1',
                templateName: 'text',
                presetName: 'PRESET_2'
            }),
            3: MFT.Button.extend(MFT.PresetEvents, {
                elementId: 'sdl_media_preset_button3',
                classNames: 'a2 preset-item',
                textBinding: 'MFT.SDLAppController.model.appInfo.customPresets.2',
                templateName: 'text',
                presetName: 'PRESET_3'
            }),
            4: MFT.Button.extend(MFT.PresetEvents, {
                elementId: 'sdl_media_preset_button4',
                classNames: 'a3 preset-item',
                textBinding: 'MFT.SDLAppController.model.appInfo.customPresets.3',
                templateName: 'text',
                presetName: 'PRESET_4'
            }),
            5: MFT.Button.extend(MFT.PresetEvents, {
                elementId: 'sdl_media_preset_button5',
                classNames: 'a4 preset-item',
                textBinding: 'MFT.SDLAppController.model.appInfo.customPresets.4',
                templateName: 'text',
                presetName: 'PRESET_5'
            }),
            6: MFT.Button.extend(MFT.PresetEvents, {
                elementId: 'sdl_media_preset_button6',
                classNames: 'a5 preset-item',
                textBinding: 'MFT.SDLAppController.model.appInfo.customPresets.5',
                templateName: 'text',
                presetName: 'PRESET_6'
            })
        })
    })
});