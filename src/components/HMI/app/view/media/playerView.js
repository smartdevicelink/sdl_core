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
MFT.playerView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'media_player_view_container',
	
	classNameBindings:['MFT.helpMode:helpmode_blur_text'],
		
	/** View components*/
	childViews: [
			'controlls',
			'info',
			'helpmode_controlls'
	],
	
	info: Em.View.extend({
		elementId: 'media_player_view_info',
		
		template: Em.Handlebars.compile(
			'<div class="track-info">'+
				'<div class="total">{{MFT.MediaController.currentSelectedPlayer.currentTrack}}/{{MFT.MediaController.currentSelectedPlayer.totalTracks}}</div>'+
            	'<div class="divider_o"></div>'+
				'<div class="title">{{MFT.MediaController.currentSelectedPlayer.data.selectedItem.album}}</div>'+
				'<div class="track-number" >{{MFT.MediaController.currentSelectedPlayer.data.selectedItem.title}}</div>'+
				'<div class="time">{{MFT.MediaController.currentSelectedPlayer.formateTimeToString}}</div>'+
				'<div id="cd_logo" {{bindAttr class="MFT.CDModel.active:visible_display MFT.SDModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
				'<div id="usb_logo" {{bindAttr class="MFT.USBModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
			'</div>'
		)
	}),
	
	controlls: Em.ContainerView.extend({
		
		classNameBindings: ['MFT.helpMode:hidden'],
		
		elementId: 'media_player_view_controlls',
		/** View components*/
		childViews: [
				'PrevTrackButton',
				'PlayButton',
				'NextTrackButton'
		],
		
		classNames: 'player_controlls',
		
		PrevTrackButton: MFT.Button.extend({
			elementId: 			'media_player_view_controlls_prev_track_button',
			classNames:			['bc-item-big', 'prevcd'],
			classNames:			['bc-item-big', 'prevcd'],
			target:				'MFT.MediaController',
			action:				'prevTrack',
			icon:				  'images/media/ico_prew.png',
		}),
		PlayButton: MFT.Button.extend({
			elementId: 			'media_player_view_controlls_play_button',
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
			elementId: 			'media_player_view_controlls_next_track_button',
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