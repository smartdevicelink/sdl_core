/**
 * @name MFT.BlueToothView
 *
 * @desc Media BlueTooth module visual representation
 *
 * @category	View
 * @filesource	app/view/media/BlueToothView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.bluetoothView = Em.ContainerView.create(MFT.LoadableView,{

    /** View ID*/
    elementId:	'media_bluetooth',

    /** Template*/
    childViews: [
        'content',
        'controlls',
        'rightmenu'
    ],

    content: Em.View.extend({

        template: Ember.Handlebars.compile(
            '<div class="bt-info">'+
                '<div id="bt_info_wrapper">'+
                    '<div id="bt-bg" class="media-blocks bt-block">'+
                        '<span class="bt_title">{{MFT.MediaController.activeSlotBT.trackInfo.title}}</span>'+
                        '<span class="bt_artist">{{MFT.MediaController.activeSlotBT.trackInfo.artist}}</span>'+
                    '</div>'+
                '</div>'+
            '</div>'
        )
    }),
    controlls: Em.ContainerView.extend({

        elementId: 'media_bluetooth_view_controlls',
        /** View components*/
        childViews: [
            'PrevTrackButton',
            'PlayButton',
            'NextTrackButton'
        ],

        classNames: 'player_controlls',

        PrevTrackButton: MFT.Button.extend({
            elementId: 			'media_bluetooth_view_controlls_prev_track_button',
            classNames:			['bc-item-big', 'prevcd'],
            icon:				  'images/media/ico_prew.png',
            disabled:			  true
        }),
        PlayButton: MFT.Button.extend({
            elementId: 			'media_bluetooth_view_controlls_play_button',
            classNames:			['bc-item-big', 'playcd'],
            /** Define button template */
            template: Ember.Handlebars.compile(
                '<img class="playIcon ico hideicon"{{bindAttr class="MFT.MediaController.currentSelectedPlayer.isPlaying:visible"}} src="images/media/ico_pause.png" />'+
                '<img class="playIcon ico showicon"{{bindAttr class="MFT.MediaController.currentSelectedPlayer.isPlaying:not-visible"}} src="images/media/ico-play.png" />'
            ),
            disabled:			  true
        }),
        NextTrackButton: MFT.Button.extend({
            elementId: 			'media_bluetooth_view_controlls_next_track_button',
            classNames:			['bc-item-big', 'nextcd'],
            icon:				  'images/media/ico_next.png',
            disabled:			  true
        })
    }),
    rightmenu:  Em.ContainerView.create({
        /** View ID */
        elementId:	'media_bluetooth_rightmenu',

        /** Class Names */
        classNames: ['right-stock'],

        classNameBindings: [],
        /** View Components*/
        childViews: [
            'optionsButton',
            'repeatButton',
            'shuffleButton'
        ],

        optionsButton:  MFT.Button.extend({
            elementId: 'media_bluetooth_rightmenu_optionsButton',
            classNames:			['rs-item'],
            icon:				  'images/media/active_arrow.png',
            text:		   MFT.locale.label.view_media_bt_options_title,
            target:		   'MFT.MediaController',
            action:		   'turnOnOptions'
        }),

        repeatButton:  MFT.Button.extend({
            elementId: 'media_bluetooth_rightmenu_repeatButton',
            classNames:			['rs-item'],
            classNameBindings: ['MFT.helpMode: hide_icon'],
            icon:				  'images/media/passiv_horiz_led.png',
            textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_repeat'),
            disabled:			  true
        }),

        shuffleButton:  MFT.Button.extend({
            classNameBindings: 	['MFT.helpMode:shuffleButton_help'],
            elementId: 'media_bluetooth_rightmenu_shuffleButton',
            classNames:			['rs-item', 'helpmode_box_shadow'],
            icon:				  'images/media/passiv_horiz_led.png',
            text:		  MFT.locale.label.view_media_shuffle,
            target:			    'MFT.MediaController',
            action:			    'turnShuffleHelpVideoOn',
            onDown:			    false,
            disabledBinding: 'MFT.reversHelpModeBoolean'
        })
    })
});