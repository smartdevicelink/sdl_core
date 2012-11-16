/**
 * @name MFT.optionsView
 * 
 * @desc Media Am Options module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/optionsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.optionsView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:			'media_options_view',
	
	classNameBindings:	[
		'MFT.MediaController.currentOptionsData.view',
		'MFT.localization',
		'MFT.helpMode:helpmode_blur_text_light'
	],
		
	
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list',
							  'helpbgblur'
						  ],

	backButton: MFT.Button.extend({
		elementId: 			'media_options_back_btn',
		classNames:		   ['backButton','button'],		
		action:			   'optionsBack',
		target:			   'MFT.MediaController',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		'media_option_title_header',
		classNames:		  ['block-header-title'],
		contentBinding:	  Ember.Binding.oneWay('MFT.MediaController.currentOptionsData.title')
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_options_list',
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			itemsBinding: Ember.Binding.oneWay('MFT.MediaController.currentOptionsData.items')
 
	}),
	
	helpbgblur: Em.View.create({
		elementId: 'media_options_view_blur',
	})
});