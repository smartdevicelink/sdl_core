/**
 * @name MFT.MediaPlayerView
 * 
 * @desc Media Player module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/common/MediaPalyerView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.appView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'app_view_container',
	
	classNameBindings:	[
							'MFT.States.app.active:active_state'
						],
	
	classNames: 'hidden',
	
	stateObj: MFT.States.media.app,
	
	/** View components*/
	childViews: [
		'controlls',
		'info',
		'helpmode_controlls'
	],
	
	info: Em.View.extend({
		elementId: 'app_view_info',
		
		template: Em.Handlebars.compile(
			'{{#with view}}'+
			'<div class="track-info">'+
				'<div class="device">Device name</div>'+
            	'<div class="divider_o"></div>'+
				'<div class="title">{{MFT.MediaController.currentSelectedPlayer.data.selectedItem.field1}}</div>'+
				'<div class="album">{{MFT.MediaController.currentSelectedPlayer.data.selectedItem.field2}}</div>'+
				'<div class="artist" >{{MFT.MediaController.currentSelectedPlayer.data.selectedItem.mediaTrack}}</div>'+
				'<div class="time">{{MFT.MediaController.currentSelectedPlayer.data.selectedItem.mediaClock}}</div>'+
//				'<div id="cd_logo" {{bindAttr class="MFT.CDModel.active:visible_display MFT.AppModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
//				'<div id="usb_logo" {{bindAttr class="MFT.USBModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
			'</div>'+
			'{{/with}}'
		)
	}),
	
	controlls: Em.ContainerView.extend({
		
		classNameBindings: ['MFT.helpMode:hidden'],
		
		elementId: 'app_view_controlls',
		/** View components*/
		childViews: [
			'PrevTrackButton',
			'PlayButton',
			'NextTrackButton'
		],
		
		classNames: 'player_controlls',
		
		PrevTrackButton: MFT.Button.extend({
			elementId: 			'app_view_controlls_prev_track_button',
			classNames:			['bc-item-big', 'prevcd'],
			classNames:			['bc-item-big', 'prevcd'],
			target:				'MFT.MediaController',
			action:				'prevTrack',
			icon:				'images/media/ico_prew.png',
		}),
		PlayButton: MFT.Button.extend({
			elementId: 			'app_view_controlls_play_button',
			classNames:			['bc-item-big', 'playcd'],
			target:				'MFT.MediaController',
			action:				'playTrack',
			/** Define button template */
			template: Ember.Handlebars.compile(
				'<img class="playIcon hideicon"{{bindAttr class="MFT.MediaController.currentSelectedPlayer.isPlaying:visible"}} src="images/media/ico_pause.png" />'+
				'<img class="playIcon showicon"{{bindAttr class="MFT.MediaController.currentSelectedPlayer.isPlaying:not-visible"}} src="images/media/ico-play.png" />'
			)
		}),
		NextTrackButton: MFT.Button.extend({
			elementId: 			'app_view_controlls_next_track_button',
			classNames:			['bc-item-big', 'nextcd'],
			target:				'MFT.MediaController',
			action:				'nextTrack',
			icon:				  'images/media/ico_next.png',
		})
	}),
	
	helpmode_controlls: Em.View.create({
		elementId: 'media_helpmode_cd_controllsblur',
		classNameBindings: ['MFT.helpMode:active_state'],
		classNames:['hidden']
	})
});