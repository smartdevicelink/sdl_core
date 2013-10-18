/**
 * @name MFT.AMView
 * 
 * @desc Media AM INFO module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/radio/AMView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.AMView = Em.ContainerView.create(MFT.LoadableView,{
	
	/** View Id */
	elementId: 'am_view',
	
	childViews: ['amInfo', 'rightmenu'],
	
	/** Class Name*/
	classNames: ['hidden','am-info'],
	
	amInfo: Em.View.create({
		/** View Id */
		elementId: 'am_info_view',
		
		/** Media Controller binding*/
		controlerBinding: 'MFT.MediaController',
		/** Define Template*/	
		template:  Em.Handlebars.compile(
			'<div class="divider_o"></div>'+	
			'<div id="band">'+
				'{{#if view.controler.directKeypressed}}'+
					'{{view.controler.directTuneToString}}'+
				'{{else}}'+
					'{{view.controler.currentActiveData.frequency}}'+
				'{{/if}}'+
			'</div>'		
		)
	}),
	
	rightmenu: Em.ContainerView.create({
		/** View Id */
		elementId: 'am_rightmenu_view',
		/** Class Names */
		classNames: ['right-stock'],

        classNameBindings: 	[
            'MFT.States.media.radio.am.options.active:hide-important'
        ],
		
		/** View Components*/
		childViews: [
			'hdButton',
			'scanButton',
			'optionsButton',
			'directTuneButton'
		],
		
		hdButton:  MFT.Button.extend({
			elementId: 'media_am_hdButton',
			classNameBindings: [	
				'MFT.MediaController.isHDButtonActiveInverted:replay_button_help',
				'MFT.helpMode: hide_icon'
			],			
			classNames:			['rs-item'],
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_hdradio'),
			onDown: false,
			icon: 'images/media/passiv_horiz_led.png',
			disabled: true
		}),
		
		scanButton:  MFT.Button.extend({
			elementId:		'media_am_scanButton',
			classNameBindings: ['MFT.helpMode:scan_button_help'],
			classNames:		['rs-item','helpmode_box_shadow'],
			icon:			  'images/media/passiv_horiz_led.png',
			target:			'MFT.MediaController',
			action:			'turnScanHelpVideoOn',
			onDown:  			false,
			textBinding:       Ember.Binding.oneWay('MFT.locale.label.view_media_scan'),
			disabledBinding:  'MFT.reversHelpModeBoolean',
			
		}),
		
		optionsButton: MFT.Button.extend({
			elementId:	'media_am_optionButton',
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
			elementId:	'media_am_directButton',
			classNameBindings: ['MFT.helpMode:direct_button_help'],
			templateName:	'text',
			classNames:			['rs-item','helpmode_box_shadow'],
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
		})
	})
});