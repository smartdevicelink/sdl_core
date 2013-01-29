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
		classNameBindings: 'MFT.localization',
		elementId: 'media_popup',
		template:  Em.Handlebars.compile(
			'<div class="preset-stored">{{MFT.locale.label.view_media_popUp}}</div>'
		)
	})
});