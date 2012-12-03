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
		'listOfOptions'
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
	
	AddCommand: function( commandId, params ){

        this.get('listOfOptions.list.childViews').pushObject(
            MFT.Button.create({
                action:                 'onCommand',
                target:                 'MFT.ApplinkMediaController',
                commandId:              commandId,
                text:                   params.menuName,
                classNames:             'list-item',
                templateName:           'text'
            })
        );

    },
	
	listOfOptions: MFT.List.extend({
		
		elementId:	'info_nonMedia_options_list',
		
		itemsOnPage:    5,
		
		items: [
			{
				type:	MFT.Button,
			
				params:		{
					templateName:	'arrow',
				
					text:	'Submenu Test',
									
					action:		'turnOnApplinkSubMenu',
					target:		'MFT.NonMediaController'
				}
			}
		]
	})
});