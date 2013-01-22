/**
 * @name MFT.ApplinkOptionsView
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/ApplinkOptionsView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.ApplinkOptionsView = Em.ContainerView.create(MFT.LoadableView,{

    classNames:        ['media_app_options_view'],

	/** View Id */	
    elementId:          'media_app_options_view',

	/** View Components*/
    childViews:         [
                            'backButton',
                            'listOfOptions',
                            'optionsLabel'
                        ],

    /** Button to return to previous view*/
    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],     
        action:            'back',
        target:            'MFT.States',  
        icon:              'images/media/ico_back.png',   
    }),

    optionsLabel:    MFT.Label.extend({

        elementId:          'optionsLabel',

        classNames:         'optionsLabel',

        content:            'Options'
    }),

    AddCommand: function( commandId, params, icon, appId ){

        this.get('listOfOptions.list.childViews').pushObject(
            MFT.Button.create({
                action:                 'onCommand',
                target:                 'MFT.ApplinkMediaController',
                commandId:              commandId,
                appId:                  appId,
                icon:                   icon,
                text:                   params.menuName,
                classNames:             'list-item',
                templateName:           icon ? 'rightIcon' : 'text'
            })
        );

    },

    DeleteCommand: function( commandId ){

       if( this.get('listOfOptions.list.childViews').filterProperty( 'commandId' , commandId ).length > 0 ){
            this.get('listOfOptions.list.childViews').removeObjects(
                this.get('listOfOptions.list.childViews').filterProperty( 'commandId' , commandId )
            );
        }else{
            MFT.ApplinkOptionsSubMenuView.DeleteCommand( commandId );
        }

    },

    AddSubMenu: function( menuId, menuName ){

        this.get('listOfOptions.list.childViews').pushObject(
            MFT.Button.create({
                action:                 'turnOnApplinkSubMenu',
                target:                 'MFT.ApplinkMediaController',
                menuId:                 menuId,
                text:                   menuName,
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

    /**
      * List for option on ApplinkOptionsView screen
      */
    listOfOptions: MFT.List.extend({

        elementId:      'media_app_options_list',

        itemsOnPage:    5,
                
        /** Items array */
        items:          new Array()
    })
});