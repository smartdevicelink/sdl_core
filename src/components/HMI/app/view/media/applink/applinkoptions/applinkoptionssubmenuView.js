/**
 * @name MFT.ApplinkOptionsSubMenuView
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/ApplinkOptionsSubMenuView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.ApplinkOptionsSubMenuView = Em.ContainerView.create(MFT.LoadableView,{

    classNames:        ['media_app_sub_menu_view'],

    elementId:          'media_app_sub_menu_view',

    childViews:         [
                            'backButton',
                            'listOfSubmenuButtons',
                            'optionsSubMenuLabel'
                        ],

    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],
        action:            'back',
        target:            'MFT.States',   
        icon:              'images/media/ico_back.png',
    }),

    optionsSubMenuLabel:    MFT.Label.extend({

        elementId:          'optionsSubMenuLabel',

        classNames:         'optionsSubMenuLabel',

        contentBinding:     'MFT.ApplinkMediaController.subMenuLabel'
    }),

    DeleteCommand: function( commandId ){

        this.get('listOfSubmenuButtons.list.childViews').removeObjects(
            this.get('listOfSubmenuButtons.list.childViews').filterProperty( 'commandId' , commandId )
        );

    },

    /** Applink Sub Mennu activate handler */
    applinkSubMenuActivate: function(){
        if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
            MFT.ApplinkOptionsSubMenuView.SubMenuActivate(MFT.MediaController.currentApplinkSubMenuid);
        }else{
            MFT.ApplinkMediaController.set('currentApplinkSubMenuid', null);
        }
    }.observes('MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active'),

    SubMenuActivate: function( ){

        var count = this.listOfSubmenuButtons.items.length;
        if(count > 0){
            for(var i = count-1; i >= 0; i--){
                this.listOfSubmenuButtons.deleteItem(i);
            }
        }

        this.listOfSubmenuButtons.list.refresh();

        count = MFT.ApplinkMediaModel.subMenuCommands.length;
        for(var i = 0; i < count; i++){
            if( MFT.ApplinkMediaController.currentApplinkSubMenuid == MFT.ApplinkMediaModel.subMenuCommands[i].menuParams.parentID ){

                this.listOfSubmenuButtons.items.push({
                    type:       MFT.Button,
                    params:     {
                        action:                 'onCommand',
                        target:                 'MFT.ApplinkMediaController',
                        commandId:              MFT.ApplinkMediaModel.subMenuCommands[i].cmdId,
                        text:                   MFT.ApplinkMediaModel.subMenuCommands[i].menuParams.menuName,
                        parentID:               MFT.ApplinkMediaModel.subMenuCommands[i].menuParams.parentID,
                        appId:                  MFT.ApplinkMediaModel.subMenuCommands[i].appId,
                        icon:                   MFT.ApplinkMediaModel.subMenuCommands[i].menuParams.icon,
                        className:              'rs-item',
                        templateName:           MFT.ApplinkMediaModel.subMenuCommands[i].menuParams.icon ? 'rightIcon' : 'text'
                    }                                   
                });
            }
        }

        this.listOfSubmenuButtons.list.refresh();

    },

     /**
      * List for option on ApplinkOptionsSubMenuView screen
      */
    listOfSubmenuButtons: MFT.List.extend({

        elementId:         'media_app_options_submenu_list',

        itemsOnPage:    5,
                
        /** Items */
            items:      new Array()
    })
});