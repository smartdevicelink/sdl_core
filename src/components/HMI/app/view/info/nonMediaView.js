/**
 * @name MFT.InfoNonMedia
 * 
 * @desc NonMedia application visual representation
 * 
 * @category	View
 * @filesource	app/view/info/nonMedia.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.InfoNonMedia = Em.ContainerView.create( MFT.LoadableView, {
	
	elementId: 'info_nonMedia',
	
	controller: MFT.NonMediaController,
	
	childViews: [
		'content'
	],
	
	content: Em.ContainerView.extend({
		classNames: ['inner_content'],
		
		childViews: [
			'deviceName'
		],
		
		deviceName: MFT.Label.extend({
			contentBinding: 'controller.model.deviceName'
		})
	})
});