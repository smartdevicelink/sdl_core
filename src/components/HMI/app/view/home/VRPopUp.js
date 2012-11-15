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

    classNames:         'VRPopUp hide',

    classNameBindings:      ['VRActive:received'],

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

    receiveMessage: function(){
        var self = this;
        if(this.VRActive){
            this.set('VRActive', false);
        }else{
            this.set('VRActive', true);
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