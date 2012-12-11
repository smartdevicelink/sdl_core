/**
 * @name MFT.InfoNonMediaOptions
 * 
 * @desc NonMedia application visual representation
 * 
 * @category	View
 * @filesource	app/view/info/nonMedia/InfoNonMediaOptions.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.InfoNonMediaOptions = Em.ContainerView.create( MFT.LoadableView, {
	
	elementId: 'info_nonMedia_options',
	
	childViews: [
		'backButton',
		'title',
		'commands'
	],
	
	backButton: MFT.Button.extend({		
		classNames:	'backButton',
		
		icon:		'images/common/ico_back.png',
		
		target:		'MFT.States',
		action:		'back'
	}),
	
	title: MFT.Label.extend({
		classNames:	'options_title',
		
		content:	'Options'
	}),
	
	commands: MFT.ApplinkCommandsList.extend({
		
		elementId:		'info_nonMedia_options_list',
				
		itemsOnPage:    5,
		
		items: [
			{
				type:	MFT.Button,
			
				params: {
					templateName:	'text',
					text:			'Exit',
					action:			'back',
					target:			'MFT.States'
				}
			},
			{
				type:	MFT.Button,
			
				params: {
					templateName:	'arrow',
					text:			'Device Information',
				}
			}
		]
	})
});