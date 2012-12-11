/**
 * @name MFT.Button
 * 
 * @desc 
 * 
 * @category	Controlls
 * @filesource	app/controlls/applink/CommandsList.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.ApplinkCommandsList = MFT.List.extend({
	
	/**
	 * Add new button to commands list
	 *
	 * @param commandId:	Number
	 * @param params:		Object
	 * @param appId:		Number
	 */
	AddCommand: function( commandId, params, appId ){
	
		this.get('list.childViews').insertAt(0,
		    MFT.Button.create({
		        action:                 'onCommand',
		        target:                 'MFT.ApplinkAppController',
		        commandId:              commandId,
		        appId:                  appId,
		        text:                   params.menuName,
		        classNames:             'list-item',
		        templateName:           'text'
		    })
		);
	},
    
    /**
	 * Delete button from commands list by command id
	 *
	 * @param commandId:	Number
	 */
    DeleteCommand: function( commandId ){

        this.get('list.childViews').removeObjects(
            this.get('list.childViews').filterProperty( 'commandId' , commandId )
        );

    },
    
    /**
	 * Add new submenu button to commands list
	 *
	 * @param commandId:	Number
	 * @param params:		Object
	 */
    AddSubMenu: function( menuId, params ){

        this.get('list.childViews').insertAt(0,
            MFT.Button.create({
                action:                 'onCommandSubMenu',
                target:                 'MFT.ApplinkAppController',
                menuId:                 menuId,
                appId:					params.appId,
                text:                   params.menuName,
                classNames:             'list-item',
                templateName:           'arrow'
            })
        );

    },
    
    /**
	 * Delete submenu button from commands list by menu id
	 *
	 * @param menuId:		Number
	 */
    DeleteSubMenu: function( menuId ){

        this.get('list.childViews').removeObjects(
            this.get('list.childViews').filterProperty( 'menuId' , menuId )
        );
    }
});