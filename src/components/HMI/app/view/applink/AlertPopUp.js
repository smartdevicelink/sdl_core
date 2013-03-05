/**
 * @name MFT.AlertPopUp
 * 
 * @desc AlertPopUp module visual representation
 * 
 * @category    View
 * @filesource    app/view/applink/AlertPopUp.js
 * @version        2.0
 *
 * @author        Andriy Melnik
 */

MFT.AlertPopUp = Em.ContainerView.create({

    elementId:            	'AlertPopUp',

    classNames:            	'AlertPopUp',

    classNameBindings:      ['active:AlertActive'],

    childViews: [
        'applicationName',
        'image',
        'message1',
        'message2',
        'message3',
        'softbuttons'
    ],

    /**
     * Id of current request
     * @type {Number}
     */
    alertRequestId:     null,

    content1:         	'Title',

    content2:           'Text',

    active:             false,

    timer:              null,

    /**
     * Wagning image on Alert PopUp
     */
    image:  Em.View.extend({
        elementId:              'alertPopUpImage',

        classNames:             'alertPopUpImage',
    }),

    applicationName :    MFT.Label.extend({

        elementId:            	'applicationName',

        classNames:            	'applicationName',

        contentBinding:        	'parentView.appName'
    }),

    message1 : MFT.Label.extend({

        elementId:            	'message1',

        classNames:            	'message1',

        contentBinding:        	'parentView.content1'
    }),

    message2 : MFT.Label.extend({

        elementId:            	'message2',

        classNames:            	'message2',

        contentBinding:        	'parentView.content2'
    }),

    message3 : MFT.Label.extend({

        elementId:           	'message3',

        classNames:				'message3',

        contentBinding:     	'parentView.content3'
    }),

    /**
     * Deactivate PopUp
     */
    deactivate: function( ABORTED ) {
        this.set('active',false);
        clearTimeout(this.timer);
        
        MFT.ApplinkController.alertResponse( ABORTED ? 'ABORTED' : 'SUCCESS', this.alertRequestId );
        
        MFT.ApplinkController.onSystemContextChange();
    },

    /**
     * Container for softbuttons
     */
    softbuttons: Em.ContainerView.extend({
        
        childViews: [
            'buttons'
        ],
   
        buttons: Em.ContainerView.extend({
            elementId:      'alertSoftButtons',

            classNames:     'alertSoftButtons'
        }),
    }),



    /**
     *
     * @desc Function creates Soft Buttons on AlertPoUp
     * 
     * @param {Object} params
     */
    addSoftButtons: function( params, appId ){

        var count = this.get('softbuttons.buttons.childViews').length - 1;
        for(var i = count; i>=0; i--){
            this.get('softbuttons.buttons.childViews').removeObject(
                this.get('softbuttons.buttons.childViews')[0]
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
                this.get('softbuttons.buttons.childViews').pushObject(
                    MFT.Button.create(MFT.PresetEventsCustom, {
                        systemAction:           params[i].systemAction,
                        groupName:              "AlertPopUp",
                        softButtonID:           params[i].softButtonID,
                        icon:                   params[i].image,
                        text:                   params[i].text,
                        classNames:             'list-item softButton ' + softButtonsClass,
                        elementId:              'softButton' + i,
                        templateName:           params[i].image ? 'rightIcon' : 'text',
                        appId:                  appId
                    })
                );
            }
        }
    },

    AlertActive: function( message, alertRequestId ){
        var self = this;
        
        // play audio alert
        if ( message.playTone ) {
            MFT.Audio.play('audio/alert.wav');
        }

        this.set('alertRequestId', alertRequestId);

        this.addSoftButtons( message.softButtons, message.appId );

        this.set('appName',    MFT.ApplinkController.getApplicationModel(message.appId).appName);

        this.set('content1',    message.AlertText1);
        this.set('content2',    message.AlertText2);
        this.set('content3',    message.AlertText3);
        this.set('active',    true);
        MFT.ApplinkController.onSystemContextChange();
        
        clearTimeout(this.timer);
        this.timer = setTimeout(function(){
            self.deactivate();
        }, message.duration);
        
        if( message.ttsChunks ){
            MFT.ApplinkModel.onPrompt(message.ttsChunks, message.duration-100);
        }
    }
});