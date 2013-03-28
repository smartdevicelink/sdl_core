/**
 * @name MFT.SDLMediaControllsV1
 * 
 * @desc SDLMediaControllsV1 visual representation
 * 
 * @category    View
 * @filesource  app/view/media/sdl/sdlMediaControllsViewV1.js
 * @version     2.0
 *
 * @author      Melnik Andriy
 */

MFT.SDLMediaControllsV1 = Em.ContainerView.create({
        
        childViews: [
            'info',
            'Controls'
        ],

        /**
         * Verification paramiter
         */
        sdlMediaControlls: "V1",

        info:   Em.View.extend({
            
            elementId: 'app_view_info',
            
            template: Em.Handlebars.compile(
                '{{#with view}}'+
                '<div class="track-info">'+
                    '<div class="device">{{MFT.SDLAppController.model.deviceName}}</div>'+
                            '<div class="divider_o"></div>'+
                    '<div class="title textLimit">{{MFT.SDLAppController.model.appInfo.field1}}</div>'+
                    '<div class="album textLimit">{{MFT.SDLAppController.model.appInfo.field2}}</div>'+
                    '<div class="artist textLimit">{{MFT.SDLAppController.model.appInfo.field3}}</div>'+
                    '<div class="time">{{MFT.SDLAppController.model.appInfo.mediaClock}}</div>'+
                    '<img class="cd_logo" {{bindAttr src="MFT.SDLAppController.model.appInfo.trackIcon"}} />'+
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
                    MFT.SDLController.onSoftButtonActionDown('SEEKLEFT', this);
                },
                actionUp:       function(){
                    this._super();
                    MFT.SDLController.onSoftButtonActionUp('SEEKLEFT', this);
                },
                icon:               'images/media/ico_prew.png',
            }),
            PlayButton: MFT.Button.extend({
                elementId:          'app_view_controlls_play_button',
                classNames:         ['bc-item-big', 'playcd'],
                actionDown:     function(){
                    this._super();
                    MFT.SDLController.onSoftButtonActionDown('OK', this);
                },
                actionUp:       function(){
                    this._super();
                    MFT.SDLController.onSoftButtonActionUp('OK', this);
                },
                /** Define button template */
                template: Ember.Handlebars.compile(
                    '<img class="playIcon hideicon"{{bindAttr class="MFT.SDLAppController.model.isPlaying:visible"}} src="images/media/ico_pause.png" />'+
                    '<img class="playIcon showicon"{{bindAttr class="MFT.SDLAppController.model.isPlaying:not-visible"}} src="images/media/ico-play.png" />'
                )
            }),
            NextTrackButton: MFT.Button.extend({
                elementId:          'app_view_controlls_next_track_button',
                classNames:         ['bc-item-big', 'nextcd'],
                actionDown:     function(){
                    this._super();
                    MFT.SDLController.onSoftButtonActionDown('SEEKRIGHT', this);
                },
                actionUp:       function(){
                    this._super();
                    MFT.SDLController.onSoftButtonActionUp('SEEKRIGHT', this);
                },
                icon:                 'images/media/ico_next.png',
            })
        })
});