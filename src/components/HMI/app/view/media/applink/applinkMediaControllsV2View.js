/**
 * @name MFT.ApplinkMediaControllsV2
 * 
 * @desc ApplinkMediaControllsV2 visual representation
 * 
 * @category    View
 * @filesource  app/view/media/applink/applinkMediaControllsViewV2.js
 * @version     2.0
 *
 * @author      Melnik Andriy
 */

MFT.ApplinkMediaControllsV2 = Em.ContainerView.create({
        
        childViews: [
            'info',
           // 'Controls',
            'tuneButtons'
        ],

        info:   Em.View.extend({
            
            elementId: 'app_view_info',
            
            template: Em.Handlebars.compile(
                '{{#with view}}'+
                '<div class="track-info">'+
                    '<div class="device">{{MFT.ApplinkMediaModel.showInfo.deviceName}}</div>'+
                            '<div class="divider_o"></div>'+
                    '<div class="title">{{MFT.ApplinkMediaModel.showInfo.field1}}</div>'+
                    '<div class="album">{{MFT.ApplinkMediaModel.showInfo.field2}}</div>'+
                    '<div class="artist">{{MFT.ApplinkMediaModel.showInfo.field3}}</div>'+
                    '<div class="time">{{MFT.ApplinkMediaModel.showInfo.mediaClock}}</div>'+
    //              '<div id="cd_logo" {{bindAttr class="MFT.CDModel.active:visible_display MFT.AppModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
    //              '<div id="usb_logo" {{bindAttr class="MFT.USBModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
                '</div>'+
                '{{/with}}'
            )
        }),

        Controls:   Em.ContainerView.create({

            elementId: 'app_view_controlls',

            /** View components*/
            childViews: [
                'PrevTrackButton',
                'PlayButton',
                'NextTrackButton'
            ],

            classNames: 'player_controlls',
            
            PrevTrackButton: MFT.Button.extend({
                elementId:          'app_view_controlls_prev_track_button',
                classNames:         ['bc-item-big', 'prevcd'],
                classNames:         ['bc-item-big', 'prevcd'],
                target:             'MFT.MediaController',
                action:             'prevTrack',
                icon:               'images/media/ico_prew.png',
            }),
            PlayButton: MFT.Button.extend({
                elementId:          'app_view_controlls_play_button',
                classNames:         ['bc-item-big', 'playcd'],
                target:             'MFT.MediaController',
                action:             'playTrack',
                /** Define button template */
                template: Ember.Handlebars.compile(
                    '<img class="playIcon hideicon"{{bindAttr class="MFT.MediaController.currentSelectedPlayer.isPlaying:visible"}} src="images/media/ico_pause.png" />'+
                    '<img class="playIcon showicon"{{bindAttr class="MFT.MediaController.currentSelectedPlayer.isPlaying:not-visible"}} src="images/media/ico-play.png" />'
                )
            }),
            NextTrackButton: MFT.Button.extend({
                elementId:          'app_view_controlls_next_track_button',
                classNames:         ['bc-item-big', 'nextcd'],
                target:             'MFT.MediaController',
                action:             'nextTrack',
                icon:                 'images/media/ico_next.png',
            })
        }),


    tuneButtons: Em.ContainerView.create({
    
        elementId: 'applink_media_presetButtons',
        
        classNameBindings:  ['MFT.States.media.applink.active:active_state'],
        
        classNames: ['main-preset-buttons-wraper'],
        
        childViews: ['wrapper'],
        
        wrapper: Em.ContainerView.extend({

            elementId: 'applink_media_presetButtons_wrapper',
            
            classNames: 'preset-items',

            childViews: [
                '1',
                '2',
                '3',
                '4',
                '5',
                '6'
            ],

            1: MFT.Button.extend({
                elementId: 'applink_media_preset_button1',
                classNames: 'a0 preset-item',
                textBinding:    'MFT.ApplinkMediaModel.showInfo.customPresets.0',
                templateName:   'text'
                //contentBinding:  Ember.Binding.oneWay( 'parentView.presetData.items.0'),
                // num: 1,
                //timer: 2000,
                //dataBinding:  Ember.Binding.oneWay('parentView.presetData')
            }),
            2: MFT.Button.extend({
                elementId:      'applink_media_preset_button2',
                classNames:     'a1 preset-item',
                textBinding:    'MFT.ApplinkMediaModel.showInfo.customPresets.1'
                //contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.1'),
                //num: 2,
                //timer: 2000,
                //dataBinding:  Ember.Binding.oneWay('parentView.presetData')
            }),
            3: MFT.Button.extend({
                elementId: 'applink_media_preset_button3',
                classNames: 'a2 preset-item',
                textBinding:    'MFT.ApplinkMediaModel.showInfo.customPresets.2'
                //contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.2'),
                //num: 3,
                //timer: 2000,
                //dataBinding:  Ember.Binding.oneWay('parentView.presetData')
            }),
            4: MFT.Button.extend({
                elementId: 'applink_media_preset_button4',
                classNames: 'a3 preset-item',
                textBinding:    'MFT.ApplinkMediaModel.showInfo.customPresets.3'
                //contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.3'),
                //num: 4,
                //timer: 2000,
                //dataBinding:  Ember.Binding.oneWay('parentView.presetData')
            }),
            5: MFT.Button.extend({
                elementId: 'applink_media_preset_button5',
                classNames: 'a4 preset-item',
                textBinding:    'MFT.ApplinkMediaModel.showInfo.customPresets.4'
                //contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.4'),
                //num: 5,
                //timer: 2000,
                //dataBinding:  Ember.Binding.oneWay('parentView.presetData')
            }),
            6: MFT.Button.extend({
                elementId: 'applink_media_preset_button6',
                classNames: 'a5 preset-item',
                textBinding:    'MFT.ApplinkMediaModel.showInfo.customPresets.5'
                //contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.5'),
                //num: 6,
                //timer: 2000,
                //dataBinding:  Ember.Binding.oneWay('parentView.presetData')
            })
        })
    })
});