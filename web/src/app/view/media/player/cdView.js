/**
 * @name MFT.CDView
 * 
 * @desc Media CD module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/palyer/cdView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.CDView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'media_player_cd_view',
	
	classNames: 'media_player_view_wrapper',
		
	/** View components*/
	childViews: [
			'controlls',
			'info',
			'rightmenu'
	],
	
	info: Em.View.extend({
		elementId: 'media_player_cd_view_info',
		
		template: Em.Handlebars.compile(
			'<div class="track-info">'+
				'<div class="total">{{MFT.MediaController.currentSelectedPlayer.currentTrack}}/{{MFT.MediaController.currentSelectedPlayer.totalTracks}}</div>'+
            	'<div class="divider_o"></div>'+
				'<div class="title">{{MFT.MediaController.currentSelectedPlayer.data.selectedItem.album}}</div>'+
				'<div class="track-number" >{{MFT.MediaController.currentSelectedPlayer.data.selectedItem.title}}</div>'+
				'<div class="time">{{MFT.MediaController.currentSelectedPlayer.formateTimeToString}}</div>'+
				'<div id="cd_logo"></div>'+
			'</div>'
		)
	}),
	
	controlls: Em.ContainerView.extend({
		
		elementId: 'media_player_cd_view_controlls',
		/** View components*/
		childViews: [
				'PrevTrackButton',
				'PlayButton',
				'NextTrackButton'
		],
		
		classNames: 'player_controlls',
		
		PrevTrackButton: MFT.Button.extend({
			elementId: 			'media_player_cd_view_controlls_prev_track_button',
			classNames:			['bc-item-big', 'prevcd'],
			target:				'MFT.MediaController',
			action:				'prevTrack',
			icon:				  'images/media/ico_prew.png'
		}),
		PlayButton: MFT.Button.extend({
			elementId: 			'media_player_cd_view_controlls_play_button',
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
			elementId: 			'media_player_cd_view_controlls_next_track_button',
			classNames:			['bc-item-big', 'nextcd'],
			target:				'MFT.MediaController',
			action:				'nextTrack',
			icon:				  'images/media/ico_next.png'
		})
	}),
	
	rightmenu:  Em.ContainerView.create({
		/** View ID */
		elementId:	'media_cd_rightmenu',
		
		/** Class Names */
		classNames: ['right-stock'],

        classNameBindings: [
            'MFT.States.media.player.cd.options.active:hidden',
            'MFT.States.media.player.cd.browse.active:hidden',
            'MFT.States.media.player.cd.moreinfo.active:hidden'
        ],
		/** View Components*/
		childViews: [
			'repeatButton',
			'shuffleButton',
			'scanButton',
			'moreInfoButton',
			'optionsButton',
			'browseButton'
		],
		
		repeatButton:  MFT.Button.extend({
			elementId: 'media_cd_rightmenu_repeatButton',
			classNames:			['rs-item'],
			classNameBindings: ['MFT.helpMode: hide_icon'],
			icon:				  'images/media/passiv_horiz_led.png',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_repeat'),
			disabled:			  true
		}),
		
		shuffleButton:  MFT.Button.extend({
			classNameBindings: 	['MFT.helpMode:shuffleButton_help'],
			elementId: 'media_cd_rightmenu_shuffleButton',
			classNames:			['rs-item', 'helpmode_box_shadow'],
			icon:				  'images/media/passiv_horiz_led.png',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_shuffle'),
			target:			    'MFT.MediaController',
			action:			    'turnShuffleHelpVideoOn',
			onDown:			    false,
			disabledBinding: 'MFT.reversHelpModeBoolean'
		}),
		
		scanButton:  MFT.Button.extend({
			elementId:		'media_cd_rightmenu_scanButton',
			classNameBindings: ['MFT.helpMode:scan_button_help'],
			classNames:		['rs-item', 'helpmode_box_shadow'],
			icon:			 'images/media/passiv_horiz_led.png',
			target:			'MFT.MediaController',
			action:			'turnScanHelpVideoOn',
			onDown: false,
			
			textBinding: 'MFT.locale.label.view_media_scan', 
			
			disabledBinding: 'MFT.reversHelpModeBoolean'
			
		}),
		moreInfoButton: MFT.Button.extend({
			classNameBindings: 	['MFT.helpMode:moreinfoButton_help'],
			elementId: 'media_cd_rightmenu_moreinfoButton',
			action:			    'turnMoreInfo',
			target:				'MFT.MediaController',
			classNames:			['rs-item', 'helpmode_box_shadow'],
			icon:				'images/media/active_arrow.png',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_moreInfo'),
			onDown: false
		}),
		
		optionsButton: MFT.Button.extend({
			elementId:	'media_cd_rightmenu_optionButton',
			classNames:			['rs-item'],
			icon:				'images/media/active_arrow.png',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_options'),
			action:			  'turnOnOptions',
			target:    'MFT.MediaController',
			disabledBinding:  'MFT.helpMode',
			
			// Define button template
			template: Ember.Handlebars.compile(
				'{{#with view}}'+
                    '<img class="ico" {{bindAttr src="icon"}} />'+
                    '<span>{{text}}</span>'+
				'{{/with}}'
			)
		}),
		
		browseButton: MFT.Button.extend({
			classNameBindings: 	['MFT.helpMode:browse_button_help'],
			elementId:	'media_cd_rightmenu_browseButton',
			action:			   'turnOnBrowse',
			target:			   'MFT.MediaController',
			classNames:			['rs-item', 'helpmode_box_shadow'],
			icon:				'images/media/active_arrow.png',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_browse'),
			onDown: false
		})		
})
});