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
MFT.MediaView = Em.ContainerView.create({
	/** View Id */
	elementId:	'media',
 	
 	classNames:	'block hidden',
	
	/** Bind class for visual representation */
	classNameBindings: ['MFT.States.media.active:active_state'],
	
	/** Media components */		
	childViews: [
		'leftMenu',
		'presets',
		'rightMenu',
		'SavePresetPopup',
		'directTune'
	],
	/** Left Menu view component */
	leftMenu: MFT.LeftMenuView,
	
	rightMenu: MFT.RightMenuView,
	
	presets: MFT.PresetsView,
	
	directTune: MFT.DirectTune,
	
	/** Save Preset Popup*/
	SavePresetPopup: Em.View.extend({
		elementId: 'media_popup',
		template:  Em.Handlebars.compile(
			'<div class="preset-stored">Preset saved</div>'
		)
	})
});