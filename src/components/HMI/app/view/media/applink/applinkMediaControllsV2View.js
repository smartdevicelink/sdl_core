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
                    '<div class="device">{{MFT.ApplinkModel.deviceName}}</div>'+
                    '<div class="divider_o"></div>'+
                    '<div class="title textLimit">{{MFT.ApplinkMediaModel.appInfo.field1}}</div>'+
                    '<div class="album textLimit">{{MFT.ApplinkMediaModel.appInfo.field2}}</div>'+
                    '<div class="artist textLimit">{{MFT.ApplinkMediaModel.appInfo.mediaClock}}</div>'+
    //              '<div id="cd_logo" {{bindAttr class="MFT.CDModel.active:visible_display MFT.AppModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
    //              '<div id="usb_logo" {{bindAttr class="MFT.USBModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
                '</div>'+
                '{{/with}}'
            )
        }),

         Controls:   Em.ContainerView.create({

            elementId: 'app_view_controllsV2',

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
                actionDown:     function(){
                    this._super();
                    FFW.Buttons.buttonEvent( 'SEEKLEFT', "BUTTONDOWN");
                    var self = this;
                    this.time = 0;
                    setTimeout(function(){ self.time ++; }, 1000);
                },
                actionUp:       function(){
                    this._super();
                    if(this.time > 0){
                        FFW.Buttons.buttonPressed( 'SEEKLEFT', "LONG");
                    }else{
                        FFW.Buttons.buttonPressed( 'SEEKLEFT', "SHORT");
                    }
                    FFW.Buttons.buttonEvent( 'SEEKLEFT', "BUTTONUP");
                    this.time = 0;
                },
                icon:               'images/media/ico_prew.png',
            }),
            PlayButton: MFT.Button.extend({
                elementId:          'app_view_controlls_play_button',
                classNames:         ['bc-item-big', 'playcd'],
                actionDown:     function(){
                    this._super();
                    FFW.Buttons.buttonEvent( 'OK', "BUTTONDOWN");
                    var self = this;
                    this.time = 0;
                    setTimeout(function(){ self.time ++; }, 1000);
                },
                actionUp:       function(){
                    this._super();
                    if(this.time > 0){
                        FFW.Buttons.buttonPressed( 'OK', "LONG");
                    }else{
                        FFW.Buttons.buttonPressed( 'OK', "SHORT");
                    }
                    FFW.Buttons.buttonEvent( 'OK', "BUTTONUP");
                    this.time = 0;
                },
                /** Define button template */
                template: Ember.Handlebars.compile(
                    '<img class="playIcon hideicon"{{bindAttr class="MFT.MediaController.currentSelectedPlayer.isPlaying:visible"}} src="images/media/ico_pause.png" />'+
                    '<img class="playIcon showicon"{{bindAttr class="MFT.MediaController.currentSelectedPlayer.isPlaying:not-visible"}} src="images/media/ico-play.png" />'
                )
            }),
            NextTrackButton: MFT.Button.extend({
                elementId:          'app_view_controlls_next_track_button',
                classNames:         ['bc-item-big', 'nextcd'],
                actionDown:     function(){
                    this._super();
                    FFW.Buttons.buttonEvent( 'SEEKRIGHT', "BUTTONDOWN");
                    var self = this;
                    this.time = 0;
                    setTimeout(function(){ self.time ++; }, 1000);
                },
                actionUp:       function(){
                    this._super();
                    if(this.time > 0){
                        FFW.Buttons.buttonPressed( 'SEEKRIGHT', "LONG");
                    }else{
                        FFW.Buttons.buttonPressed( 'SEEKRIGHT', "SHORT");
                    }
                    FFW.Buttons.buttonEvent( 'SEEKRIGHT', "BUTTONUP");
                    this.time = 0;
                },
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
                textBinding:    'MFT.ApplinkMediaModel.appInfo.customPresets.0',
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
                textBinding:    'MFT.ApplinkMediaModel.appInfo.customPresets.1',
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
                textBinding:    'MFT.ApplinkMediaModel.appInfo.customPresets.2',
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
                textBinding:    'MFT.ApplinkMediaModel.appInfo.customPresets.3',
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
                textBinding:    'MFT.ApplinkMediaModel.appInfo.customPresets.4',
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
                textBinding:    'MFT.ApplinkMediaModel.appInfo.customPresets.5',
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