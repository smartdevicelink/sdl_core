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
								  ],
									
			classNames:			['rs-item', 'hidden_display','top_rightmenu_button_blur'],
			
			
			isHDActive: function(){
				if(!MFT.MediaController.directTuneSelected && MFT.FmModel.active){
					return (MFT.MediaController.currentModuleData.get('selectedItem').isHd);
				}else{
					if(MFT.FmModel.active && MFT.MediaController.currentModuleData.get('selectedDirectItem'))
					return (MFT.MediaController.currentModuleData.get('selectedDirectItem').isHd);
				}	
			}.property('MFT.MediaController.currentModuleData.selectedItem', 'MFT.MediaController.directTuneSelected','MFT.FmModel.active'),
			
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_hdradio'),
			
			icon: 'images/media/passiv_horiz_led.png',
			
			onIconChange: function(){
				if(this.get('isHDActive')){
					this.set('icon', 'images/media/active_horiz_led.png');
				}else{
					this.set('icon', 'images/media/passiv_horiz_led.png');
				}
			}.observes('this.isHDActive'),
							
			disabled:			true,
			
			// rerender the view if the template name changes
			templateNameChanged: function() {
				
				if(MFT.helpMode){
					this.set('templateName','text');
					this.rerender();
				}else{
					this.set('templateName','');
					this.rerender();
				}
			}.observes('MFT.helpMode')
			
		}),
		repeatButton:  MFT.Button.extend({
			classNameBindings: 	[ 'MFT.CDModel.active:visible_display',
									'MFT.SDModel.active:visible_display',
									'MFT.USBModel.active:visible_display'
								  ],
			elementId: 'media_rightmenu_repeatButton',
			classNames:			['rs-item','hidden_display','top_rightmenu_button_blur'],
			icon:				  'images/media/passiv_horiz_led.png',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_repeat'),
			disabled:			  true,
			
			// rerender the view if the template name changes
			templateNameChanged: function() {
				
				if(MFT.helpMode){
					this.set('templateName','text');
					this.rerender();
				}else{
					this.set('templateName','');
					this.rerender();
				}
			}.observes('MFT.helpMode')
		}),
		shuffleButton:  MFT.Button.extend({
			classNameBindings: 	['MFT.CDModel.active:visible_display',
									'MFT.SDModel.active:visible_display',
									'MFT.USBModel.active:visible_display',,'MFT.helpMode:shuffleButton_help'],
			elementId: 'media_rightmenu_shuffleButton',
			classNames:			['rs-item','hidden_display'],
			icon:				  'images/media/passiv_horiz_led.png',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_shuffle'),
			target:			    'MFT.MediaController',
			action:			    'turnShuffleHelpVideoOn',
			onDown:			    false,
			disabled:			  true,
			helpModel: function(){
				(MFT.helpMode)? this.set('disabled',false): this.set('disabled',true);
			}.observes('MFT.helpMode')
		}),
		
		replayButton:  MFT.Button.extend({
			classNameBindings: 	['MFT.SiriusModel.active:visible_display','MFT.helpMode:replay_button_help'],
			elementId:			'media_rightmenu_replayButton',
			classNames:		   ['rs-item','hidden_display'],
			icon:				 'images/media/passiv_horiz_led.png',
			textBinding:		  Ember.Binding.oneWay('MFT.locale.label.view_media_replay'),
			target:			   'MFT.MediaController',
			action:			   'turnReplayHelpVideoOn',
			disabled:			 true,
			onDown: 			   false,
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
				if(MFT.USBModel.active || MFT.SDModel.active ){
					this.set('action','turnSimilarHelpVideoOn')
					return MFT.locale.label.view_media_similarMusic
				}else{
					this.set('action','turnScanHelpVideoOn')
					return MFT.locale.label.view_media_scan;
				}
			}.property('MFT.MediaController.currentModuleData.selectedItem','MFT.locale.label'),
			
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
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_moreInfo'),
		}),
		
		optionsButton: MFT.Button.extend({
			elementId:	'media_rightmenu_optionButton',
			classNameBindings: ['MFT.helpMode:option_blur'],
			classNames:			['rs-item'],
			icon:				'images/media/active_arrow.png',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_options'),
			action:			  'turnOnOptions',
			target:    'MFT.MediaController',
			disabledBinding:  'MFT.helpMode',
			
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
			
			templateName:	'text',
			
			elementId:	'media_rightmenu_directButton',
			classNames:			['rs-item', 'hidden_display'],
			target:				'MFT.MediaController',
			action:				'onDirectTune',
			onDown: false,
			
			text: function(){	
					if(!MFT.MediaController.directTuneHide){
						this.set('action','offDirectTune');
						return MFT.locale.label.view_media_presets
					}else{
						this.set('action','onDirectTune');
						return MFT.locale.label.view_media_directTune
					}
				}.property('MFT.MediaController.directTuneHide','MFT.locale.label')
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
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_browse'),
			onDown: false,
		})		
});