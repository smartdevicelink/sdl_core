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

        this.get('listOfOptions.list.childViews').insertAt(0,
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
    
    DeleteCommand: function( commandId ){

        this.get('listOfOptions.list.childViews').removeObjects(
            this.get('listOfOptions.list.childViews').filterProperty( 'commandId' , commandId )
        );

    },
    
    AddSubMenu: function( menuId, params ){

        this.get('listOfOptions.list.childViews').insertAt(0,
            MFT.Button.create({
                action:                 'turnOnApplinkSubMenu',
                target:                 'MFT.NonMediaController',
                menuId:                 menuId,
                text:                   params.menuName,
                classNames:             'list-item',
                templateName:           'arrow'
            })
        );

    },

    DeleteSubMenu: function( menuId ){

        this.get('listOfOptions.list.childViews').removeObjects(
            this.get('listOfOptions.list.childViews').filterProperty( 'menuId' , menuId )
        );
    },
	
	listOfOptions: MFT.List.extend({
		
		elementId:	'info_nonMedia_options_list',
		
		itemsOnPage:    5,
		
		items: [
			{
				type:	MFT.Button,
			
				params:		{
					templateName:	'text',
				
					text:	'Exit',
									
					action:		'back',
					target:		'MFT.States'
				}
			},
			{
				type:	MFT.Button,
			
				params:		{
					templateName:	'arrow',
				
					text:	'Device Information',
									
					//action:		'back',
					//target:		'MFT.States'
				}
			}
		]
	})
});