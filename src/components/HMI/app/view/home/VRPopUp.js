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
        'VRLabel',
        'VRImage',
        'listOfCommands'
    ],


    VRImage : Em.View.extend({

        elementId:          'VRImage',

        classNames:         'VRImage',
    }),

    VRLabel:    MFT.Label.extend({

        elementId:      'VRLabel',

        classNames:     'VRLabel',

        content:        'Speak the command'
    }),

    VRActive:           false,

    popUp : Em.View.extend({

        elementId:          'popUp',

        classNames:         'popUp',
    }),

   AddCommand: function( commandId, vrCommands ){

        for(var j = 0; j < vrCommands.length; j++){
            this.get('listOfCommands.list.childViews').pushObject(
                MFT.Button.create({
                    action:                 'onCommand',
                    target:                 'MFT.ApplinkMediaController',
                    commandId:              commandId,
                    text:                   vrCommands[j],
                    classNames:             'list-item',
                    templateName:           'text'
                })
            );
        }

    },

    DeleteCommand: function( commandId ){

        this.get('listOfCommands.list.childViews').removeObjects(
            this.get('listOfCommands.list.childViews').filterProperty( 'commandId' , commandId )
        );

    },

    CreateInteractionChoise: function( params ){

        for(var i = 0; i<params.choiceSet.length; i++){
            for(var j = 0; j<params.choiceSet[i].vrCommands.length; j++){
                this.get('listOfCommands.list.childViews').pushObject(
                    MFT.Button.create({
                        action:                 'onVRPerformInteractionChoosed',
                        target:                 'MFT.ApplinkMediaController',
                        choiceID:               params.choiceSet[i].choiceID,
                        interactionChoiceSetID: params.interactionChoiceSetID,
                        text:                   params.choiceSet[i].vrCommands[j],
                        classNames:             'list-item',
                        templateName:           'text'
                    })                                   
                );
            }
        }

    },

    DeleteInteractionChoise: function( choiseSetID ){

        this.get('listOfCommands.list.childViews').removeObjects(
            this.get('listOfCommands.list.childViews').filterProperty( 'interactionChoiceSetID' , choiseSetID )
        );

    },

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