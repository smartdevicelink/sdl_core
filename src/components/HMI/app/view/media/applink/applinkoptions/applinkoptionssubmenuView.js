
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
                            'listOfSubmenuButtons'
                        ],

    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],     
        action:            'back',
        target:            'MFT.States',   
        icon:              'images/media/ico_back.png',   
    }),

    /** Applink Sub Mennu activate handler */
    applinkSubMenuActivate: function(){
        if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
            MFT.ApplinkOptionsSubMenuView.SubMenuActivate(MFT.MediaController.currentApplinkSubMenuid);
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

        count = MFT.ApplinkModel.subMenuCommands.length;
        for(var i = 0; i < count; i++){
            if( MFT.ApplinkMediaController.currentApplinkSubMenuid == MFT.ApplinkModel.subMenuCommands[i].menuParams.parentID ){

                this.listOfSubmenuButtons.items.push({
                    type:       MFT.Button,
                    params:     {
                        action:                 'onCommand',
                        target:                 'FFW.UI',
                        commandId:              MFT.ApplinkModel.subMenuCommands[i].cmdId,
                        text:                   MFT.ApplinkModel.subMenuCommands[i].menuParams.menuName,
                        parentID:               MFT.ApplinkModel.subMenuCommands[i].menuParams.parentID,
                        className:              'rs-item',
                        templateName:           'text'
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