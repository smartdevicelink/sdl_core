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
            'Controls',
            'tuneButtons'
        ],

        /**
         * Verification paramiter
         */
        applinkMediaControlls: "V2",

        info:   Em.View.extend({
            
            elementId: 'app_view_info',
            
            template: Em.Handlebars.compile(
                '{{#with view}}'+
                '<div class="track-info">'+
                    '<div class="device">{{MFT.ApplinkAppController.model.deviceName}}</div>'+
                    '<div class="divider_o"></div>'+
                    '<div class="title textLimit">{{MFT.ApplinkAppController.model.appInfo.field1}}</div>'+
                    '<div class="album textLimit">{{MFT.ApplinkAppController.model.appInfo.field2}}</div>'+
                    '<div class="artist textLimit">{{MFT.ApplinkAppController.model.appInfo.mediaClock}}</div>'+
                    '<img class="cd_logo" {{bindAttr src="MFT.ApplinkAppController.model.appInfo.trackIcon" class="MFT.ApplinkController.protocolVersion2State:cd_logoV2"}} />'+
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

            classNames:         'player_controlls',

            classNameBindings:  'MFT.ApplinkController.protocolVersion2State:player_controllsV2',
            
            PrevTrackButton: MFT.Button.extend({
                elementId:          'app_view_controlls_prev_track_button_v2',
                classNames:         ['bc-item-big', 'prevcd'],
                classNames:         ['bc-item-big', 'prevcd'],
                actionDown:     function(){
                    this._super();
                    MFT.ApplinkController.onSoftButtonActionDown('SEEKLEFT', this);
                },
                actionUp:       function(){
                    this._super();
                    MFT.ApplinkController.onSoftButtonActionUp('SEEKLEFT', this);
                },
                icon:               'images/media/ico_prew.png',
            }),
            PlayButton: MFT.Button.extend({
                elementId:          'app_view_controlls_play_button_v2',
                classNames:         ['bc-item-big', 'playcd'],
                actionDown:     function(){
                    this._super();
                    MFT.ApplinkController.onSoftButtonActionDown('OK', this);
                },
                actionUp:       function(){
                    this._super();
                    MFT.ApplinkController.onSoftButtonActionUp('OK', this);
                    MFT.ApplinkAppController.model.set('isPlaying', !MFT.ApplinkAppController.model.isPlaying);
                },
                /** Define button template */
                template: Ember.Handlebars.compile(
                    //'<img class="playIcon hideicon"{{bindAttr class="MFT.ApplinkAppController.model.isPlaying:visible"}} src="images/media/ico_pause.png" />'+
                    //'<img class="playIcon showicon"{{bindAttr class="MFT.ApplinkAppController.model.isPlaying:not-visible"}} src="images/media/ico-play.png" />'
                    '<img class="playIcon" {{bindAttr class="MFT.ApplinkAppController.model.isPlaying:visible:not-visible"}} src="images/media/ico_pause.png" />'+
                    '<img class="playIcon not-visible" {{bindAttr class="MFT.ApplinkAppController.model.isPlaying:not-visible:visible"}} src="images/media/ico-play.png" />'
                )
            }),
            NextTrackButton: MFT.Button.extend({
                elementId:          'app_view_controlls_next_track_button_v2',
                classNames:         ['bc-item-big', 'nextcd'],
                actionDown:     function(){
                    this._super();
                    MFT.ApplinkController.onSoftButtonActionDown('SEEKRIGHT', this);
                },
                actionUp:       function(){
                    this._super();
                    MFT.ApplinkController.onSoftButtonActionUp('SEEKRIGHT', this);
                },
                icon:               'images/media/ico_next.png',
            })
        }),


    tuneButtons: Em.ContainerView.create({
    
        elementId: 'applink_media_presetButtons',
        
        classNameBindings:  [
                                'MFT.ApplinkController.protocolVersion2State::hidden'
                            ],
        
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
                textBinding:    'MFT.ApplinkAppController.model.appInfo.customPresets.0',
                templateName:   'text',
                actionDown:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_1", "BUTTONDOWN");
                    var self = this;
                    this.time = 0;
                    setTimeout(function(){ self.time ++; }, 1000);
                },
                actionUp:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_1", "BUTTONUP");
                    if(this.time > 0){
                        FFW.Buttons.buttonPressed( "PRESET_1", "LONG");
                    }else{
                        FFW.Buttons.buttonPressed( "PRESET_1", "SHORT");
                    }
                    this.time = 0;
                }
            }),
            2: MFT.Button.extend({
                elementId:      'applink_media_preset_button2',
                classNames:     'a1 preset-item',
                textBinding:    'MFT.ApplinkAppController.model.appInfo.customPresets.1',
                templateName:   'text',
                actionDown:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_2", "BUTTONDOWN");
                    var self = this;
                    this.time = 0;
                    setTimeout(function(){ self.time ++; }, 1000);
                },
                actionUp:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_2", "BUTTONUP");
                    if(this.time > 0){
                        FFW.Buttons.buttonPressed( "PRESET_2", "LONG");
                    }else{
                        FFW.Buttons.buttonPressed( "PRESET_2", "SHORT");
                    }
                    this.time = 0;
                }
            }),
            3: MFT.Button.extend({
                elementId: 'applink_media_preset_button3',
                classNames: 'a2 preset-item',
                textBinding:    'MFT.ApplinkAppController.model.appInfo.customPresets.2',
                templateName:   'text',
                actionDown:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_3", "BUTTONDOWN");
                    var self = this;
                    this.time = 0;
                    setTimeout(function(){ self.time ++; }, 1000);
                },
                actionUp:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_3", "BUTTONUP");
                    if(this.time > 0){
                        FFW.Buttons.buttonPressed( "PRESET_3", "LONG");
                    }else{
                        FFW.Buttons.buttonPressed( "PRESET_3", "SHORT");
                    }
                    this.time = 0;
                }
            }),
            4: MFT.Button.extend({
                elementId: 'applink_media_preset_button4',
                classNames: 'a3 preset-item',
                textBinding:    'MFT.ApplinkAppController.model.appInfo.customPresets.3',
                templateName:   'text',
                actionDown:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_4", "BUTTONDOWN");
                    var self = this;
                    this.time = 0;
                    setTimeout(function(){ self.time ++; }, 1000);
                },
                actionUp:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_4", "BUTTONUP");
                    if(this.time > 0){
                        FFW.Buttons.buttonPressed( "PRESET_4", "LONG");
                    }else{
                        FFW.Buttons.buttonPressed( "PRESET_4", "SHORT");
                    }
                    this.time = 0;
                }
            }),
            5: MFT.Button.extend({
                elementId: 'applink_media_preset_button5',
                classNames: 'a4 preset-item',
                textBinding:    'MFT.ApplinkAppController.model.appInfo.customPresets.4',
                templateName:   'text',
                actionDown:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_5", "BUTTONDOWN");
                    var self = this;
                    this.time = 0;
                    setTimeout(function(){ self.time ++; }, 1000);
                },
                actionUp:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_5", "BUTTONUP");
                    if(this.time > 0){
                        FFW.Buttons.buttonPressed( "PRESET_5", "LONG");
                    }else{
                        FFW.Buttons.buttonPressed( "PRESET_5", "SHORT");
                    }
                    this.time = 0;
                }
            }),
            6: MFT.Button.extend({
                elementId: 'applink_media_preset_button6',
                classNames: 'a5 preset-item',
                textBinding:    'MFT.ApplinkAppController.model.appInfo.customPresets.5',
                templateName:   'text',
                actionDown:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_6", "BUTTONDOWN");
                    var self = this;
                    this.time = 0;
                    setTimeout(function(){ self.time ++; }, 1000);
                },
                actionUp:       function(){
                    this._super();
                    FFW.Buttons.buttonEvent( "PRESET_6", "BUTTONUP");
                    if(this.time > 0){
                        FFW.Buttons.buttonPressed( "PRESET_6", "LONG");
                    }else{
                        FFW.Buttons.buttonPressed( "PRESET_6", "SHORT");
                    }
                    this.time = 0;
                }
            })
        })
    })
});