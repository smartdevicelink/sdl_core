/**
 * @name MFT.InfoNonMediaOptionsSubMenu
 * 
 * @desc NonMedia application visual representation
 * 
 * @category	View
 * @filesource	app/view/info/nonMedia/options/subMenuView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.InfoNonMediaOptionsSubMenu = Em.ContainerView.create( MFT.LoadableView, {
	
	elementId: 'info_nonMedia_options_submenu',
	
	childViews: [
		'backButton',
		'title',
		'list'
	],
	
	backButton: MFT.Button.extend({		
		classNames:	'backButton',
		
		icon:		'images/common/ico_back.png',
		
		target:		'MFT.States',
		action:		'back'
	}),
	
	title: MFT.Label.extend({
		classNames:	'options_title',
		
		content:	'Submenu'
	}),
	
	list: MFT.List.extend({
		
		elementId:	'info_nonMedia_options_submenu_list',
		
		itemsOnPage:    5,
		
		items: [
			{
				type:	MFT.Button,
			
				params:		{
					templateName:	'text',
				
					text:	'Test'
				}
			}	
		]
	})
});