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
            this.showVRCommands();
        }
    },

    showVRCommands: function(){
        if(this.VRActive){
            this.listOfCommands.items =  MFT.ApplinkModel.voiceRecognitionCommands.slice();
            this.listOfCommands.list.refresh();
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