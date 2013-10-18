/**
 * @name MFT.MediaView
 * 
 * @desc Media module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/MediaView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.MediaView = Em.ContainerView.create(MFT.LoadableView,{
	/** View Id */
	elementId:	'media',
 		
	/** Media components */		
	childViews: [
		'leftMenu',
		'presets',
		'directTune',
		'optionsMessage',
		
	],
	/** Left Menu view component */
	leftMenu: MFT.LeftMenuView,
	
	//rightMenu: MFT.RightMenuView,
	
	presets: MFT.PresetsView,
	
	directTune: MFT.DirectTune,
	
	optionsMessage: MFT.Label.extend({
		elementId:		'media_option_bottom_message',
		classNames:       'bottom_message',
		contentBinding:	  Ember.Binding.oneWay('MFT.MediaController.updatingMessage')
	}),
});