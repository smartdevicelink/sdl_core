/**
 * @name MFT.RightMenuView
 * 
 * @desc Media  Right Menu visual representation
 * 
 * @category	View
 * @filesource	app/view/media/MFT.RightMenuView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.RightMenuView = Em.ContainerView.create({
		/** View ID */
		elementId:	'media_rightmenu',
		/** Class Names bindings for visual representation*/
		classNameBindings: 	[
			'MFT.CDModel.active:visible_display' ,
			'MFT.SDModel.active:visible_display',
			'MFT.USBModel.active:visible_display',
			'MFT.SiriusModel.active:visible_display',
			'MFT.FmModel.active:visible_display',
			'MFT.AmModel.active:visible_display',
			'MFT.helpMode:help_mode'
		],
		/** Class Names */
		classNames: ['right-stock', 'hidden_display'],
		/** View Components*/
		childViews: [
			'hdButton',
			'repeatButton',
			'shuffleButton',
			'replayButton',
			'scanButton',
			'moreInfoButton',
			'optionsButton',
			'directTuneButton',
			'browseButton',
			'help_yellow_borders',
			'help_yellow_dividers'
		],
		
		/**  Help Mode Yellow Borders*/
		help_yellow_borders:Em.View.extend({
			elementId: 'yellow_borders',
			classNameBindings: 'MFT.helpMode:active_state',
			classNames: 'hidden',
			/** Define button template */
				template: Ember.Handlebars.compile(
					'{{#with view}}'+
						'<div id="media_rightmenu_yellow_amfm_borders" {{bindAttr class="MFT.AmModel.active:visible_display MFT.FmModel.active:visible_display"}}>'+
							'<div id="amfmscan"></div>'+
							'<div id="amfmdirect"></div>'+
						'</div>'+
						'<div id="media_rightmenu_yellow_sir_borders" {{bindAttr class="MFT.SiriusModel.active:visible_display"}}>'+
							'<div id="uplist_borders"></div>'+
							'<div id="bottomlist_borders"></div>'+
						'</div>'+
						'<div id="media_rightmenu_yellow_cd_borders" {{bindAttr class="MFT.CDModel.active:visible_display MFT.SDModel.active:visible_display MFT.USBModel.active:visible_display"}}>'+
							'<div id="triple_borders"></div>'+
							'<div id="single_border"></div>'+
						'</div>'+
					'{{/with}}'
				)
		}),
		/**  Help Mode Yellow Dividers*/
		help_yellow_dividers: Em.View.extend({
				elementId: 'yellow_dividers',
				classNameBindings: 'MFT.helpMode:active_state',
				classNames: 'hidden',
				template: Ember.Handlebars.compile(
					'{{#with view}}'+
						'<div id="media_rightmenu_yellow_sir_dividers" {{bindAttr class="MFT.SiriusModel.active:visible_display"}}>'+
							'<div class="yellow_divider top"></div>'+
							'<div class="yellow_divider bottom"></div>'+
						'</div>'+
						'<div id="media_rightmenu_yellow_cd_dividers" {{bindAttr class="MFT.States.media.player.active:visible_display"}}>'+
							'<div class="yellow_divider cdtop"></div>'+
							'<div class="yellow_divider cdbottom"></div>'+
						'</div>'+
					'{{/with}}'
				)
		}),
		
		hdButton:  MFT.Button.extend({
			elementId: 'media_am-fm_hdButton',
			classNameBindings: 	[
									'isHDActive:isHd',
									'MFT.FmModel.active:visible_display' , 
									'MFT.AmModel.active:visible_display', 
									'MFT.helpMode:hd_blur helpmode_blur_cont',
									'isHDhelpModeActive:hd_active_blur'],
									
			classNames:			['rs-item','hdButton', 'hidden_display'],
			
			isHDhelpModeActive: function() {
				return (MFT.MediaController.currentModuleData.get('selectedItem').isHd  && MFT.helpMode);
			}.property('MFT.MediaController.currentModuleData.selectedItem.isHd','MFT.helpMode'),
			
			isHDActive: function(){
				if(!MFT.MediaController.directTuneSelected){
					return (MFT.MediaController.currentModuleData.get('selectedItem').isHd);
				}else{
					return (MFT.MediaController.currentModuleData.get('selectedDirectItem').isHd);
				}	
			}.property('MFT.MediaController.currentModuleData.selectedItem', 'MFT.MediaController.directTuneSelected'),
			
			text:				'HD Radio',
			
			icon:				'images/media/passiv_horiz_led.png',				
			
			disabled:			true,
			/** Define button template */
			template: Ember.Handlebars.compile(
				'{{#with view}}'+
						'<div class="ico"></div>'+
						'<span>{{text}}</span>'+
				'{{/with}}'
			)
		}),
		repeatButton:  MFT.Button.extend({
			classNameBindings: 	['MFT.CDModel.active:visible_display',
									'MFT.SDModel.active:visible_display',
									'MFT.USBModel.active:visible_display',,'MFT.helpMode:repeatButton_blur'],
			elementId: 'media_rightmenu_repeatButton',
			classNames:			['rs-item','hidden_display'],
			icon:				'images/media/passiv_horiz_led.png',
			text:				'Repeat',
			disabled:		true,
			/** Define button template */
			template: Ember.Handlebars.compile(
				'{{#with view}}'+
					'{{#unless MFT.helpMode}}'+
						'<img class="ico" {{bindAttr src="icon"}} />'+
						'<span>{{text}}</span>'+
					'{{/unless}}'+
				'{{/with}}'
			)
		}),
		shuffleButton:  MFT.Button.extend({
			classNameBindings: 	['MFT.CDModel.active:visible_display',
									'MFT.SDModel.active:visible_display',
									'MFT.USBModel.active:visible_display',,'MFT.helpMode:shuffleButton_help'],
			elementId: 'media_rightmenu_shuffleButton',
			classNames:			['rs-item','hidden_display'],
			icon:				'images/media/passiv_horiz_led.png',
			text:				'Shuffle',
			target:			'MFT.MediaController',
			action:			'turnShuffleHelpVideoOn',
			onDown: false,
			disabled:		true,
			helpModel: function(){
				(MFT.helpMode)? this.set('disabled',false): this.set('disabled',true);
			}.observes('MFT.helpMode')
		}),
		
		replayButton:  MFT.Button.extend({
			classNameBindings: 	['MFT.SiriusModel.active:visible_display','MFT.helpMode:replay_button_help'],
			elementId:	'media_rightmenu_replayButton',
			classNames:			['rs-item','hidden_display'],
			icon:				'images/media/passiv_horiz_led.png',
			text:				'Replay',
			target:			'MFT.MediaController',
			action:			'turnReplayHelpVideoOn',
			disabled:		true,
			onDown: false,
			helpModel: function(){
				(MFT.helpMode)? this.set('disabled',false): this.set('disabled',true);
			}.observes('MFT.helpMode')
		}),
		
		scanButton:  MFT.Button.extend({
			elementId:		'media_rightmenu_scanButton',
			classNameBindings: ['MFT.helpMode:scan_button_help'],
			classNames:		['rs-item'],
			icon:			 'images/media/passiv_horiz_led.png',
			target:			'MFT.MediaController',
			action:			'turnScanHelpVideoOn',
			disabled:		true,
			onDown: false,
			
			text: function(){
				if(MFT.States){
					if(MFT.USBModel.active){
						this.set('action','turnSimilarHelpVideoOn')
						return 'Similar Music'
					}else{
						this.set('action','turnScanHelpVideoOn')
						return 'Scan'
					}
				}
			}.property('MFT.MediaController.currentModuleData.selectedItem'),
			
			helpModel: function(){
				(MFT.helpMode)? this.set('disabled',false): this.set('disabled',true);
			}.observes('MFT.helpMode')
			
		}),
		moreInfoButton: MFT.Button.extend({
			classNameBindings: 	['MFT.CDModel.active:visible_display',
									'MFT.SDModel.active:visible_display',
									'MFT.USBModel.active:visible_display',
									'MFT.helpMode:scan_help','MFT.helpMode:moreinfoButton_help'],
			elementId: 'media_rightmenu_moreinfoButton',
			action:			    'turnMoreInfo',
			target:				'MFT.MediaController',
			classNames:			['rs-item','hidden_display'],
			icon:				'images/media/active_arrow.png',
			text:				'More Info'
		}),
		
		optionsButton: MFT.Button.extend({
			elementId:	'media_rightmenu_optionButton',
			classNameBindings: ['MFT.helpMode:option_blur'],
			classNames:			['rs-item'],
			icon:				'images/media/active_arrow.png',
			text:				'Options',
			action:			  'turnOnOptions',
			target:    'MFT.MediaController',
			
			disabled: function(){
				return ( MFT.SDModel.active || MFT.helpMode )
			}.property('MFT.SDModel.active','MFT.helpMode'),
			
			// Define button template
			template: Ember.Handlebars.compile(
				'{{#with view}}'+
					'{{#unless MFT.helpMode}}'+
						'<img class="ico" {{bindAttr src="icon"}} />'+
						'<span>{{text}}</span>'+
					'{{/unless}}'+
				'{{/with}}'
			)
		}),
		
		directTuneButton: MFT.Button.extend({
			classNameBindings: 	[
				'MFT.SiriusModel.active:visible_display',
				'MFT.AmModel.active:visible_display', 
				'MFT.FmModel.active:visible_display',
				'MFT.helpMode:direct_button_help'
			],
						
			elementId:	'media_rightmenu_directButton',
			classNames:			['rs-item', 'hidden_display'],
			target:				'MFT.MediaController',
			action:				'onDirectTune',
			onDown: false,
			
			text: function(){	
					if(!MFT.MediaController.directTuneHide){
						this.set('action','offDirectTune');
						return 'Presets'
					}else{
						this.set('action','onDirectTune');
						return 'Direct Tune'
					}
				}.property('MFT.MediaController.directTuneHide')
		}),
		
		
		browseButton: MFT.Button.extend({
			classNameBindings: 	['MFT.SiriusModel.active:visible_display', 'MFT.CDModel.active:visible_display',
									'MFT.SDModel.active:visible_display',
									'MFT.USBModel.active:visible_display',,'MFT.helpMode:browse_button_help'],
			elementId:	'media_rightmenu_browseButton',
			action:			   'turnBrowse',
			target:			   'MFT.MediaController',
			classNames:			['rs-item','hidden_display'],
			icon:				'images/media/active_arrow.png',
			text:				'Browse',
			onDown: false,
		})		
});