/**
 * @name MFT.VRPopUp
 * 
 * @desc VRPopUp module visual representation
 * 
 * @category    View
 * @filesource  app/view/home/VRPopUp.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */

MFT.VRPopUp = Em.ContainerView.create({

    elementId:          'VRPopUp',

    classNames:         'VRPopUp',

    classNameBindings:      ['VRActive:active'],

    childViews: [
        'popUp',
        'listOfCommands'
    ],

    VRActive:           false,

    AddCommand: function( commandId, vrCommands){

        for(var i=0;i<vrCommands.length;i++){

            this.listOfCommands.items.push({
                type:       MFT.Button,
                params:     {
                    action:                 'onCommand',
                    target:                 'FFW.VR',
                    commandId:              commandId,
                    text:                   vrCommands[i],
                    className:              'rs-item',
                    templateName:           'text'
                }                                   
            });
        }

        this.listOfCommands.list.refresh();

    },

    DeleteCommand: function(commandId){

      var count = this.listOfCommands.items.length;
        for(var i = count-1; i >= 0; i--){
            if(this.listOfCommands.items[i].params.commandId == commandId){
                this.listOfCommands.deleteItem(i);
            }
        }

        this.listOfCommands.list.refresh();

    },

    popUp : Em.View.extend({

        elementId:          'popUp',

        classNames:         'popUp',
    }),

    activateVRPopUp: function(){
        var self = this;
        if(this.VRActive){
            this.set('VRActive', false);
            if(MFT.States.media.applink.active){
                MFT.ApplinkMediaController.onSystemContextSwitcher(MFT.ApplinkMediaController.eSystemContext.application);
            }else{
                MFT.ApplinkMediaController.onSystemContextSwitcher(MFT.ApplinkMediaController.eSystemContext.main);
            }
        }else{
            this.set('VRActive', true);
            MFT.ApplinkMediaController.onSystemContextSwitcher(MFT.ApplinkMediaController.eSystemContext.VR);
        }
    },

     /**
      * List for option on ApplinkOptionsView screen
      */
    listOfCommands: MFT.List.extend({

        elementId:      'VR_list',

        itemsOnPage:    5,
                
        /** Items array */
        items:          new Array()
    })
});