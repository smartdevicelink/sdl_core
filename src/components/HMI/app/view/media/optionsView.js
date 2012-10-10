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
	classNameBindings:	[ 'MFT.States.media.options.active:active_state',
							'MFT.MediaController.currentOptionsData.view',
							'MFT.helpMode'
						  ],
	
	classNames:		   ['hidden'],
	
	stateObj: MFT.States.media.options,
	
	elementId:			'media_options_view',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],

	backButton: MFT.Button.extend({
		classNames:		   ['backButton','button'],		
		action:			   'optionsBack',
		target:			   'MFT.MediaController',	
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