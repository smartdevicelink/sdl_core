/**
 * @name MFT.AlertManeuverPopUp
 * 
 * @desc AlertManeuverPopUp module visual representation
 * 
 * @category    View
 * @filesource    app/view/applink/AlertManeuverPopUp.js
 * @version        2.0
 *
 * @author        Andriy Melnik
 */

MFT.AlertManeuverPopUp = Em.ContainerView.create({

    elementId:              'AlertManeuverPopUp',

    classNames:             'AlertManeuverPopUp',

    classNameBindings:      ['activate:AlertManeuverActive'],

    childViews: [
        'applicationName',
        //'image',
        //'message1',
        //'message2',
        //'message3',
        'softbuttons',
        'closeButton'
    ],

    content1:           'Title',

    content2:           'Text',

    activate:           false,

    timer:              null,

    /**
     * Wagning image on Alert Maneuver PopUp
     */
    image:  Em.View.extend({
        elementId:              'alertManeuverPopUpImage',

        classNames:             'alertManeuverPopUpImage',
    }),

    applicationName :    MFT.Label.extend({

        elementId:              'applicationName',

        classNames:             'applicationName',

        contentBinding:         'parentView.appName'
    }),

    message1 : MFT.Label.extend({

        elementId:              'message1',

        classNames:             'message1',

        contentBinding:         'parentView.content1'
    }),

    message2 : MFT.Label.extend({

        elementId:              'message2',

        classNames:             'message2',

        contentBinding:         'parentView.content2'
    }),

    message3 : MFT.Label.extend({

        elementId:              'message3',

        classNames:             'message3',

        contentBinding:         'parentView.content3'
    }),

    /**
     * Container for softbuttons
     */
    softbuttons: Em.ContainerView.extend({
        elementId:      'alertManeuverSoftButtons',

        classNames:     'alertManeuverSoftButtons'
    }),

    /**
     * Close button
     */
    closeButton: MFT.Button.create({
        text:                   'Close',
        classNames:             'closeButton softButton',
        action:                 'closeAlertMeneuverPopUp',
        target:                 'MFT.ApplinkController',
        templateName:           'text'
    }),

    /**
     *
     * @desc Function creates Soft Buttons on AlertPoUp
     * 
     * @param {Object} params
     */
    addSoftButtons: function( params ){

        var count = this.get('softbuttons.childViews').length - 1;
        for(var i = count; i>=0; i--){
            this.get('softbuttons.childViews').removeObject(
                this.get('softbuttons.childViews')[0]
            );
        }

        if( params ){

            var softButtonsClass;
            switch(params.length){
                case 1 : softButtonsClass = 'one';
                    break;
                case 2 : softButtonsClass = 'two';
                    break;
                case 3 : softButtonsClass = 'three';
                    break;
                case 4 : softButtonsClass = 'four';
                    break;
            }

            for(var i=0; i<params.length; i++){
                this.get('softbuttons.childViews').pushObject(
                    MFT.Button.create(MFT.PresetEventsCustom, {
                        softButtonID:           params[i].softButtonID,
                        icon:                   params[i].image,
                        text:                   params[i].text,
                        classNames:             'list-item softButton ' + softButtonsClass,
                        elementId:              'softButton' + i,
                        templateName:           params[i].image ? 'rightIcon' : 'text',
                        systemAction:           params[i].systemAction
                    })
                );
            }
        }
    },

    AlertManeuverActive: function( message ){
        var self = this;

        // play audio alert
        if ( message.playTone ) {
            MFT.Audio.play('audio/alert.wav');
        }

        this.addSoftButtons(message.softButtons);
        if( message.ttsChunks ){
            MFT.ApplinkModel.onPrompt(message.ttsChunks.ttsChunks);
        }

        this.set('appName',    MFT.ApplinkController.getApplicationModel(message.appId).appName);

        this.set('activate',    true);
        
        clearTimeout(this.timer);
        this.timer = setTimeout(function(){self.set('activate', false);}, 5000);
    }
});