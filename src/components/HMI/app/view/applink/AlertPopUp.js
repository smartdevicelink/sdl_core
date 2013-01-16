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

    classNameBindings:      ['activate:AlertActive'],

    childViews: [
        'applicationName',
        'image',
        'message1',
        'message2',
        'message3',
        'softbuttons'
    ],

    content1:         	'Title',

    content2:           'Text',

    activate:           false,

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
     * Container for softbuttons
     */
    softbuttons: Em.ContainerView.extend({
        elementId:		'alertSoftButtons',

        classNames:		'alertSoftButtons'
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
                    MFT.Button.create({
                        actionDown:        function(){
                            this._super();
                            MFT.ApplinkController.onSoftButtonActionDownCustom(this);
                        },
                        actionUp:        function(){
                            this._super();
                            MFT.ApplinkController.onSoftButtonActionUpCustom(this);
                        },
                        softButtonID:           params[i].softButtonID,
                        icon:                   params[i].image,
                        text:                   params[i].text,
                        classNames:             'list-item softButton ' + softButtonsClass,
                        elementId:              'softButton' + i,
                        templateName:           params[i].image ? 'rightIcon' : 'text'
                    })
                );
            }
        }
    },

    AlertActive: function( message ){
        var self = this;

        FFW.UI.OnSystemContext('ALERT');

        // play audio alert
        if ( message.playTone ) {
            MFT.Audio.play('audio/alert.wav');
        }

        this.addSoftButtons(message.softButtons);
        if( message.ttsChunks ){
            MFT.ApplinkModel.onPrompt(message.ttsChunks.ttsChunks);
        }

        this.set('appName',    MFT.ApplinkController.getApplicationModel(message.appId).appInfo.appName);

        this.set('content1',    message.AlertText1);
        this.set('content2',    message.AlertText2);
        this.set('content3',    message.AlertText3);
        this.set('activate',    true);
        
        clearTimeout(this.timer);
        this.timer = setTimeout(function(){self.set('activate', false);}, message.duration);
    }
});