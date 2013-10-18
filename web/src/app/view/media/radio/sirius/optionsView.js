/**
 * @name MFT.SiriusOptionsView
 * 
 * @desc Media Sirius Options module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/radio/sirius/optionsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SiriusOptionsView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:			'media_sirius_options_view',
	
	classNames: ['sirius','media_options_view'],
	
	customInsertElementBinding: 'MFT.MediaView',
	
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
	
	topTitile: MFT.Label.extend({
		elementId:		'media_option_sirius_title_header',
		classNames:		  ['block-header-title','media_option_title_header'],
		contentBinding:	  Ember.Binding.oneWay('MFT.locale.label.view_media_sirius_options_title')
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_sirius_options_list',
			
			classNameBindings: ['FLAGS.locale','MFT.helpMode:disable-click','list'],
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			items: [
			{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_sirius_options_soundSettings',
						templateName:		  'arrow',
						action: 'turnOnSoundSettings',
						target: 'MFT.MediaController'
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_sirius_options_setCategory',
						buttonTexBindingt:	'MFT.locale.label.view_media_sirius_options_allChannels',
						templateName:		 'arrow',
						buttonAction: 		 'setPTYforSeekSirius',
						buttonTarget: 		 'MFT.MediaController'
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_sirius_options_parentalLockout',
						buttonTexBindingt:	'MFT.locale.label.view_media_sirius_options_changePIN',
						templateName:		  'arrow',
						buttonAction: 		 'changePIN',
						buttonTarget: 		 'MFT.MediaController'
					}							
				},
						
				{
								
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_media_sirius_options_alerts',
						tButtonValue:		 1,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_sirius_options_OnOff'
					}			
						
				},		
								
				{
					type:		MFT.Label,
	
					params:{
						contentBinding: 'MFT.locale.label.view_media_sirius_options_serialNumber',
					}							
				}
			]
 
	}),
	
	backButton: MFT.Button.extend({
		elementId: 			'media_options_sirius_back_btn',
		classNames:		  ['backButton','button','media_options_back_btn'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png'
	})
});