/**
 * @name MFT.FMOptionsView
 * 
 * @desc Media FM Options module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/radio/fm/optionsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.FMOptionsView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:			'media_fm_options_view',
	
	classNames: ['fm','media_options_view'],
	
	customInsertElementBinding: 'MFT.MediaView',
	
	childViews:		   [
							  'backButton',
							  'refreshPresetPopup',
							  'topTitile',
							  'list'
						  ],
	
	topTitile: MFT.Label.extend({
		elementId:		'media_option_fm_title_header',
		classNames:		  ['block-header-title','media_option_title_header'],
		contentBinding:	  Ember.Binding.oneWay('MFT.locale.label.view_media_amFm_options_title')
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_fm_options_list',
			
			classNameBindings: ['FLAGS.locale','MFT.helpMode:disable-click','list'],
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			items:[
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_soundSettings',
						templateName:		'arrow',
						action: 'turnOnSoundSettings',
						target: 'MFT.MediaController'
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_setPTY',
						buttonTexBindingt:	'MFT.locale.label.view_media_amFm_options_noProgram',
						templateName:		 'arrow',
						buttonDisabledBinding: 'MFT.MediaController.PTYButtonIsDisabled',
						buttonAction:         'setPTYforSeekFM',
						buttonTarget:		 'MFT.MediaController'							
					}							
				},
	
				{								
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_RDS',
						tButtonValueBinding:  'MFT.MediaController.fmRDSTextOnOff',
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_amFm_options_onOff'
					}
				},	
	
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_autoset',
						buttonTexBindingt:	'MFT.locale.label.view_media_amFm_options_refresh',
						buttonAction: 'refreshPresetsFm',
						buttonTarget: 'MFT.MediaController'
					}							
				},
				
				{								
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_TAG',
                        tButtonValueBinding:  'MFT.MediaController.tagStoreOnOff',
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_amFm_options_onOff',
                        labelDisabled: 		!MFT.SettingsModel.isEnglish,
                        tButtonDisabled:	  !MFT.SettingsModel.isEnglish
					}									
				}
	
			]
 
	}),
	
	backButton: MFT.Button.extend({
		elementId: 			'media_options_fm_back_btn',
		classNames:		  ['backButton','button','media_options_back_btn'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
				  
	refreshPresetPopup: Em.View.extend({
		classNames: 'optionPopup',
		elementId: 'fm_refreshPresetPopup',
		messageBinding: Ember.Binding.oneWay('MFT.MediaController.popUpMessage'),
		template:  Em.Handlebars.compile(
			'{{view.message}}'
		)
	})
});