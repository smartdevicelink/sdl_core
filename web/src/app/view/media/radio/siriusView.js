/**
 * @name MFT.AMFMInfoView
 * 
 * @desc Media AM/FM INFO module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/AMFMInfoView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.SiriusView = Em.ContainerView.create(MFT.LoadableView,{
	
		elementId: 'sirius_view',
		
		/** Class Name*/
		classNames: ['hidden','sirius-info'],
		
		childViews: ['siriusInfo', 'rightmenu'],
		
		
		siriusInfo:  Em.View.create({
		
			/** controler binding*/
			controlerBinding: 'MFT.MediaController',
			
			/** Define Template*/	
			template:  Em.Handlebars.compile(
					'<div class="siriusband" {{bindAttr class="view.controler.isStationLogo:hidden_display"}}>'+
						'{{view.controler.currentActiveData.frequency}}'+
					'</div>'+
					'<div>'+
						'<div class="channel" {{bindAttr class="view.controler.directKeypressed:hidden_display"}}>{{view.controler.currentActiveData.channel}}</div>'+
						'<div class="channel hidden_display"{{bindAttr class="view.controler.directKeypressed:visible_display"}}>'+
							'Channel {{view.controler.directTuneToString}}'+
						'</div>'+
						
						
						'<div class="siriusgenre">{{view.controler.currentActiveData.genre}}</div>'+
						'<div class="titanium" {{bindAttr class="view.controler.isStationLogo:visible_display"}} {{bindAttr src="MFT.SiriusModel.sir1.selectedItem.logo"}}></div>'+
						'<div class="siriustitle "}}>{{view.controler.currentActiveData.title}}</div>'+
						'<div class="siriusartist "}}> {{view.controler.currentActiveData.artist}}</div>'+
						'{{view MFT.Button '+
							'target="MFT.MediaController"'+
							'action="turnAlertHelpVideoOn" '+
							'classNameBindings="MFT.helpMode:help" '+
							'classNames="button btn_alert helpmode_box_shadow" '+
							'onDown=false '+
							'textBinding="MFT.locale.label.view_media_sirius_alert" '+
							/* Binnding to helpMode invert function */
							'disabledBinding="MFT.reversHelpModeBoolean" '+							
						'}}'+
					'</div>'	
			)
		}),
		
		rightmenu: Em.ContainerView.create({
			/** View ID */
			elementId:	'sirius_rightmenu_view',
			
			/** Class Names */
			classNames: ['right-stock'],
			/** View Components*/
			childViews: [
				'replayButton',
				'scanButton',
				'optionsButton',
				'directTuneButton',
				'browseButton'
			],
			
			replayButton:  MFT.Button.extend({
				classNameBindings: 	['MFT.helpMode:replay_button_help'],
				elementId:			'media_sirius_rightmenu_replayButton',
				classNames:         ['rs-item', 'helpmode_box_shadow'],
				icon:				 'images/media/passiv_horiz_led.png',
				textBinding:		  Ember.Binding.oneWay('MFT.locale.label.view_media_replay'),
				target:			   'MFT.MediaController',
				action:			   'turnReplayHelpVideoOn',
				disabled:			 true,
				onDown: 			   false,
				disabledBinding: 'MFT.reversHelpModeBoolean'
			}),
			
			scanButton:  MFT.Button.extend({
				elementId:		'media_sirius_rightmenu_scanButton',
				classNameBindings: ['MFT.helpMode:scan_button_help'],
				classNames:		['rs-item','helpmode_box_shadow'],
				icon:			 'images/media/passiv_horiz_led.png',
				target:			'MFT.MediaController',
				action:			'turnScanHelpVideoOn',
				disabled:		true,
				onDown: false,
				
				textBinding: 'MFT.locale.label.view_media_scan', 
				
				disabledBinding: 'MFT.reversHelpModeBoolean'
				
			}),
			
			optionsButton: MFT.Button.extend({
				elementId:	'media_sirius_rightmenu_optionButton',
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
			
			directTuneButton: MFT.Button.extend({
				classNameBindings: ['MFT.helpMode:direct_button_help'],
				templateName:	'text',
				elementId:	'media_sirius_rightmenu_directButton',
				classNames:			['rs-item', 'helpmode_box_shadow'],
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
				classNameBindings: 	['MFT.helpMode:browse_button_help'],
				elementId:	'media_sirius_rightmenu_browseButton',
				action:			   'turnOnBrowse',
				target:			   'MFT.MediaController',
				classNames:			['rs-item','helpmode_box_shadow'],
				icon:				'images/media/active_arrow.png',
				textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_browse'),
				onDown: false
			})		
	})

});