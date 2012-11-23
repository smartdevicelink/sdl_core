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

    /** 
      * Function to add new command button to
      * right menu in applinkView screen
      */
    AddSubMenu: function( menuId, menuName ){

        this.listOfOptions.items.push({
                type:       MFT.Button,
                params:     {
                    action:                 'turnOnApplinkSubMenu',
                    target:                 'MFT.ApplinkMediaController',
                    menuId:                 menuId,
                    text:                   menuName,
                    className:              'rs-item',
                    templateName:           'text'
                }                                   
            });

        this.listOfOptions.list.refresh();
    },

    /** 
      * Function to add new command button to
      * Options screen
      */
    AddCommand: function( commandId, params){

        this.listOfOptions.items.push({
                type:       MFT.Button,
                params:     {
                    action:                 'onCommand',
                    target:                 'MFT.ApplinkMediaController',
                    commandId:              commandId,
                    text:                   params.menuName,
                    className:              'rs-item',
                    templateName:           'text'
                }                                   
            });

        this.listOfOptions.list.refresh();
    },

    DeleteCommand: function(commandId){

        var deleted = false;
            count = this.listOfOptions.items.length;
        for(var i = count-1; i >= 0; i--){
            if(this.listOfOptions.items[i].params.commandId == commandId){
                this.listOfOptions.deleteItem(i);
                deleted = true;
            }
        }
        if(!deleted){
            count = MFT.ApplinkModel.subMenuCommands.length;
            for(var i = count-1; i >= 0; i--){
                if(MFT.ApplinkModel.subMenuCommands[i].cmdId == commandId){
                   MFT.ApplinkModel.subMenuCommands.splice(i, 1);
                }
            }
        }

        this.listOfOptions.list.refresh();

    },

    /** 
      * Function to delete command button from
      * right menu in applinkView screen
      */
    DeleteSubMenu: function(menuId){

        var deleted = false;
            count = this.listOfOptions.items.length;
        for(var i = count-1; i >= 0; i--){
            if(this.listOfOptions.items[i].params.menuId == menuId){
                this.listOfOptions.deleteItem(i);
                deleted = true;
            }
        }
        if(deleted){
            return "SUCCESS";
        }else{
            return "INVALID_DATA";
        }

        this.listOfOptions.list.refresh();
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