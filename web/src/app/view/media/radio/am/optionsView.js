/**
 * @name MFT.AMOptionsView
 * 
 * @desc Media Am Options module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/radio/am/optionsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.AMOptionsView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:			'media_am_options_view',
	
	classNames: ['am','media_options_view'],
	
	customInsertElementBinding: 'MFT.MediaView',
	
	childViews:		   [
							  'backButton',
							  'refreshPresetPopup',
							  'topTitile',
							  'list'
						  ],
	
	topTitile: MFT.Label.extend({
		elementId:		'media_option_am_title_header',
		classNames:		  ['block-header-title','media_option_title_header'],
		contentBinding:	  Ember.Binding.oneWay('MFT.locale.label.view_media_amFm_options_title')
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_options_list',
			
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
						buttonDisabled: 	   true						
					}							
				},
	
				{
								
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_RDS',
						tButtonValue:		 null,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_amFm_options_onOff',
						tButtonDisabled:	  true,
					}			
						
				},	
	
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_autoset',
						buttonTexBindingt:	'MFT.locale.label.view_media_amFm_options_refresh',
						buttonAction: 'refreshPresetsAm',
						buttonTarget: 'MFT.MediaController'
					}							
				},
				
								{
								
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_TAG',
						tButtonValue:		 null,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_amFm_options_onOff',
						tButtonDisabled:	  true,
                        labelDisabled:	  true,
					}
						
				}
	
			]
 
	}),
	
	backButton: MFT.Button.extend({
		elementId: 			'media_options_am_back_btn',
		classNames:		  ['backButton','button','media_options_back_btn'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
				  
	refreshPresetPopup: Em.View.extend({
		classNames: 'optionPopup',
		elementId: 'am_refreshPresetPopup',
		messageBinding: Ember.Binding.oneWay('MFT.MediaController.popUpMessage'),
		template:  Em.Handlebars.compile(
			'{{view.message}}'
		)
	})
});