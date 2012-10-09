/**
 * @name MFT.MediaAmOptionsView
 * 
 * @desc Media Am Options module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/options/AmOptionsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.MediaOptionsView = Em.ContainerView.create({
	classNameBindings:	[ 'MFT.States.media.Am.options.active:active_state',
							'MFT.States.media.Fm.options.active:active_state',
							'MFT.States.media.Sirius.options.active:active_state',
							'MFT.States.media.cd.options.active:active_state',
							'MFT.States.media.usb.options.active:active_state',
							'MFT.States.media.bt.options.active:active_state',
							'MFT.MediaController.currentOptionsData.view',
							'MFT.helpMode'
						  ],
	
	classNames:		   ['hidden'],
	elementId:			'media_options_view',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],

	backButton: MFT.Button.extend({
		classNames:		   ['backButton','button'],		
		action:			   'back',
		target:			   'MFT.States',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	  Ember.Binding.oneWay('MFT.MediaController.currentOptionsData.title')
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_options_list',
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			itemsBinding: Ember.Binding.oneWay('MFT.MediaController.currentOptionsData.items')
 
	})
});