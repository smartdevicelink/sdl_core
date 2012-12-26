/**
 * @name MFT.ApplinkMediaControllsV1
 * 
 * @desc ApplinkMediaControllsV1 visual representation
 * 
 * @category    View
 * @filesource  app/view/media/applink/applinkMediaControllsViewV1.js
 * @version     2.0
 *
 * @author      Melnik Andriy
 */

MFT.ApplinkMediaControllsV1 = Em.ContainerView.create({
        
        childViews: [
            'info',
            'Controls'
        ],

        /**
         * Verification paramiter
         */
        applinkMediaControlls: "V1",

        info:   Em.View.extend({
            
            elementId: 'app_view_info',
            
            template: Em.Handlebars.compile(
                '{{#with view}}'+
                '<div class="track-info">'+
                    '<div class="device">{{MFT.ApplinkModel.deviceName}}</div>'+
                            '<div class="divider_o"></div>'+
                    '<div class="title textLimit">{{MFT.ApplinkMediaModel.appInfo.field1}}</div>'+
                    '<div class="album textLimit">{{MFT.ApplinkMediaModel.appInfo.field2}}</div>'+
                    '<div class="artist textLimit">{{MFT.ApplinkMediaModel.appInfo.field3}}</div>'+
                    '<div class="time">{{MFT.ApplinkMediaModel.appInfo.mediaClock}}</div>'+
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
        })
});